/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : MidjDrv_p.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 25/07/2009 10:08:21
************************************************************************/
#ifndef __MidjDrv_p__H__
#define __MidjDrv_p__H__

/***** Include files ***************************************************/

#include "MidjDrv.h"

/***** Defines  ********************************************************/

#define MIDJ_PRE_0 0x81
#define MIDJ_PRE_1 0xA1
#define MIDJ_PRE ((MIDJ_PRE_0 << 8) | MIDJ_PRE_1)

#define MIDJ_PORT EXT_UART_PORT_C

/***** Typedefs ********************************************************/

/***** Private Return Code *********************************************/

/***** Functions Declarations ******************************************/

RC MidjDrv_FixMsg (MSG *Msg);

#endif  /* __MidjDrv_p__H__ */
/* Do not add any thing below this line ! */
