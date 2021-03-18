/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : MidjDrv.c
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 25/07/2009 09:39:58
************************************************************************/


/***** Include files ***************************************************/

#include "MidjDrv_p.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>

MSG *MidjRxMsg=NULL;
BYTE MidjRxIdx=0;
pthread_mutex_t MIDGMutex;
mySerial serialMIDG("/dev/ttyUSB1",115200);
/************************************************************************
* Name       : RC MidjDrv_Init (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC MidjDrv_Init (void)
{

   MidjRxMsg = NULL;
   MidjRxIdx = 0;
   pthread_mutex_init(&MIDGMutex,0);
   return(OK);
}


void* threadMIDGHandler(void* args)
{
    RC rc;
    MSG MIDGmsg;
    MSG* Msg=NULL;
    BYTE len;
    unsigned char r1[100];
    while(1)
    {

         
         MidjDrv_Rx(&Msg);
           // msg=Comm_BuffAlloc(&rc,TRUE);
         if (Msg)
         switch (Msg->Header.OpCode)
         {
            case MIDJ_ID_ATT: Msg->Header.OpCode=0;rc = Algo_SendInsData((MIDJ_InsMsg *)Msg->Data); Comm_BuffFree (Msg, TRUE);Msg=NULL;break;
            //case MIDJ_ID_ATT: t_ins.A[0]=timer2;t_ins.A[1]=timer2;t_ins.A[2]=timer2;rc = Algo_SendInsData((MIDJ_InsMsg *)Msg->Data); Comm_BuffFree (Msg, FALSE);break;
            //case MIDJ_ID_GPS: t_gps.A[0]=timer2;t_gps.A[1]=timer2;t_gps.A[2]=timer2;rc = Algo_SendGpsData((MIDJ_GpsMsg *)Msg->Data);Comm_BuffFree (Msg, FALSE); break;
            default: Msg->Header.OpCode=0;rc = rcMidjDrvWrongInput;Comm_BuffFree(Msg,TRUE);Msg=NULL; break;
         }



            //msg=NULL;
            //MotorDrvRx (&msg);
           // RxMsghandler(msg->Data);
            //std::cout<<px_deg<<endl;
    }
}



RC Algo_SendInsData(MIDJ_InsMsg *InsInfo)

{
   
   RC rc;
   std::cout<<(float)InsInfo->Pitch/100.0<<std::endl;
   //ant_angles=sat_aim(-4, 32.0,35.0, InsInfo->Roll/100.0,InsInfo->Pitch/100.0, InsInfo->Yaw/100.0, 0, 0, 0);
  // rc=update_dcm(&DCM,InsInfo->Roll/100.0, InsInfo->Pitch/100.0,InsInfo->Yaw/100.0);
  // rc=update_angles(&DCM_fix,&DCM,&ant_angles,&Vned,&Vant);
   //Vant.A[0]=ant_angles.A[0];
   //rc=Algo_SendMotorData(&ant_angles);
   return OK;
}


/************************************************************************
* Name       : MidjDrv_Start (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC MidjDrv_Start (void)
{
   RC rc = OK;

   if (!MidjRxMsg)
      MidjRxMsg = Comm_BuffAlloc (&rc, FALSE);
   MidjRxIdx = 0;

   return(rc);
}

/************************************************************************
* Name       : MidjDrv_Stop (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC MidjDrv_Stop (void)
{
   RC rc = OK;

   if (MidjRxMsg)
      rc = Comm_BuffFree (MidjRxMsg, FALSE);

   return(rc);
}

/*********************************************************************************************************
* Name       : RC MidjDrv_Rx (MSG **msg)
* Description: Build MIDJ info from received port
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC MidjDrv_Rx (MSG **msg)
{
   RC rc=OK;
   BYTE len;
   BYTE DataLen;

   *msg = NULL;

   // make sure we have buffer
   if (!MidjRxMsg)
      {
         
      MidjRxMsg = Comm_BuffAlloc (&rc, TRUE);
      MidjRxIdx = 0;
      
      }

         
      if (!MidjRxMsg)
         return(rc);
      if (rc)
      {
         Comm_BuffFree(MidjRxMsg,TRUE);
         return(rc);
      }
   // wait for preamble and ID
   while (MidjRxIdx < 3)
   {      
      len = 1;
      len=serialMIDG.Receive(&(MidjRxMsg->Data[MidjRxIdx]),len);

      

      if (MidjRxIdx == 0)
      {
         if (MidjRxMsg->Data[0] == MIDJ_PRE_0)
            MidjRxIdx++;
        
         
      }
      else  if (MidjRxIdx == 1)
            {
               
               if (MidjRxMsg->Data[1] == MIDJ_PRE_1)
               {
                  MidjRxIdx++;
               }
               else
               {
                  MidjRxIdx = 0;
               }
            }

            
      // Get ID
            else  if (MidjRxIdx == 2)
                  {
                     if (MidjRxMsg->Data[2] == MIDJ_ID_ATT )//|| MidjRxMsg->Data[2] == MIDJ_ID_GPS || MidjRxMsg->Data[2] == MIDJ_ID_UTC)
                        MidjRxIdx++;
                        
                     else
                        MidjRxIdx = 0;
                  }
   }

   // Get Data Len
   if (MidjRxIdx == 3)
   {
      
   len = 1;
   len=serialMIDG.Receive(&(MidjRxMsg->Data[3]),len);
   //rc = ExtUartDrv_Rx (MIDJ_PORT, &(MidjRxMsg->Data[3]), &len);

   if (MidjRxMsg->Data[3] > MAX_DATA_LEN)
      {
      MidjRxIdx = 0;
      return(rcMidjDrvNoInfo);
      }
   MidjRxIdx++;
   }

   // Get Msg data
   // Preamble + ID + Count field + Data_Count + CS_Len
   DataLen = 2 + 1 + 1 + MidjRxMsg->Data[3] + 2;
   len = DataLen - MidjRxIdx;
   len=serialMIDG.Receive(&(MidjRxMsg->Data[4]),len);
   MidjRxIdx += len;

   // check if we got all [Msg_Hdr + Data_Len + CS_Len]
   if (MidjRxIdx >= DataLen)
      {
      MidjRxIdx = 0;

      // check CS
      UINT16 CheckSum;
      rc = CalcFletcherCheckSum(&(MidjRxMsg->Data[2]), DataLen-4, &CheckSum);
      if (rc || CheckSum != ((MidjRxMsg->Data[DataLen-1] << 8) | MidjRxMsg->Data[DataLen-2]))
         return(rcMidjDrvNoInfo);

      // transfer msg to calling user
      MidjRxMsg->Header.OpCode = MidjRxMsg->Data[2];
      *msg = MidjRxMsg;
      MidjRxIdx = 0;
      rc = MidjDrv_FixMsg(MidjRxMsg);


      MidjRxMsg = NULL;
      if (rc)
         return(rc);
      return(OK);
      }

   return(rcMidjDrvNoInfo);
}

/*********************************************************************************************************
* Name       : RC MidjDrv_Rx (MSG *Msg)
* Description: Build MIDJ info from received port
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC MidjDrv_FixMsg (MSG *Msg)
{
   MIDJ_InsMsg *InsMsg;
   MIDJ_GpsMsg *GpsMsg;
   MIDJ_UtcMsg *UtcMsg;
   RC rc = OK;

   switch (Msg->Header.OpCode)
      {
      case MIDJ_ID_ATT:
         InsMsg = ((MIDJ_InsMsg *)Msg->Data);

         InsMsg->xRate = LittleXBig16(InsMsg->xRate);
         InsMsg->yRate = LittleXBig16(InsMsg->yRate);
         InsMsg->zRate = LittleXBig16(InsMsg->zRate);
         InsMsg->xAcc = LittleXBig16(InsMsg->xAcc);
         InsMsg->yAcc = LittleXBig16(InsMsg->yAcc);
         InsMsg->zAcc = LittleXBig16(InsMsg->zAcc);
         InsMsg->Yaw = LittleXBig16(InsMsg->Yaw);
         InsMsg->Pitch = LittleXBig16(InsMsg->Pitch);
         InsMsg->Roll = LittleXBig16(InsMsg->Roll);
         InsMsg->wQuaternion = LittleXBig32(InsMsg->wQuaternion);
         InsMsg->xQuaternion = LittleXBig32(InsMsg->xQuaternion);
         InsMsg->yQuaternion = LittleXBig32(InsMsg->yQuaternion);
         InsMsg->zQuaternion = LittleXBig32(InsMsg->zQuaternion);
         break;

      case MIDJ_ID_GPS:
         GpsMsg = ((MIDJ_GpsMsg *)Msg->Data);

         GpsMsg->Time = LittleXBig32(GpsMsg->Time);
         GpsMsg->Week = LittleXBig16(GpsMsg->Week);
         GpsMsg->Status = LittleXBig16(GpsMsg->Status);
         GpsMsg->Long = LittleXBig32(GpsMsg->Long);
         GpsMsg->Lat = LittleXBig32(GpsMsg->Lat);
         GpsMsg->Alt = LittleXBig32(GpsMsg->Alt);
         GpsMsg->EastVel = LittleXBig32(GpsMsg->EastVel);
         GpsMsg->NorthVel = LittleXBig32(GpsMsg->NorthVel);
         GpsMsg->UpVel = LittleXBig32(GpsMsg->UpVel);
         GpsMsg->PosDop = LittleXBig16(GpsMsg->PosDop);
         GpsMsg->PosAccuracy = LittleXBig16(GpsMsg->PosAccuracy);
         GpsMsg->SpeedAccuracy = LittleXBig16(GpsMsg->SpeedAccuracy);
         break;

      case MIDJ_ID_UTC:
         UtcMsg = ((MIDJ_UtcMsg *)Msg->Data);

         UtcMsg->Time = LittleXBig32(UtcMsg->Time);
         UtcMsg->nanoSec = LittleXBig32(UtcMsg->nanoSec);
         UtcMsg->Year = LittleXBig16(UtcMsg->Year);
         break;

      default: rc = rcMidjDrvWrongInput; break;
      }

   return(rc);
}

