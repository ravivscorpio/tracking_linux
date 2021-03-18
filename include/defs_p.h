/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Defs_p.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 09/10/2009 15:06:17
************************************************************************/
#ifndef __Defs_p__H__
#define __Defs_p__H__


/***** Include files ***************************************************/



#define FAR

/***** Defines  ********************************************************/


typedef unsigned int   BOOL;
typedef unsigned char  BYTE;

typedef char           INT8;
typedef unsigned char  UINT8;

typedef short          INT16;
typedef unsigned short UINT16;

typedef long           INT32;
typedef unsigned long  UINT32;


typedef float  FLOAT32;
#define LittleXBig16(x) (((x << 8) & 0xFF00) | ((x >> 8) & 0xFF))

#define MAX_VER_LEN           16
#define MAX_SN_LEN            16
#define MAX_BD_REV_LEN        16
#define MAX_MODULE_NAME_SIZE  16
#define MAX_SW_FILE_NAME_SIZE 32

/***** Typedefs ********************************************************/


#endif  /* __Defs_p__H__ */
/* Do not add any thing below this line ! */
