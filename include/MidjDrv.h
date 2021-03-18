/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : MidjDrv.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 25/07/2009 10:09:59
************************************************************************/
#ifndef __MidjDrv__H__
#define __MidjDrv__H__


/***** Include files ***************************************************/

#include "defs.h"
#include "rc.h"
#include "Utils.h"
#include "protocol.h"
#include "Comm.h"
#include "serial.h"


/***** Defines  ********************************************************/


/***** Typedefs ********************************************************/


typedef enum
   {
   MIDJ_ID_ATT = 10,
   MIDJ_ID_GPS = 20,
   MIDJ_ID_UTC = 25,

   MIDJ_ID_MAX
   }MIDJ_IdType;


typedef struct
   {
   UINT16 Sync;
   BYTE   ID;
   BYTE   Len;
   UINT32 TimeStamp;
   INT16  xRate;
   INT16  yRate;
   INT16  zRate;
   INT16  xAcc;
   INT16  yAcc;
   INT16  zAcc;
   INT16  Yaw;
   INT16  Pitch;
   INT16  Roll;
   INT32  wQuaternion;
   INT32  xQuaternion;
   INT32  yQuaternion;
   INT32  zQuaternion;
   BYTE   Flags;
   UINT16 CS;
   }MIDJ_InsMsg;


typedef struct
   {
   UINT16 Sync;
   BYTE   ID;
   BYTE   Len;
   UINT32 Time;
   UINT16 Week;
   UINT16 Status;
   INT32  Long;
   INT32  Lat;
   INT32  Alt;
   INT32  EastVel;
   INT32  NorthVel;
   INT32  UpVel;
   UINT16 PosDop;
   UINT16 PosAccuracy;
   UINT16 SpeedAccuracy;
   UINT16 CS;
   }MIDJ_GpsMsg;


typedef struct
   {
   UINT16 Sync;
   BYTE   ID;
   UINT32 Time;
   INT32  nanoSec;
   UINT16 Year;
   BYTE   Month;
   BYTE   Day;
   BYTE   Hour;
   BYTE   Minute;
   BYTE   Second;
   BYTE   Valid;
   UINT16 CS;
   }MIDJ_UtcMsg;

/***** Private Return Code *********************************************/

#define rcMidjDrvWrongInput     (rcErr | rcMidjDrvBase | 0x01)

#define rcMidjDrvNoInfo         (rcInfo | rcMidjDrvBase | 0x01)

/***** Functions Declarations ******************************************/

RC MidjDrv_Init (void);
RC MidjDrv_Start (void);
RC MidjDrv_Stop (void);
RC MidjDrv_Rx (MSG **msg);
void* threadMIDGHandler(void* args);
RC Algo_SendInsData(MIDJ_InsMsg *InsInfo);


#endif  /* __MidjDrv__H__ */
/* Do not add any thing below this line ! */

