/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Defs.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 09/10/2009 14:53:08
************************************************************************/
#ifndef __Defs__H__
#define __Defs__H__

// ##################################################################
// !! Care for all units must be taken when change this file ...
// !! This file is parsing during compilation of all system units as
// !! well for the PCsim program - Please considure changs to be done
// !! on the relevants defs_p.h files rather then change to this file.
// ##################################################################

/***** Defines  ********************************************************/

#define TRUE     1
#define FALSE    0

#define YES      1
#define NO       0

#define ON       1
#define OFF      0

#ifndef NULL
   #define NULL     0
#endif

typedef char     ASCII;

/***** Include files ***************************************************/

#include "defs_p.h"

/***** Typedefs ********************************************************/
#endif  /* __Defs__H__ */
/* Do not add any thing below this line ! */
