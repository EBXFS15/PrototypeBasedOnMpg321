
#ifndef _KVP_KEYVALUEPARSER_
#define _KVP_KEYVALUEPARSER_

/* Example how the key/value file could look like.

[section1]
key1=value1
key2=value2
#key3=value3   -->key3 is now a comment...

[section2]
key1=value2

[section3]
key1=value3

[section4]
key1=value4

*/

//- INCLUDE REFERENCES --------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

/*- FORWARD DECLARATIONS -------------------------------------------------------*/

/*- TYPE DEFINITIONS -----------------------------------------------------------*/
typedef enum {
  Success = 0,
  AttributeNotFound,
  FileNotFound,
  SectionNotFound,
  OutBufferExceeded,
  InternalFailure,

  GetAttributeStatusMax     // This must be after the last status value.
} GetAttributeStatus;


typedef enum {
  KvpAttributeSuccess,           /* Get: The attribute was found: Set: The attribute was set. */
  KvpAttributeNotFound,          /* Get: The attribute was not found. */
  KvpAttributeFileNotFound,      /* Get/Set: The path or the file was not found. */
  KvpAttributeSectionNotFound,   /* Get: The section name was not found. */
  KvpAttributeOutBufferExceeded, /* Get: The length of the output buffer is longer than the parameter. */
				 /*      value "inOutBufferLength" of function arLibGetAttributeByFieldId(). */ 
  KvpAttributeLinkedListCorrupt, /* Set: Internal linked list is corrupt. */
  KvpAttributeNotPermitted,      /* Set: Set attribute isn't permitted with this file name. */
  KvpAttributeStateMax           /* This must be after the last state value. */  
} KvpAttributeState;


/*- CONSTANT DEFINITIONS -------------------------------------------------------*/

/*- CLASS DEFINITIONS ----------------------------------------------------------*/

/*- INLINE FUNCTION DEFINITIONS ------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

  /* Get the attribute from section/key.
     The file is opened and closed per call of this function. */
  GetAttributeStatus kvp_getAttributeValue(/* File identification. */
					   const char* inFilePathP,
					   /* The delimiter between key name and attribute. */
					   const char  inDelimiter,
					   /* Under that [SectionName] is the attribute. '\0' terminated. */
					   const char* inSectionNameP,
					   /* The key of the attribute. '\0' terminated. */
					   const char* inKeyP,
					   /* Buffer contains the returned attribute. */
					   /* The returned buffre is '\0' terminated. */
					   char*       outBufferP,
					   /* Buffer length of outBufferP. */
					   size_t      inOutBufferLength);

  /* Set the attribute from section/key.
     The file is opened and closed per call of this function. The attribute is stored to the file immediately. */
  GetAttributeStatus kvp_setAttributeValue(/* File identification. */
					   const char* inFilePathP,
					   /* The delimiter between key name and attribute. */
					   const char  inDelimiter,
					   /* Under that [SectionName] is the attribute. '\0' terminated. */
					   const char* inSectionNameP,
					   /* The key of the attribute. '\0' terminated. */
					   const char* inKeyP,
					   /* The value to be set, must be zero-terminated ('\0'). */
					   const char* inValueP);


  /* The function opens the file and caches the data in RAM until kvp_fileClose() is called.
     The interface functions kvp_getAttributeByFileId() and kvp_setAttributeByFileId() access the cached data in RAM.
     The function returns 0 on success.
     The function call is rejected with following return values:
       ETXTBSY (file busy)
       ENOENT (no such file or directory)
       EPERM (Operation not permitted)
       EMFILE (Too many open files). */
  int kvp_fileOpen(/* File path contains the absolute path and the name of the file. '\0' terminated. */
		   const char* inFilePathP,
		   /* Returned file id. This value is valid when return value of the function is 0. */
		   uint32_t*   outFileIdP,
		   /* True: File is opened to modify. False: File is just read. */
		   const bool  inModifyFile);

  /* Write the data to the file and close the file.
     If the function returns false the data could not be written, the modifications are lost. */
  bool kvp_fileClose(/* File identification. */
		     const uint32_t inFileId);

  /* Get the attribute from  section/key */
  KvpAttributeState kvp_getAttributeByFileId(/* File identification. */
					     const uint32_t inFileId,
					     /* That's the delimiter between key name and attribute. */
					     const char   inDelimiter,
					     /* Under that SectionName is the attribute. '\0' terminated. */
					     const char*  inSectionNameP,
					     /* That's the key of the attribute. '\0' terminated. */
					     const char*  inKeyP,
					     /* Buffer contains the attribute when the returned function state */
					     /* is "KvpAttributeSuccess". Returned char value is '\0' terminated. */
					     char*        outBufferP,
					     /* Allocated buffer length for outBufferP. */
					     size_t       inOutBufferLength);

  /* Set the attribute to section/key */
  KvpAttributeState kvp_setAttributeByFileId(/* File identification. */
					     const uint32_t inFileId,
					     /* That's the delimiter between key name and attribute. */
					     const char   inDelimiter,
					     /* Under that SectionName is the attribute. '\0' terminated. */
					     const char*  inSectionNameP,
					     /* That's the key of the attribute. '\0' terminated. */
					     const char*  inKeyP,
					     /* Attribute value. '\0' terminated. */
					     const char*  inValueP);

  /* The key value parser interface by "FileId" handles maximum "KeyValueFileLineLengthMax-1" characters per line */
  const uint16_t KeyValueFileLineLengthMax = 300;

#ifdef __cplusplus
}
#endif

#endif



/*- END Of MODULE --------------------------------------------------------------*/

