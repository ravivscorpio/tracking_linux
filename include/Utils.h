/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Utils.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 11/10/2020 09:24:23
************************************************************************/
#ifndef __Utils__H__
#define __Utils__H__


/***** Include files ***************************************************/

#include "defs.h"
#include "rc.h"
#include "protocol.h"

/***** Defines  ********************************************************/

 #define ISR_FUNC __attribute__((__interrupt__, auto_psv, __shadow__))

/***** Typedefs ********************************************************/


/***** Private Return Code *********************************************/

#define rcUtilsWrongPtr                 (rcErr | rcUtilsBase | 0x0)
#define rcUtilsDefISROccureBaseNoStart  (rcErr | rcUtilsBase | 0x1)
#define rcUtilsDefISROccureBaseNoEnd   ((rcErr | rcUtilsBase | 0x1) + 64)

#define rcUtilsWrongISRState            (rcErr | rcUtilsBase | 0x80)

/***** Functions Declarations ******************************************/

RC Utils_Init (void);
RC Utils_Start (void);

RC EnableISR (UINT16 Param);
RC DisableISR (UINT16 Param);

RC WdogEnable (void);
RC WdogDisable (void);
RC WdogRefresh (void);
BOOL isWdogRestart (void);

RC CalcCRC16(BYTE *buf, UINT16 len, UINT16 *res);
RC CalcCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum);
RC CalcMsgCheckSum(MSG *pData, UINT16 *pCheckSum);
RC CalcSwappedCheckSum(MSG *pData, UINT16 *pCheckSum);
RC CalcFletcherCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum);
RC CalcAsciiOrBitsCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum);
RC SwapMsg (MSG *pOrigin, MSG *pSwaped);
RC MemClear(BYTE *MemPtr, UINT16 size);
RC MemCopy(void *dst, const void *src, UINT16 size);

UINT32 LittleXBig32(UINT32 val);

RC Delay (UINT32 delay);
RC Error (RC rc);

#endif  /* __Utils__H__ */
/* Do not add any thing below this line ! */

