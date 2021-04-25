/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Comm.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 16/10/2020 15:57:32
************************************************************************/
#ifndef __Comm__H__
#define __Comm__H__


/***** Include files ***************************************************/

#include "defs.h"
#include "rc.h"
#include "protocol.h"
//#include "Module.h"

/***** Defines  ********************************************************/


/***** Typedefs ********************************************************/
typedef enum
{
   SPI_A_ID,
   SPI_P_ID,
   USART_A_ID,
   USART_B_ID,

   MAX_COMM_DRV_ID
}COMM_DRV_ID;

typedef RC (*ComRxFunc) (FAR MSG** Msg, void *Param);


/***** Private Return Code *********************************************/

#define rcCommInitError     (rcErr | rcCommBase | 0x01)
#define rcCommWrongState    (rcErr | rcCommBase | 0x02)
#define rcCommWrongParam    (rcErr | rcCommBase | 0x03)

#define rcCommNoFreeBuff    (rcInfo | rcCommBase | 0x01)
#define rcCommNoFullBuff    (rcInfo | rcCommBase | 0x02)

/***** Functions Declarations ******************************************/

RC Comm_Init (void);
RC Comm_Start (void);
UINT16 Comm_GetErrCtr (void);
RC Comm_RegisterRX (ComRxFunc Func, COMM_DRV_ID Id);

FAR MSG* Comm_BuffAlloc (RC *rc, BOOL isSafe);
RC Comm_BuffFree (FAR MSG *buff, BOOL isSafe);

RC Comm_PutRxBuff (FAR MSG *buff);
FAR MSG* Comm_GetRxBuff (RC *rc, BOOL isSafe);

RC Comm_TransmitBuff (FAR MSG *msg, COMM_DRV_ID drvId);

#endif  /* __Comm__H__ */
/* Do not add any thing below this line ! */
