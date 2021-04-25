/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Comm.c
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 16/10/2020 15:43:33
************************************************************************/


/***** Include files ***************************************************/

#include "Comm_p.h"

int NumBufInUse;
int FreeBuffOutIdx;

// full messages Q
FAR MSG *FullMsgs[MAX_MSG_BUFFS];
BOOL MsgBankFlag[MAX_MSG_BUFFS];
int FullBuffInIdx;
int FullBuffOutIdx;

ComRxFunc RxFuncList[MAX_COMM_DRV_ID];
BYTE RxFuncRegNum;

static BYTE MyAddr;
UINT16 MemOverRunCtr;
UINT16 MemUseErrCtr;
UINT16 MemGotWrongPtr;
UINT16 MemWithWrongCS;

/************************************************************************
* Name       : Comm_Init (void)
* Description: Initiate internal state
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC Comm_Init (void)
{
   int i;


   /* Initiate all buffers and set them free */
   for (i = 0; i < MAX_MSG_BUFFS; i++)
      {
      MsgBank[i]->Header.Preamble1 = PREAMBLE_VAL1;
      MsgBank[i]->Header.Preamble2 = PREAMBLE_VAL2;
      MsgBank[i]->Header.Addr = 0;

      MsgBankFlag[i] = FALSE;

      FullMsgs[i] = NULL;

      // mark safe sign
      MsgBank[i]->Data[MAX_DATA_LEN-1] = 0xDD;
      }

   NumBufInUse = 0;
   FreeBuffOutIdx = 0;
   FullBuffInIdx = 0;
   FullBuffOutIdx = 0;
   MemOverRunCtr = 0;
   MemUseErrCtr = 0;
   MemGotWrongPtr = 0;
   MemWithWrongCS = 0;

   // get unit address
   //Module_GetComAddr (&MyAddr);

   // reset RX calling bank
   for (i = 0; i < MAX_COMM_DRV_ID; i++)
      RxFuncList[i] = NULL;
   RxFuncRegNum = 0;

   return(OK);
}

/************************************************************************
* Name       : Comm_Start (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC Comm_Start (void)
{

   return(OK);
}

/************************************************************************
* Name       : UINT16 Comm_GetErrCtr (void)
* Description: Return the Error Counter value
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
UINT16 Comm_GetErrCtr (void)
{
   return(MemOverRunCtr);
}

/************************************************************************
* Name       : RC Comm_RegisterRX (ComRxFunc *Func, COMM_DRV_ID Id)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC Comm_RegisterRX (ComRxFunc Func, COMM_DRV_ID Id)
{

   if (!Func || Id >= MAX_COMM_DRV_ID)
      return(rcCommWrongParam);

   if (!RxFuncList[Id])
      RxFuncRegNum++;
   RxFuncList[Id] = Func;

   return(OK);
}

/*********************************************************************************************************
* Name       : FAR MSG* Comm_BuffAlloc (RC *rc, BOOL isSafe)
* Description: Get free message buffer from system pull
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
FAR MSG* Comm_BuffAlloc (RC *rc, BOOL isSafe)
{
   int i;
   MSG *retMsg = NULL;

   if (rc)
      *rc = OK;

   /* check if we have free buffers */
   if (NumBufInUse >= MAX_MSG_BUFFS)
      {
      if (rc)
         *rc = rcCommNoFreeBuff;
      return(NULL);
      }

   if (!isSafe)
      DisableISR(NULL);

   /* look for free buffer */
   for (i = 0; (i < MAX_MSG_BUFFS) && MsgBankFlag[FreeBuffOutIdx]; i++, FreeBuffOutIdx++)
      if (FreeBuffOutIdx >= MAX_MSG_BUFFS-1)
         FreeBuffOutIdx = -1;

   if (MsgBankFlag[FreeBuffOutIdx])
      {
      if (rc)
         *rc = rcCommWrongState;
      if (!isSafe)
         EnableISR(NULL);
      return(NULL);
      }

   /* assign buffer is in use */
   MsgBankFlag[FreeBuffOutIdx] = TRUE;
   NumBufInUse ++;
   retMsg = MsgBank[FreeBuffOutIdx];

   if (!isSafe)
      EnableISR(NULL);

   return(retMsg);
}

/*********************************************************************************************************
* Name       : RC Comm_BuffFree (MSG *buff, BOOL isSafe)
* Description: Free message buffer
* Arguments  :
* Effected   :
* Note(s)    :
*******************************************************************************************************/
RC Comm_BuffFree (FAR MSG *buff, BOOL isSafe)
{
   RC rc = OK;
   int i;

   if (!buff)
      return(rcCommWrongState);

   if (!isSafe)
      DisableISR(NULL);

   // Check if the user used last buffer byte
   if (buff->Data[MAX_DATA_LEN-1] != 0xDD)
      MemOverRunCtr++;

   /* Assign it as free */
   buff->Header.Addr = 0;

   for (i = 0; (i < MAX_MSG_BUFFS) && MsgBank[i] != buff; i++);
   if (MsgBank[i] == buff)
      {
      NumBufInUse --;
      MsgBankFlag[i] = FALSE;
      }
   else
      rc = rcCommWrongState;

   if (!isSafe)
      EnableISR(NULL);

   // check used bank list
   for (i = 0; i < MAX_MSG_BUFFS; i++)
      {
      if (MsgBankFlag[i] != 0 && MsgBankFlag[i] != 1)
         {
         MsgBankFlag[i] = 0; // fix it
         MemUseErrCtr++;
         }
      }


   return(rc);
}

/*********************************************************************************************************
* Name       : RC Comm_PutRxBuff (MSG *buff)
* Description: Receive full buffer from driver
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC Comm_PutRxBuff (FAR MSG *buff)
{
   int i;

   if (!buff)
      return(rcCommWrongParam);


   // verify we got a legal pointer
   for (i = 0; i < MAX_MSG_BUFFS && MsgBank[i] != buff; i++);
   if (buff != MsgBank[i])
      {
      MemGotWrongPtr++;
      return(OK);   // we return OK to fix user state
      }

   /* store msg pointer */
   for (i = 0; i < MAX_MSG_BUFFS && FullMsgs[FullBuffInIdx]; i++, FullBuffInIdx++)
      if (FullBuffInIdx >= MAX_MSG_BUFFS-1)
         FullBuffInIdx = -1;

   if (FullMsgs[FullBuffInIdx])
      return(rcCommWrongState);

   FullMsgs[FullBuffInIdx] = buff;

   return(OK);
}

/*********************************************************************************************************
* Name       : RC Comm_TransmitBuff (MSG *msg, COMM_DRV_ID drvId)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC Comm_TransmitBuff (FAR MSG *msg, COMM_DRV_ID drvId)
{
   return(OK);
}


/*********************************************************************************************************
* Name       : FAR MSG* Comm_GetRxBuff (RC *rc, BOOL isSafe)
* Description: Get full received buffer from com
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
FAR MSG* Comm_GetRxBuff (RC *rc, BOOL isSafe)
{
   int i, t;
   FAR MSG *tmp;
   RC retCode = rcCommNoFullBuff;
   UINT16 CheckSum;
   RC rCode = OK;

   // Run Drv Call Back RX functions
   if (RxFuncRegNum)
      {
      for (t = i = 0; i < RxFuncRegNum; i++)
         {
         for (; t < MAX_COMM_DRV_ID && !RxFuncList[t]; t++);
         if (RxFuncList[t])
            {
            tmp = NULL;
            rCode = (RxFuncList[t])(&tmp, NULL);
            if (tmp)
               {
               if (!isSafe)
                  DisableISR(NULL);
               rCode = Comm_PutRxBuff (tmp);
               if (!isSafe)
                  EnableISR(NULL);
               if (rCode)
                  Comm_BuffFree(tmp, isSafe);
               rCode = OK;
               }
            }
         }
      }


   /* get msg pointer */
   for (i = 0; i < MAX_MSG_BUFFS && !(FullMsgs[FullBuffOutIdx]); i++, FullBuffOutIdx++)
      if (FullBuffOutIdx >= MAX_MSG_BUFFS - 1)
         FullBuffOutIdx = -1;

   if (i >= MAX_MSG_BUFFS || !FullMsgs[FullBuffOutIdx])
      {
      if (rc)
         *rc = rcCommNoFullBuff;
      return(NULL);
      }

   // if we got here means we found waiting Msg
   if (!isSafe)
      DisableISR(NULL);

   tmp = FullMsgs[FullBuffOutIdx];
   FullMsgs[FullBuffOutIdx] = NULL;

   if (!isSafe)
      EnableISR(NULL);

   // verify we got a legal pointer
   for (i = 0; i < MAX_MSG_BUFFS && MsgBank[i] != tmp; i++);
   if (tmp != MsgBank[i])
      {
      if (rc)
         *rc = rcCommNoFullBuff;
      MemGotWrongPtr++;
      return(NULL);
      }

   /* verify Message with valid CS */
   if (tmp)
      {
      CalcMsgCheckSum(tmp, &CheckSum);
      if (tmp->Header.CheckSum != CheckSum)
         {
         Comm_BuffFree (tmp, isSafe);
         tmp = NULL;
         MemWithWrongCS++;
         retCode = rcCommNoFullBuff;
         }
      else
         retCode = OK;
      }

   if (rc)
      *rc = retCode;

   return(tmp);
}


