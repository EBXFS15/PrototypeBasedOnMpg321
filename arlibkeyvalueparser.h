/*- IDENTIFICATION -------------------------------------------------------------
 * 
 * PROJECT     : 
 * 
 * DOMAIN      : Ar - Architecture
 * 
 * SUB-SYSTEM  : Lib - Library
 * 
 * MODULE      : arlibkeyvalueparser.h
 * 
 * DESCRIPTION : 
 * 
 * REFERENCE   : 
 * 
 * AUTHOR      : Michel Grundmann
 * 
 * LAST UPDATE : 
 * 
 * 
 * HISTORY     : 
 * 
 *
 *------------------------------------------------------------------------------
 */ 

#ifndef _ARLIBKEYVALUEPARSER_
#define _ARLIBKEYVALUEPARSER_

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
  Success           =   0,  // 0x00
  AttributeNotFound,        // 0x01
  FileNotFound,             // 0x02
  SectionNotFound,          // 0x03
  OutBufferExceeded,        // 0x04
  InternalFailure,          // 0x05
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

  GetAttributeStatus arLibGetAttributeValue(const char* filePath, 
					    const char delimiter,
					    const char* sectionName,
					    const char* key, 
					    char* outBuffer,
					    size_t inOutBufferLength);

  GetAttributeStatus arLibSetAttributeValue(const char* filePath, 
					    const char delimiter, 
					    const char* sectionName, 
					    const char* key,
					    const char* value);


  /* If return value==0 (value==0: Operation successful) then the function call was successful. */
  /* The function call is rejected with following return values: ETXTBSY (file busy), ENOENT (no such file or directory), */
  /* EPERM (Operation not permitted) and EMFILE (Too many open files). */
  int arLibKeyValueFileOpen(/* File path contains the absolute path and the name of the file. '\0' terminated. */
			    const char*   inFilePathP,
			    /* Returned file id. This value is valid when return value of the function is 0. */
                uint32_t*     outFileIdP,
			    /* True: File is opened to modify. False: File is just read. */
                const bool inModifyFile);

  /* If return value==False then the modifications weren't saved. The modifications are lost. */
  bool arLibKeyValueFileClose(/* File identification. */
			      const uint32_t inFileId);

  KvpAttributeState arLibGetAttributeByFileId(/* File identification. */
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

  KvpAttributeState arLibSetAttributeByFileId(/* File identification. */
					      const uint32_t inFileId,
					      /* That's the delimiter between key name and attribute. */
					      const char   inDelimiter,
					      /* Under that SectionName is the attribute. '\0' terminated. */
					      const char*  inSectionNameP,
					      /* That's the key of the attribute. '\0' terminated. */
					      const char*  inKeyP,
					      /* Attribute value. '\0' terminated. */
					      const char*  inValueP);

  /* The new key value parser reads KeyValueFileLineLengthMax-1 chars per line from file and the line is completed with '\0'. */
  const uint16_t KeyValueFileLineLengthMax = 300;

#ifdef __cplusplus
}
#endif

#endif



/*- END Of MODULE --------------------------------------------------------------*/

