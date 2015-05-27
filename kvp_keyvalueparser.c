

/*- INCLUDE REFERENCES ---------------------------------------------------------*/ 
#include "kvp_keyvalueparser.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h> /* pthread_mutex_t */


/*- FORWARD DECLARATION --------------------------------------------------------*/

/*- TYPE DEFINITIONS -----------------------------------------------------------*/

typedef struct {
  const char*  data;
  size_t       dataSize;
  char*        currentPos;
} Buffer;


/* Linked list of the data. */
typedef struct DataRec {
  struct DataRec* nextDataP;      /* Pointer to next line of the text file (Next linked list element). */
  char            lineBuffer[1];  /* Must be the last attribute. First char of the text line. Text line is '\0' terminated! */
} DataRec;

/* Linked list of the section names. */
typedef struct SectionRec {
  struct SectionRec* nextSectionP;  /* Pointer to next section name list element. */
  DataRec*           sectionDataP;  /* Pointer to data linked list element which contains the section name. */ 
} SectionRec;

typedef struct KvpFileOpen {
  uint8_t     clientCounter;   /* Number of clients which are reading simultaneously this text file. */
  bool        fileIsModified;  /* The flag is set to true when an attribute is set and it is set to false when the data are written. */
  bool        fileExist;       /* File path or file exists. */
  DataRec*    dataListP;       /* Pointer to first linked list element of the text lines. The lines are listed in order of the text file.*/
  DataRec**   dataLastListPP;  /* Pointer to nextDataP of last linked list element. */
  SectionRec* sectionListP;    /* Pointer to first linked list element of the section names. */
  char        filePath[1];     /* Must be the last attribute. First char of the pathname of this text file. Path name is '\0' terminated!*/
} KvpFileOpenStruct;


/*- CONSTANT DEFINITIONS -------------------------------------------------------*/ 

static const uint32_t FileIdBits = 0xffff0000;


/*- VARIABLE DEFINITIONS -------------------------------------------------------*/

static uint32_t            fileOpenArrayLength = 0;     /* Length of the array "fileOpenArrayP". */
static KvpFileOpenStruct** fileOpenArrayP      = NULL;  /* Pointer to array of pointer to struct KvpFileOpenStruct. */


/*- FUNCTIONS DEFINITIONS ------------------------------------------------------*/

static void initBuffer(Buffer* outBufferP, const char* inDataP, size_t inDataSize);
static bool getLine(Buffer* inBufferP, Buffer* outBufferP);
static bool getKeyValuePair(const Buffer* inBufferP, const char inDelimiter, Buffer* outKeyP, Buffer* outValueP);
static bool findKeyValuePair(Buffer* inBufferP, const char* inSearchKey, const char inDelimiter, Buffer* outKeyP, Buffer* outValueP);
static bool findSection(const Buffer* inBufferP, const char* sectionName, Buffer* outBufferP);
static KvpAttributeState setAttributeMerge(const uint32_t inFileId, const char inDelimiter, const char* inSectionNameP, 
					   const char* inKeyP, const char*  inValueP, bool inMergeAttribute);
static bool giveArrayPlace(const char* inFilePathP, const size_t inFilePathLength, uint32_t* outArrayIndex);
static void lockData(void);
static void unlockData(void);


/* To protect the internal data structure of the new key value parser */
static pthread_mutex_t dataProtection = PTHREAD_MUTEX_INITIALIZER;

/*- MEMBER FUNCTION DEFINITIONS ------------------------------------------------*/

static void
initBuffer(Buffer* outBufferP, const char* inDataP, size_t inDataSize)
{
  outBufferP->data       = inDataP;
  outBufferP->dataSize   = inDataSize;
  outBufferP->currentPos = (char*)inDataP;
}


static bool
getLine(Buffer* inBufferP, Buffer* outBufferP)
{
  bool retVal = false;
  if (inBufferP->currentPos < (inBufferP->data + inBufferP->dataSize)) {
    size_t dataSizeToParse = (size_t)((inBufferP->data + inBufferP->dataSize) - inBufferP->currentPos);
    initBuffer(outBufferP, inBufferP->currentPos, dataSizeToParse);

    retVal  = true;
    bool goAhead = true;
    while(goAhead) {
      if (inBufferP->currentPos < (inBufferP->data + inBufferP->dataSize)) {
	if ((*inBufferP->currentPos == '\r') || (*inBufferP->currentPos == '\n')) {
	  goAhead   = false;
	  outBufferP->dataSize = (size_t)(inBufferP->currentPos - outBufferP->data);
	}
	inBufferP->currentPos++;
      } else {
	goAhead = false;
      }
    }
  }
  return retVal;
}


static bool
getKeyValuePair(const Buffer* inBufferP, const char inDelimiter, Buffer* outKeyP, Buffer* outValueP)
{
  bool retVal = false;
  if (inBufferP->currentPos < (inBufferP->data + inBufferP->dataSize)) {
    bool goAhead = true;
    size_t dataSizeToParse = (size_t)((inBufferP->data + inBufferP->dataSize) - inBufferP->currentPos);
    
    size_t idx;
    for (idx = 0; (idx < dataSizeToParse) && goAhead; idx++) {
      if (inBufferP->currentPos[idx] == inDelimiter) {
	goAhead = false;
	retVal  = true;

	initBuffer(outKeyP, inBufferP->currentPos, idx);
	if ((inBufferP->currentPos + idx + 1) < (inBufferP->data + inBufferP->dataSize)) {
	  const char* pos = inBufferP->currentPos +idx +1;
	  initBuffer(outValueP, pos, (size_t)(inBufferP->data + inBufferP->dataSize - pos));
	} else {
	  // Empty value 
      initBuffer(outValueP, outKeyP->data + outKeyP->dataSize + 1, 0);
	}
      }
    }
  }
  return retVal;
}


static bool
findKeyValuePair(Buffer* inBufferP, const char* inSearchKeyP, const char inDelimiter, Buffer* outKeyP, Buffer* outValueP)
{
  bool retVal  = false;
  bool goAhead = true;

  Buffer line;
  initBuffer(&line, 0, 0);

  while (getLine(inBufferP, &line) && goAhead) {

    // Skip commments....
    if (line.dataSize > 0) {
      if (line.data[0] == '#') {
	continue;
      }
    }
    if (getKeyValuePair(&line, inDelimiter, outKeyP, outValueP) && 
	(outKeyP->dataSize == strlen(inSearchKeyP)) &&
	(strncmp(outKeyP->data, inSearchKeyP, outKeyP->dataSize) == 0)) {
      retVal = true;
      goAhead = false;
    }      
  }
  return retVal;
}


static bool
findSection(const Buffer* inBufferP, const char* sectionName, Buffer* outBufferP)
{
  bool retVal = false;
  if (inBufferP->currentPos < (inBufferP->data + inBufferP->dataSize)) {
    const char* startName = NULL;
    const char* endName   = NULL;

    const char* tmp = inBufferP->currentPos;
    bool goAhead = true;
    while((tmp != (inBufferP->data + inBufferP->dataSize)) && goAhead) {
      if (*tmp == '[') {
	if (startName == NULL)
	  startName = tmp+1;
	else {
	  goAhead = false;
	  --tmp;
	}
      }
      if (*tmp == ']') {
	endName = tmp;
	if((strlen(sectionName) == (size_t)(endName - startName)) && 
	   (strncmp(startName, sectionName, (size_t)((endName - startName))) == 0)) {
	  retVal = true;
	} else {
	  startName = NULL;
	  endName   = NULL;
	}
      }
      ++tmp;
    }
    
    if (retVal) {
      endName    = tmp;
      startName -= 1;
      initBuffer(outBufferP, startName, (size_t)(endName - startName));
    }
  }
  return retVal;
}


static void
writeToFile(int           fd,
	    const Buffer* part1P,
	    const char*   sectionName,
	    const char    delimiter,
	    const char*   key,
	    const char*   value,
	    const Buffer* part2P)
{
  // Write part 1
  if (part1P->dataSize > 0) {
    size_t idx;
    for (idx = (size_t)(part1P->currentPos - part1P->data);
	 (part1P->currentPos + idx) < (part1P->data + part1P->dataSize); idx++) {
      write(fd,&(part1P->currentPos[idx]),1);
    }
  }

  // Write section
  if (strlen(sectionName) > 0) {
    write(fd,"[", 1);
    write(fd, sectionName, strlen(sectionName));
    write(fd,"]", 1);
    write(fd,"\n", 1);
  }

  // Write key value pair
  if (strlen(key) > 0) {
    write(fd, key, strlen(key)); // the key value
    write(fd, &delimiter, 1);                // the delimiter
    write(fd, value, strlen(value)) ;        // the value
    write(fd,"\n", 1);                       // the newline
  } 

  // Write part 2
  if (part2P->dataSize > 0) {
    size_t idx;
    for (idx = (size_t)(part2P->currentPos - part2P->data);
	 (part2P->currentPos + idx) < (part2P->data + part2P->dataSize); idx++) {
      write(fd,&(part2P->currentPos[idx]),1);
    }
  }
}


GetAttributeStatus
kvp_getAttributeValue(const char* inFilePathP,
		      const char  inDelimiter,
		      const char* inSectionNameP,
		      const char* inSearchKeyP, 
		      char*       outBufferP,
		      size_t      outBufferLen)
{
  GetAttributeStatus retVal = AttributeNotFound;

  FILE * pFile;
  
  pFile = fopen (inFilePathP, "rb" );
  if (pFile != NULL) {

    size_t        lSize;
    char *        buffer;
    size_t        result;
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = (size_t)(ftell(pFile));
    rewind (pFile);

    if (lSize > 0) {
      
      // allocate memory to contain the whole file:
      buffer = (char*) malloc (sizeof(char)*lSize);

      // copy the file into the buffer:
      result = fread (buffer,1,lSize,pFile);
    
      if (result == lSize) {

	Buffer dataBuffer;
	initBuffer(&dataBuffer, buffer, lSize);

	if (strlen(inSectionNameP) > 0) {

	  Buffer section;
	  if (findSection(&dataBuffer, inSectionNameP, &section)) {
	    // reduce the data amount that must be parsed.
	    initBuffer(&dataBuffer, section.data, section.dataSize);
	  } else {
	    retVal = SectionNotFound;
	  }
	}
	if (retVal != SectionNotFound) {
	  Buffer key;
	  initBuffer(&key,0,0);
	  Buffer value;
	  initBuffer(&value,0,0);


	  if (findKeyValuePair(&dataBuffer, inSearchKeyP, inDelimiter, &key, &value)) {

	    if (value.dataSize <= (outBufferLen-1)) {
	      memcpy(outBufferP, value.data, value.dataSize);
	      outBufferP[value.dataSize] = '\0';
	      retVal = Success;
	    } else {
	      retVal = OutBufferExceeded;
	    }
	  } else {
	    retVal = AttributeNotFound;
	  }
	}
      }
      free (buffer);
    }
    // terminate
    fclose (pFile);
  } else {
    retVal = FileNotFound;
  }
  return retVal;
}


GetAttributeStatus
kvp_setAttributeValue(const char* inFilePathP,
		      const char  inDelimiter,
		      const char* inSectionNameP,
		      const char* inSearchKeyP,
		      const char* inValueP)
{
  GetAttributeStatus retVal = AttributeNotFound;


  const char* path        = strrchr(inFilePathP,'/');
  size_t pathSize         = (path != NULL) ? (size_t)(path-inFilePathP) : 0;	
  char* tmpFileName       = (char*) malloc(pathSize+20);
  
  if (pathSize > 0) {
    strncpy(tmpFileName, inFilePathP, pathSize);
    tmpFileName[pathSize] = '\0';
    strcat(tmpFileName,"/tmpFile001.txt");
  } else {
    strcpy(tmpFileName, "tmpFile001.txt");
  }

  FILE * pFile;  
  pFile = fopen(inFilePathP, "rb");
  if (pFile == NULL) {
    // file doesn't exist...create it
    pFile = fopen(inFilePathP, "ab");
  }
  if (pFile != NULL) {

    size_t        lSize;
    char *        buffer;
    size_t        result;
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = (size_t)(ftell(pFile));
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    
    if (result == lSize) {

      Buffer dataBuffer;
      initBuffer(&dataBuffer, buffer, lSize);

      if (strlen(inSectionNameP) > 0) {

	Buffer section;
	if (findSection(&dataBuffer, inSectionNameP, &section)) {
	  // reduce the data amount that must be parsed.
	  initBuffer(&dataBuffer, section.data, section.dataSize);
	} else {
	  retVal = SectionNotFound;
	}
      }

      bool replaceValue = false;

      Buffer key;
      initBuffer(&key,0,0);
      
      Buffer dummyValue;
      initBuffer(&dummyValue,0,0);

      if (retVal != SectionNotFound) {
	
	if (findKeyValuePair(&dataBuffer, inSearchKeyP, inDelimiter, &key, &dummyValue)) {
	  // We have to replace this key/value pair.
	  replaceValue = true;
	} else {
	  // We have to add this key/value pair
	  replaceValue = false;
	}
      }
      int fd;
      
      // Create the temporary working file
      if ((fd= open(tmpFileName,O_RDWR|O_CREAT|O_TRUNC,0600)) != -1) {
	
	if (retVal == SectionNotFound) {
	  // The requested section was not found. Append the new
	  // section.

	  Buffer part1;
	  initBuffer(&part1, buffer, lSize);
	  	  
	  Buffer part2;
	  initBuffer(&part2, buffer, 0);
	  
	  writeToFile(fd, &part1, inSectionNameP, inDelimiter, inSearchKeyP, inValueP, &part2);
	} else if (replaceValue) {
	  // Update the requested key/value pair.

	  Buffer part1;
	  initBuffer(&part1, buffer, (size_t)(key.data - buffer));
	  
	  size_t startOffset = (size_t)(dummyValue.data - buffer) + dummyValue.dataSize +1;
	  
	  Buffer part2;
	  initBuffer(&part2, &buffer[startOffset], lSize - startOffset);
	  
	  writeToFile(fd, &part1,"", inDelimiter, inSearchKeyP, inValueP, &part2);
	} else {
	  // Create the new key/value pair.

	  Buffer part1;
	  initBuffer(&part1, buffer, (size_t)(dataBuffer.currentPos - buffer));
	  
	  size_t startOffset = (size_t)(dataBuffer.currentPos - buffer);
	  
	  Buffer part2;
	  initBuffer(&part2, &buffer[startOffset], lSize - startOffset);
	  
	  writeToFile(fd, &part1,"", inDelimiter, inSearchKeyP, inValueP, &part2);
	}
	retVal = Success;	  
	close(fd);
      } else {
	retVal = InternalFailure;
      }
    }
    // terminate
    fclose (pFile);
    if (buffer != NULL)  //lint !e774 
      free (buffer);
  } else {
    retVal = FileNotFound;
  }

  if (retVal == Success) {
    (void) remove(inFilePathP);
    (void) rename(tmpFileName, inFilePathP);
  } else {
    (void) remove(tmpFileName);
  }

  free(tmpFileName);

  return retVal;
}


static bool
giveArrayPlace(const char* inFilePathP, const size_t inFilePathLength, uint32_t* outArrayIndex)
{
  /* Mutex isn't used here because the caller of this function is already protected with Mutex. */ 
  uint32_t arrayIndex = ~FileIdBits;

  /* Look for an empty array place for the new client. */
  bool arrayPlaceFound = false;
  uint32_t i;
  for (i=0; (i < fileOpenArrayLength) && !arrayPlaceFound; i++) {
    if (fileOpenArrayP[i] == NULL) {
      arrayIndex = i;
      arrayPlaceFound = true;
    }
  }
  if (!arrayPlaceFound) {
    /* No empty array place for the new client ==> expand the array. */
    if (fileOpenArrayLength < (~FileIdBits)) {
      arrayIndex = fileOpenArrayLength;
      fileOpenArrayLength++;
      fileOpenArrayP = 
	(KvpFileOpenStruct**)realloc((void*)fileOpenArrayP,
				     (size_t)((sizeof(KvpFileOpenStruct*))*fileOpenArrayLength));
      arrayPlaceFound = true;
    }
  }

  if (arrayPlaceFound) {
    /* Save the new client in the array. */
    fileOpenArrayP[arrayIndex] = 
      (KvpFileOpenStruct*)malloc(sizeof(KvpFileOpenStruct)+(sizeof(char)*(inFilePathLength+1)));
    fileOpenArrayP[arrayIndex]->clientCounter  = 1;
    fileOpenArrayP[arrayIndex]->fileIsModified = false;
    fileOpenArrayP[arrayIndex]->fileExist      = false;
    fileOpenArrayP[arrayIndex]->dataListP      = NULL;
    fileOpenArrayP[arrayIndex]->sectionListP   = NULL;
    fileOpenArrayP[arrayIndex]->dataLastListPP = &(fileOpenArrayP[arrayIndex]->dataListP);
    (void)memcpy(fileOpenArrayP[arrayIndex]->filePath, inFilePathP, sizeof(char)*(inFilePathLength+1));
    *outArrayIndex = arrayIndex;
  }

  return arrayPlaceFound; 
}  /* giveArrayPlace. */


int /*errno_t*/
kvp_fileOpen(const char* inFilePathP, uint32_t* outFileIdP, const bool inModifyFile)
{
  *outFileIdP = ~FileIdBits;

  size_t             filePathLength = strlen(inFilePathP);
  KvpFileOpenStruct* fileOpenRecP   = NULL;
  uint32_t           arrayIndex     = ~FileIdBits;
  int /*errno_t*/    retVal         = ENOENT;  /* No such file or directory. */

  lockData();

  /* Check if the file name already in use. */
  uint32_t i;
  bool goAhead = true;
  for (i=0; (i < fileOpenArrayLength) && goAhead; i++) {
    if (fileOpenArrayP[i] != NULL) {
      if (strcmp(inFilePathP, fileOpenArrayP[i]->filePath) == 0) {
	fileOpenRecP = fileOpenArrayP[i];
        arrayIndex = i;
        goAhead = false;
      }
    }
  }
  
  if (fileOpenRecP == NULL) {
    /* File name not in use. */
    /* Open file. */
    FILE* textFile = fopen(inFilePathP, "r");
    if (textFile != NULL) {
      /* File can be opened. File exists. */
      if (giveArrayPlace(inFilePathP, filePathLength, &arrayIndex)) {
	fileOpenRecP            = fileOpenArrayP[arrayIndex];
	fileOpenRecP->fileExist = true;
	
	/* Create linked lists. */
	DataRec**    dataPreviousPP    = &(fileOpenRecP->dataListP);
	DataRec*     dataCurrentP      = NULL;
	SectionRec** sectionPreviousPP = &(fileOpenRecP->sectionListP);
	SectionRec*  sectionCurrentP   = NULL;
	size_t       strLength         = 0;
	char*        lineP             = (char*)calloc(KeyValueFileLineLengthMax, sizeof(char));
	
	/* Read the lines of the text file. */
	while (fgets(lineP, KeyValueFileLineLengthMax, textFile)) {
	  strLength = strlen(lineP);
	  /* Remove '\n' and '\r' at the end of the line */
	  while ((strLength > 0) &&
		 ((lineP[strLength-1] == '\n') || (lineP[strLength-1] == '\r'))) {
	    lineP[strLength-1] = '\0';
	    strLength--;
	  }
	  /* Create data linked list. */
	  dataCurrentP = (DataRec*)malloc(sizeof(DataRec)+(sizeof(char)*(strLength+1)));
	  (void)memcpy(dataCurrentP->lineBuffer, lineP, sizeof(char)*(strLength+1));  //lint !e669
	  
	  dataCurrentP->nextDataP = NULL;
	  *dataPreviousPP         = dataCurrentP;
	  dataPreviousPP          = &(dataCurrentP->nextDataP);
	  
	  /* Test line for [SectionName] */
	  if (lineP[0] == '[') {
	    /* Create section linked list. */
	    sectionCurrentP = (SectionRec*)malloc(sizeof(SectionRec));
	    
	    sectionCurrentP->nextSectionP = NULL;
	    sectionCurrentP->sectionDataP = dataCurrentP;
	    *sectionPreviousPP            = sectionCurrentP;
	    sectionPreviousPP             = &(sectionCurrentP->nextSectionP);
	  }
	}  /* while (fgets(lineP, KeyValueFileLineLengthMax, textFile)). */
	/* Free line buffer */
	free(lineP);
	/* Close file. */
	(void)fclose(textFile);
	
	fileOpenRecP->dataLastListPP = dataPreviousPP;

	*outFileIdP = arrayIndex | FileIdBits;
	retVal = 0;  /* Operation successful. */
      }  /* if (giveArrayPlace(inFilePathP, filePathLength, &arrayIndex)) */
      else {
        retVal = EMFILE;  /* Too many open files */
      }
    }  /* if (textFile != NULL) */
    else {
      /* File can't be opened. */
      if (errno == ETXTBSY) {
        /* File busy. */
	retVal = ETXTBSY;  /* Text file busy. */
      } else {
	if (!inModifyFile) {
	  /* The file isn't modified, just read ==> The file has to exist. */
	  retVal = ENOENT;  /* No such file or directory. */
	}
	else {
	  /* Check file path. */
	  FILE* textFile2 = fopen(inFilePathP, "w");
	  if (textFile2 == NULL) {
            /* file path does not exist. */
	    retVal = ENOENT;  /* No such file or directory. */
	  }  /* if(textFile2 != NULL). */  
	  else {
            /* file path exist. */
	    (void)fclose(textFile2);

	    if (giveArrayPlace(inFilePathP, filePathLength, &arrayIndex)) {

	      *outFileIdP = arrayIndex | FileIdBits;
	      retVal = 0;  /* Operation successful. */
	    } 
	    else {
	      retVal = EMFILE;  /* Too many open files */
	    }
	  }  /* else (textFile2 != NULL). */
	}
      }
    }  /* else (textFile != NULL) */
  }  /* if (fileOpenRecP == NULL) */
  else {
    /* File name already in use. */
    if (fileOpenRecP->fileExist == true) {
      /* File exists. */
      fileOpenRecP->clientCounter++;

      *outFileIdP = arrayIndex | FileIdBits;
      retVal = 0;  /* Operation successful. */
    }
    else {
      /* File doesn't exist. */
      if (!inModifyFile) {
        /* The file isn't modified, just read ==> The file has to exist. */
        retVal = ENOENT;  /* No such file or directory. */
      }
      else {
        /* The file is modified/written ==> The file hasn't to exist. */
	fileOpenRecP->clientCounter++;

	*outFileIdP = arrayIndex | FileIdBits;
	retVal = 0;  /* Operation successful. */  
      }
    } 
  }  /* else (fileOpenRecP == NULL) */

  unlockData();

  return retVal;
}  /* kvp_fileOpen. */


bool
kvp_fileClose(const uint32_t inFileId)
{
  bool     retVal     = false;
  uint32_t arrayIndex = inFileId & (~FileIdBits);

  lockData();

  if ((arrayIndex < fileOpenArrayLength) &&
      (fileOpenArrayP[arrayIndex] != NULL)) {
    KvpFileOpenStruct* fileOpenRecP = fileOpenArrayP[arrayIndex];

    if (fileOpenRecP->clientCounter > 0) {
      fileOpenRecP->clientCounter--;
    }
    
    if (fileOpenRecP->fileIsModified) {
      fileOpenRecP->fileIsModified = false;
      
      /* Write data linked list to file. */
      FILE* textFile = fopen(fileOpenRecP->filePath, "wt");
      
      if (textFile != NULL) {
	size_t   sizeOfChar = sizeof(char);
	size_t   strLength  = 0;
	DataRec* dataP      = fileOpenRecP->dataListP;
	
	while (dataP != NULL) {
	  strLength = strlen(dataP->lineBuffer);
	  dataP->lineBuffer[strLength] = '\n';  /* Set '\n' instead of '\0' */
	  (void)fwrite(dataP->lineBuffer, sizeOfChar, strLength+1, textFile);
	  dataP->lineBuffer[strLength] = '\0';  /* Set back to '\0' */
	  dataP = dataP->nextDataP;
	}  /* (dataP != NULL) */
	
	if (fclose(textFile) == 0) retVal = true;
      }  /* if (textFile != NULL) */
    }  /* if (fileOpenRecP->fileIsModified) */
    else {
      retVal = true;
    }

    if (fileOpenRecP->clientCounter == 0) {
      /* Delete data linked list. */
      DataRec* dataP = fileOpenRecP->dataListP;
      DataRec* dataNextP;
      while (dataP != NULL) {
        dataNextP = dataP->nextDataP;
	free(dataP);
        dataP = dataNextP;       
      }  /* (dataP != NULL) */
      
      /* Delete section linked list. */
      SectionRec* sectionP = fileOpenRecP->sectionListP;
      SectionRec* sectionNextP;
      while (sectionP != NULL) {
        sectionNextP = sectionP->nextSectionP;
	free(sectionP);
        sectionP = sectionNextP;       
      }  /* (sectionP != NULL) */
      
      free(fileOpenRecP);
      fileOpenArrayP[arrayIndex] = NULL;
    }  /* if (fileOpenRecP->clientCounter == 0) */
  }  /* ((arrayIndex < fileOpenArrayLength) && (fileOpenArrayP[arrayIndex] =! NULL)) */

  unlockData();

  return retVal;
}  /* kvp_fileClose. */


KvpAttributeState
kvp_getAttributeByFileId(const uint32_t inFileId,
			 const char     inDelimiter,
			 const char*    inSectionNameP,
			 const char*    inKeyP,
			 char*          outBufferP,
			 size_t         inOutBufferLength)
{
  KvpAttributeState retVal           = KvpAttributeNotFound;
  uint32_t          arrayIndex       = inFileId & (~FileIdBits);
  size_t            oldSectionLength = strlen(inSectionNameP);
  char*             sectionNameP     = (char*)calloc(oldSectionLength+3, sizeof(char));  /* +3: for '[', ']' and '\0'. */

  sectionNameP[0]                  = '[';
  sectionNameP[oldSectionLength+1] = ']';
  sectionNameP[oldSectionLength+2] = '\0';
  (void)memcpy(sectionNameP+1, inSectionNameP, sizeof(char)*(oldSectionLength));

  lockData();

  if (((inFileId & FileIdBits) == FileIdBits) &&
      (arrayIndex < fileOpenArrayLength) &&
      (fileOpenArrayP[arrayIndex] != NULL)) {
    /* Find section name. */
    SectionRec* sectionP = fileOpenArrayP[arrayIndex]->sectionListP;
    bool        found    = false;

    while (sectionP != NULL) {
      if (strcmp(sectionP->sectionDataP->lineBuffer, sectionNameP) == 0) {
        found = true;
        break;  /* Leave while loop. */
      }
      sectionP = sectionP->nextSectionP;
    }  /* while (sectionP != NULL) */

    if (found == true) {
      /* Section found. Find key and delimiter */
      found = false;
      DataRec* dataP      = sectionP->sectionDataP->nextDataP;
      size_t   keyLength  = strlen(inKeyP);
      size_t   lineLength = 0; 

      while ((dataP != NULL) &&
	     (dataP->lineBuffer[0] != '[')) {  /* Section check. The minimum length of a line is 1 ('\0'). */
      
	if (strncmp(dataP->lineBuffer, inKeyP, keyLength) == 0) {
	  lineLength = strlen(dataP->lineBuffer);

          if ((lineLength > keyLength) &&
	      (dataP->lineBuffer[keyLength] == inDelimiter)) {
	    found = true;
	    break;  /* Leave while loop. */
	  }
	}
	
	dataP = dataP->nextDataP;
      }  /* while ((dataP != NULL) && (dataP->lineBuffer[0] != '[')) */

      if (found == true) {
        /* Section, Key and delimiter found. */
        size_t attributeLength = lineLength - keyLength - 1;

        if (inOutBufferLength > attributeLength) {  /* plus '\0' */
	  (void)memcpy(outBufferP, dataP->lineBuffer+keyLength+1, sizeof(char)*(attributeLength+1));  /* '\0' is included. */
	  retVal = KvpAttributeSuccess;
	}
        else {
	  retVal = KvpAttributeOutBufferExceeded;
	}
      }
    }  /* if (found == true) */
    else {
      /* Section not found. */
      retVal = KvpAttributeSectionNotFound;
    }  /* else (foundd == true) */
  }  /* ((arrayIndex < fileOpenArrayLength) && (fileOpenArrayP[arrayIndex] =! NULL)) */
  else {
    retVal = KvpAttributeFileNotFound;
  }

  unlockData();

  /* Free buffers */
  free(sectionNameP);

  return retVal;
}  /* kvp_getAttributeByFileId */


static KvpAttributeState 
setAttributeMerge(const uint32_t inFileId,
		  const char   inDelimiter,
		  const char*  inSectionNameP,
		  const char*  inKeyP,
		  const char*  inValueP,
		  bool         inMergeAttribute)
{
  KvpAttributeState retVal     = KvpAttributeSuccess;
  uint32_t            arrayIndex = inFileId & (~FileIdBits);

  /* Find section name. */
  SectionRec*  sectionP          = fileOpenArrayP[arrayIndex]->sectionListP;
  SectionRec** sectionPreviousPP = &(fileOpenArrayP[arrayIndex]->sectionListP);
  size_t       keyLength         = strlen(inKeyP);
  size_t       attributeLength   = strlen(inValueP);
  bool         sectionFound      = false;
  
  while (sectionP != NULL) {
    if (strcmp(sectionP->sectionDataP->lineBuffer, inSectionNameP) == 0) {
      sectionFound = true;
      break;  /* Leave while loop. */
    }
    sectionPreviousPP = &(sectionP->nextSectionP);
    sectionP          = sectionP->nextSectionP;
  }  /* while (sectionP != NULL) */
  
  if (sectionFound == true) {
    /* Section found. Find key and delimiter */
    bool      keyFound       = false;
    DataRec*  dataP          = sectionP->sectionDataP->nextDataP;
    DataRec** dataPreviousPP = &(sectionP->sectionDataP->nextDataP);
    size_t    lineLength     = 0; 
    
    while ((dataP != NULL) &&
	   (dataP->lineBuffer[0] != '[')) {  /* Section check. The minimum length of a line is 1 ('\0'). */
      
      if (strncmp(dataP->lineBuffer, inKeyP, keyLength) == 0) {
	lineLength = strlen(dataP->lineBuffer);
	
	if ((lineLength > keyLength) &&
	    (dataP->lineBuffer[keyLength] == inDelimiter)) {
	  keyFound = true;
	  break;  /* Leave while loop. */
	}
      }
      
      dataPreviousPP = &(dataP->nextDataP);
      dataP          = dataP->nextDataP;
    }  /* while ((dataP != NULL) && (dataP->lineBuffer[0] != '[')) */
    
    if (keyFound == true) {
      /* Section, Key and delimiter found. */
      size_t oldAttributeLength = lineLength - keyLength - 1;  /* -1: Delimiter length. */
      
      if (attributeLength > oldAttributeLength) {
	dataP = 
	  (DataRec*)realloc(dataP, 
			    (size_t)(sizeof(DataRec)+((sizeof(char))*(keyLength+attributeLength+2)))); /* +2: Delimiter + '\0'. */
	*dataPreviousPP = dataP;
      }
      
      (void)memcpy(dataP->lineBuffer+keyLength+1, inValueP, sizeof(char)*(attributeLength+1));  /* +1: '\0' is also copied. */
    }
    else {
      /* Section found but Key and delimiter not found. */
      if (inMergeAttribute) {
	retVal = KvpAttributeNotFound;
      } 
      else {
	/* Create a new data linked list element for the attribute. */
	DataRec* localDataP = 
	  (DataRec*)malloc(sizeof(DataRec)+(sizeof(char)*(keyLength+attributeLength+2))); /* +2: Delimiter + '\0'. */
	(void)memcpy(localDataP->lineBuffer, inKeyP, sizeof(char)*(keyLength));
	localDataP->lineBuffer[keyLength] = inDelimiter;
	(void)memcpy(localDataP->lineBuffer+keyLength+1, inValueP, sizeof(char)*(attributeLength+1)); /* +1: '\0' is also copied. */
	
	if (*dataPreviousPP == NULL) {
	  /* Data linked list is at the end ==> Append the new attribute. */
	  *dataPreviousPP       = localDataP;
	  localDataP->nextDataP = NULL;
	  fileOpenArrayP[arrayIndex]->dataLastListPP = &(localDataP->nextDataP);
	} else {
	  /* Data linked list is not at the end ==> Link between the new attribute. */
	  localDataP->nextDataP = *dataPreviousPP;
	  *dataPreviousPP       = localDataP;
	}
      }  /* else (inMergeAttribute) */
    }  /* else (keyFound == true) */
    
    fileOpenArrayP[arrayIndex]->fileIsModified = !inMergeAttribute;
  }  /* if (sectionFound == true) */
  else {
    /* Section not found. */
    if (inMergeAttribute) {
      retVal = KvpAttributeSectionNotFound;
    } 
    else {
      /* Extend the data and the section list. */
      if ((fileOpenArrayP[arrayIndex]->dataLastListPP != NULL) &&
	  (*(fileOpenArrayP[arrayIndex]->dataLastListPP) == NULL) &&
	  (*sectionPreviousPP == NULL)) {
	/* Create a new data linked list element for the section. */
	size_t sectionLength = strlen(inSectionNameP);
	
	DataRec* localDataP = 
	  (DataRec*)malloc(sizeof(DataRec)+(sizeof(char)*(sectionLength+1)));  /* +1: '\0' */
	(void)memcpy(localDataP->lineBuffer, inSectionNameP, sizeof(char)*(sectionLength+1));
	
	/* Append the data list. Insert the section */
	*(fileOpenArrayP[arrayIndex]->dataLastListPP) = localDataP;
	localDataP->nextDataP = NULL;
	fileOpenArrayP[arrayIndex]->dataLastListPP = &(localDataP->nextDataP);
	
	/* Create a new section linked list element. */
	SectionRec* localSectionP = (SectionRec*)malloc(sizeof(SectionRec));
	
	/* Append the section list. */
	localSectionP->nextSectionP = NULL;
	localSectionP->sectionDataP = localDataP;
	*sectionPreviousPP          = localSectionP;
	
	/* Create a new data linked list element for the attribute. */
	localDataP = (DataRec*)malloc(sizeof(DataRec)+(sizeof(char)*(keyLength+attributeLength+2)));
	/* +2: Delimiter + '\0'. */ 
	(void)memcpy(localDataP->lineBuffer, inKeyP, sizeof(char)*(keyLength));
	localDataP->lineBuffer[keyLength] = inDelimiter;
	(void)memcpy(localDataP->lineBuffer+keyLength+1, inValueP, sizeof(char)*(attributeLength+1));  /* +1: '\0' is also copied.*/
	
	/* Append the data list. Insert the attribute */
	*(fileOpenArrayP[arrayIndex]->dataLastListPP) = localDataP;
	localDataP->nextDataP = NULL;
	fileOpenArrayP[arrayIndex]->dataLastListPP = &(localDataP->nextDataP);
	
	fileOpenArrayP[arrayIndex]->fileIsModified = !inMergeAttribute;
      }
      else {
	retVal = KvpAttributeLinkedListCorrupt;
      }      
    }  /* else (inMergeAttribute) */
    
  }  /* else (sectionFound == true) */

  return retVal;
}  /* setAttributeMerge */


KvpAttributeState 
kvp_setAttributeByFileId(const uint32_t inFileId,
			 const char     inDelimiter,
			 const char*    inSectionNameP,
			 const char*    inKeyP,
			 const char*    inValueP)
{
  uint32_t arrayIndex        = inFileId & (~FileIdBits);
  KvpAttributeState retVal = KvpAttributeFileNotFound;

  lockData();

  if (((inFileId & FileIdBits) == FileIdBits) &&
      (arrayIndex < fileOpenArrayLength) &&
      (fileOpenArrayP[arrayIndex] != NULL)) {
    /* Frame section name with brackets. */
    size_t sectionLength = strlen(inSectionNameP);
    char*  sectionNameP  = (char*)calloc(sectionLength+3, sizeof(char));  /* +3: for '[', ']' and '\0'. */

    sectionNameP[0]               = '[';
    sectionNameP[sectionLength+1] = ']';
    sectionNameP[sectionLength+2] = '\0';
    (void)memcpy(sectionNameP + 1, inSectionNameP, sizeof(char)*(sectionLength));
  
    retVal = setAttributeMerge(inFileId, inDelimiter, sectionNameP, inKeyP, inValueP, false);

    /* Free buffers */
    free(sectionNameP);
  }
  else {
    /* Set attribute isn't permitted with this file name. */
    retVal = KvpAttributeNotPermitted;      
  }  /* ((arrayIndex < fileOpenArrayLength) && (fileOpenArrayP[arrayIndex] =! NULL)) */

  unlockData();

  return retVal;
}  /* kvp_setAttributeByFileId */


static void
lockData(void)
{
  pthread_mutex_lock(&dataProtection);
}


static void
unlockData(void)
{
  pthread_mutex_unlock(&dataProtection);
}


/* - END OF MODULE ------------------------------------------------------------- */
