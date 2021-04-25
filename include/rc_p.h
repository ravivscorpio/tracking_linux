/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : RC_p.H
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 13/02/2009 09:17:57
************************************************************************/
#ifndef __RC_p__H__
#define __RC_p__H__


/***** Include files ***************************************************/


/***** Defines  ********************************************************/

#define rcErr    0x8000
#define rcInfo   0x0000

#define rcIsErr(rcode) (((rcode) & 0x8000) ? TRUE : FALSE)
#define rcIsInfo(rcode)(((rcode) & 0x8000) ? FALSE : TRUE)


/***** Typedefs ********************************************************/


/***** Private Return Code *********************************************/


#define rcMemMgrBase           0x0005000000



#define rcInitBase         0x0100
#define rcUtilsBase        0x0200
#define rcCommBase         0x0300
#define rcAlgoBase         0x0400
#define rcIOportsBase      0x0500
#define rcArincDrvBase     0x0600
#define rcUartDrvBase      0x0700
#define rcEEpromBase       0x0800
#define rcAdcBase          0x0900
#define rcBitBase          0x0A00
#define rcInterruptBase    0x0B00
#define rcModuleBase       0x0C00
#define rcAdcMngBase       0x0D00
#define rcFlashBase        0x0E00
#define rcExtUartDrvBase   0x0F00
#define rcMidjDrvBase      0x1000
#define rcHemDrvBase       0x1100
#define rcMagDrvBase       0x1200

/***** Functions Declarations ******************************************/


#endif  /* __RC_p__H__ */
/* Do not add any thing below this line ! */
