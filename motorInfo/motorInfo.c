
/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : MotorInfo.c
* AUTHORS       : 
* CREATE TIME   : 
************************************************************************/

/***** Include files ***************************************************/

#include "motorInfo.h"
#include "Utils.h"
//#include "uartdrv.h"
#include "Comm.h"
#include <math.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>

using namespace std;


//#define _LOG_


mySerial serial("/dev/ttyUSB0",230400);
mySerial serialR("/dev/ttyUSB1",230400);

MSG *MotorRxMsg;
BYTE MotorRxIdx;
int FIFOin,FIFOout;


INT32 kp;
INT32 accl;
INT32 deccl;
INT32 pm;
INT32 spp;

INT32 mo;
INT32 mf;
ACU_homing_stat_t homing_status;
ACU_deicing_stat_t deicing_status;
// current Status
MotorStatus_t status;
MotorFailStatus_t MotorFailStatusWord;

// Motor Temperature
INT32 MotorTemp;

char* Ver[MAX_VER_LEN];
BOOL VerUpdated;

char* VerCheckSum[MAX_VER_LEN];
BOOL VerCheckSumUpdated;

char* BootVer[MAX_VER_LEN];
BOOL BootVerUpdated;

char* BootVerCheckSum[MAX_VER_LEN];
BOOL BootVerCheckSumUpdated;

char* DriverSN[MAX_VER_LEN];
BOOL DriverSNUpdated;

char* DriverRev[MAX_VER_LEN];
BOOL DriverRevUpdated;

char* EncoderSN[MAX_VER_LEN];
BOOL EncoderSNUpdated;

char* SystemSN[MAX_VER_LEN];
BOOL SystemSNUpdated;

UINT32 ZeroCal;
UINT32 ZeroCalRead;

INT32 px;

// the estimated location the axis should be in 5ms
double est_deg;

// the location that the axis should be now, estimated 5 ms ago
double pred_est_deg;

// the location the axis should be now
double curr_deg;

BOOL CommVerified;

BOOL ConfigCheckStat;





double px_deg;

// target position and position offset
double px_deg_err; 

// estimated target position and current position offset
double px_est_deg_err; 


// Calibratoin Data
// command data
UINT8 TargetAddr;
UINT16 Msg_op;
    
// NIP data
double allowed_err;
BOOL isNIP;

double MotorErrHist[MOTOR_ERROR_HISTORY_SIZE];
int MotorErrIdx;
double MotorErrAccumulator;

// MF status
BOOL MotorFail;
BOOL ReadMotorFail;

// prebuilt, frequently used,  commands

UINT8 MotorBeginMotionCmd[MOTOR_CMD_DATA_LENGTH];
UINT8 MotorMotorOnCmd[MOTOR_CMD_DATA_LENGTH];
UINT8 MotorGetPxCmd[MOTOR_CMD_LENGTH];
UINT8 MotorGetStatusCmd[MOTOR_CMD_LENGTH];
UINT8 MotorGetTempCmd[MOTOR_CMD_LENGTH];


char *m_name;

// TBD : parameters
void *SERVOCmd_ptr; // point to the custom command data structure. Will set there the values that were recieved fromt the SERVO

// These values are the values of the expected configuraiton
UINT32 ConfigAccl;
UINT32 ConfigDecl;
UINT32 ConfigStopDecl;
UINT32 ConfigSPP;
UINT32 ConfigUm;
UINT32 ConfigVer;
UINT32 ConfigPM;

//	UINT32 ConfigHRL1;
//	UINT32 ConfigHRL2;
//	UINT32 ConfigLRL1;
//	UINT32 ConfigLRL2;
UINT32 ConfigXmodLow;
UINT32 ConfigXmodHigh;
UINT32 ConfigYmodLow;
UINT32 ConfigYmodHigh;

FLOAT32 ConfigTL1;
FLOAT32 ConfigTL2;
FLOAT32 ConfigTL3;
FLOAT32 ConfigPeakDurMin;
FLOAT32 ConfigPeakDurMax;
FLOAT32 ConfigPeakLimMin;
FLOAT32 ConfigPeakLimMax;
UINT32 ConfigRefMode;
//	UINT32 ConfigEchoMode;

//	UINT32 ConfigUI_1;
UINT32 ConfigUI_6;
UINT32 ConfigUI_7;
UINT32 ConfigUI_8;
UINT32 ConfigUI_9;
UINT32 ConfigUI_10;
UINT32 ConfigUI_15;
UINT32 ConfigUI_16;
UINT32 ConfigUI_17;
UINT32 ConfigUI_CalVer;

UINT32 ConfigCP_17;
    VEC motor_angles;

#define PI 3.1415926535897932384626433832795
unsigned char   rrr[4096];
MSG msg_data;
MSG* msg=NULL;


pthread_mutex_t myMutex;

void Motor_init()
{
    pthread_mutex_init(&myMutex,0);
    FIFOin=0,FIFOout=0;
}

void* threadRcvHandler(void* args)
{
    RC rc;
    while(1)
    {
           // msg=Comm_BuffAlloc(&rc,TRUE);
           // rc=Comm_BuffFree(msg,TRUE);
            //msg=NULL;
            //MotorDrvRx (&msg);
           // RxMsghandler(msg->Data);
            //std::cout<<px_deg<<endl;
    }
}

void* threadRcv(void* args)
{
    int bytelen;
    RC rc=OK;
    while(1)
    {
        pthread_mutex_lock(&myMutex);
        MotorDrvRx(&msg);
        if (msg)
            if (msg->Header.OpCode == 0x11){
                RxMsghandler(msg->Data);
                std::cout<<px_deg<<endl;
                rc=Comm_BuffFree(msg,TRUE);
            
        }
        
        pthread_mutex_unlock(&myMutex);
        //usleep(120);
    }
}


void* threadSend(void* args)
{
    BYTE  azimuth_angle=90;
    time_t startTime = clock();
    time_t check = 0;
    unsigned int kkk,n;
    RC rc;
    float T=9;//sec
    float Ts=5;//msec
    float rep=1;
    float Total_time;
    

    while(1) //program main loop
    {
	
        check = float(clock() - startTime)/CLOCKS_PER_SEC*1000;
        //std::cout<<"S"<<check<<endl;
        if( check >= Ts) //after 5 milliseconds have elapsed
        {
            
            startTime = clock();
            motor_angles.A[0]=(double)azimuth_angle*sin(2*PI*(1/T)*(Ts/1000.0)*n);
            startTime = clock();
            rc=SendMotorData(&motor_angles);
            check = 0;
            kkk++;
            n++;
            if (n*Ts>=rep*T*1000)
            {
                n=0;
            }		
            usleep(3000);
        }
    }
}



UINT8* BuildServoCmd(UINT8* ptr, UINT16 cmd_id, UINT8 idx, UINT8 type, UINT8 data_type)
{
    // ptr was verified on a higher level
    
    servo_cmd_t *cmd_ptr;
    UINT16 crc_16;

   memClear(ptr, 7);

    cmd_ptr = (servo_cmd_t*)ptr;

    cmd_ptr->magic_1 = 0x49;
    cmd_ptr->magic_2 = 0x5D;
    cmd_ptr->cmd_id_1 = (UINT8)((cmd_id & 0x3F00)>>8);
    cmd_ptr->cmd_id_2 = (UINT8)((cmd_id & 0xFF));

    cmd_ptr->idx_1 = idx & 0x03; 
    cmd_ptr->idx_2 = (idx & 0x3c) >> 2;
    cmd_ptr->cmd_type = type;
    cmd_ptr->data_type = data_type;
    cmd_ptr->error = SERVO_NO_ERROR;
    cmd_ptr->command_dir = SERVO_TO_DRIVER;

    crc_16 = calc_servo_crc16(0, (UINT8*)cmd_ptr + 2, 3);
    cmd_ptr->crc_16_1 = (UINT8)((crc_16 & 0xFF00) >> 8);
    cmd_ptr->crc_16_2 = (UINT8)(crc_16 & 0x00FF);

    return(ptr + sizeof(servo_cmd_t));
}




UINT8* BuildServoDataCmd(UINT8* ptr, UINT16 cmd_id, UINT8 idx, UINT8 data_type, void* data_ptr, UINT8 cmd_dir, UINT8 cmd_type)
{
    // pointers were verified on a higher level
    servo_data_cmd_t *cmd_ptr;
    UINT32 dat;
    UINT16 crc_16;

    memClear(ptr, 11);

    dat = *(UINT32*)data_ptr;

    cmd_ptr = (servo_data_cmd_t*)ptr;

    cmd_ptr->magic_1 = 0x49;
    cmd_ptr->magic_2 = 0x5D;
    cmd_ptr->cmd_id_1 = (UINT8)((cmd_id & 0x3F00)>>8);
    cmd_ptr->cmd_id_2 = (UINT8)((cmd_id & 0xFF));
	cmd_ptr->idx_1 = idx & 0x03; 
    cmd_ptr->idx_2 = (idx & 0x3c) >> 2;
    cmd_ptr->cmd_type = cmd_type;
    cmd_ptr->error = SERVO_NO_ERROR;
    cmd_ptr->command_dir = cmd_dir;

    cmd_ptr->data_type = data_type;

    cmd_ptr->data_1 = (UINT8)(dat & 0x000000FF);
    cmd_ptr->data_2 = (UINT8)((dat & 0x0000FF00) >> 8);
    cmd_ptr->data_3 = (UINT8)((dat & 0x00FF0000) >> 16);
    cmd_ptr->data_4 = (UINT8)((dat & 0xFF000000) >> 24);

    crc_16 = calc_servo_crc16(0, (UINT8*)cmd_ptr + 2, 7);
    cmd_ptr->crc_16_1 = (UINT8)((crc_16 & 0xFF00) >> 8);
    cmd_ptr->crc_16_2 = (UINT8)(crc_16 & 0x00FF);

    return(ptr + sizeof(servo_data_cmd_t));
}

RC MotorDrvRx (MSG **msg)
{
    RC rc=OK;
    BYTE len;

    *msg = NULL;
    
    if (!MotorRxMsg)
      {
        MotorRxMsg = Comm_BuffAlloc (&rc, TRUE);
        MotorRxIdx = 0;
      }
    if (!MotorRxMsg)
        return(rc);
        
    if (MotorRxIdx < 2)
    {
        
        len = 1;
        len = serial.Receive(&(MotorRxMsg->Data[MotorRxIdx]),len);
        //rc=UartDrv_Rx (MOTOR_PORT, &(MotorRxMsg->Data[MotorRxIdx]), &len);


        if (MotorRxIdx == 0)
        {

            if (MotorRxMsg->Data[0] == Motor_PRE_0)
                MotorRxIdx++;

        }
        else if (MotorRxIdx == 1)
        {
            if (MotorRxMsg->Data[1] == Motor_PRE_1)
                MotorRxIdx++;
        	else
            	MotorRxIdx = 0;
        }
	}
        // Get ID
    if (MotorRxIdx > 1)    
	{

		len = Max_RxMotorMsg - MotorRxIdx;
        len = serial.Receive(&(MotorRxMsg->Data[MotorRxIdx]),len);
 
  			//rc=UartDrv_Rx (MOTOR_PORT, &(MotorRxMsg->Data[MotorRxIdx]), &len);

        MotorRxIdx += len;
	}
	if (MotorRxIdx >=  Max_RxMotorMsg)
    {
       
      MotorRxIdx = 0;
      MotorRxMsg->Header.OpCode = 0x11;
      *msg = MotorRxMsg;
      if (rc)
         return(rc);
      MotorRxMsg = NULL;
      return(OK);
     }

   return(1);
}


RC SendMotorData(VEC *ant_angles)
{
   RC rc;
int bytelen=0;

   FLOAT32 YYY;



   rc = OK;




      
		  UINT8 MotorBeginMotionCmd[MOTOR_CMD_DATA_LENGTH];
		  UINT8 MotorMotorOnCmd[MOTOR_CMD_DATA_LENGTH];
	   	  UINT8 MotorGetPxCmd[MOTOR_CMD_LENGTH];
		  UINT8 MotorGetStatusCmd[MOTOR_CMD_LENGTH];
		  UINT8 MotorGetTempCmd[MOTOR_CMD_LENGTH];
		  UINT8 MotorPA[MOTOR_CMD_DATA_LENGTH];
		  UINT8 MotorTrack[MOTOR_CMD_DATA_LENGTH];
		  INT32 data_val=1;
	      static INT32 data5=0, data10, data15;
	      INT32 delta10, delta15;	
	
         BuildServoDataCmd(MotorMotorOnCmd, SERVO_MO_CMD, 0, SERVO_INT, &data_val, SERVO_TO_DRIVER, SERVO_SET); 
        // BuildServoDataCmd(MotorBeginMotionCmd, SERVO_BG_CMD, 0, SERVO_INT, &data_val, SERVO_TO_DRIVER, SERVO_SET);
         BuildServoCmd(MotorGetPxCmd, SERVO_PX_CMD, 0, SERVO_GET,0);
        // BuildServoCmd(MotorGetStatusCmd, SERVO_STATUS_CMD, 0, SERVO_GET,0);
        // BuildServoCmd(MotorGetTempCmd, SERVO_TI_CMD, 1, SERVO_GET, SERVO_FLT);
			//YYY=InsInfo->Yaw;
			YYY=ant_angles->A[0];
			if (YYY<0)
				YYY+=360;
			data_val=(UINT32)(YYY*TICS_PER_DEGREE);

//	        data5 = (INT32)((YYY+InsInfo->zRate/200.0)*TICS_PER_DEGREE/100.0);
//	        data10 = (INT32)((YYY+InsInfo->zRate/100.0)*TICS_PER_DEGREE/100.0);
//	        data15 = (INT32)((YYY+InsInfo->zRate*15.0/1000.0)*TICS_PER_DEGREE/100.0);
	
//	        delta15 = data15 - data10;
//	        delta10 = data10 - data5;
	

	
/*	        if(delta10 > SERVO_3PNT_DELTA_MAX_VAL)
	            delta10 = SERVO_3PNT_DELTA_MAX_VAL;
	        if(delta15 > SERVO_3PNT_DELTA_MAX_VAL)
	            delta15 = SERVO_3PNT_DELTA_MAX_VAL;
	        if(delta10 < -SERVO_3PNT_DELTA_MAX_VAL)
	            delta10 = -SERVO_3PNT_DELTA_MAX_VAL;
	        if(delta15 < -SERVO_3PNT_DELTA_MAX_VAL)
	            delta15 = -SERVO_3PNT_DELTA_MAX_VAL;	*/		


			//data_val = (data5 & 0x0003FFFF )<<(2*SERVO_3PNT_DELTA_BITS) | ( delta10 & 0x0000007F)<<SERVO_3PNT_DELTA_BITS | (delta15 & 0x0000007F) ;
			
			BuildServoDataCmd(MotorPA, SERVO_PA_CMD, 0, SERVO_INT, &data_val, SERVO_TO_DRIVER, SERVO_SET);
			//BuildServoDataCmd(MotorTrack, SERVO_3PNT_MOVE_CMD, 0, SERVO_INT, &data_val, SERVO_TO_DRIVER, SERVO_SET);


			//UINT8 az[100];
			
			//UINT8* write_ptr = az;
			
		    //memClear(write_ptr,100);
			
			/*memCopy(write_ptr, MotorMotorOnCmd, MOTOR_CMD_DATA_LENGTH);
			write_ptr += MOTOR_CMD_DATA_LENGTH;
			memCopy(write_ptr, MotorPA, MOTOR_CMD_DATA_LENGTH);
			//memCopy(write_ptr, MotorTrack, MOTOR_CMD_DATA_LENGTH);
			write_ptr += MOTOR_CMD_DATA_LENGTH;
			memCopy(write_ptr, MotorBeginMotionCmd, MOTOR_CMD_DATA_LENGTH);
			write_ptr += MOTOR_CMD_DATA_LENGTH;
			//rc=UartDrv_Tx (MOTOR_PORT, az, MOTOR_CMD_DATA_LENGTH);
            serial.Send(az,MOTOR_CMD_DATA_LENGTH);
 			memCopy(write_ptr, MotorGetPxCmd, MOTOR_CMD_DATA_LENGTH);
			write_ptr += MOTOR_CMD_DATA_LENGTH;*/
			//rc=UartDrv_Tx (MOTOR_PORT, az, MOTOR_CMD_DATA_LENGTH*4); 
            serial.Send(MotorMotorOnCmd,MOTOR_CMD_DATA_LENGTH);    
            serial.Send(MotorPA,MOTOR_CMD_DATA_LENGTH);
            serial.Send(MotorGetPxCmd,MOTOR_CMD_LENGTH);
            

	

	return(rc);
}

            

                

    




void RxMsghandler(void *cmd_p)
{
    UINT16 cmd_id;
    UINT32 cmd_data;
    INT32 cmd_i_data;
    UINT16 cmd_idx;
    FLOAT32 cmd_flt_data;
	MSG *msg;
	msg=(MSG*)cmd_p;
    servo_data_cmd_t *cmd_ptr = (servo_data_cmd_t *)cmd_p;

    cmd_id = ((INT16)cmd_ptr->cmd_id_1 << 8 ) | (UINT16)cmd_ptr->cmd_id_2;
    // returning commands always contain data
    cmd_data =  (UINT32)(cmd_ptr->data_1) | ((UINT32)(cmd_ptr->data_2)<<8) | 
               ((UINT32)(cmd_ptr->data_3) << 16) | ((UINT32)(cmd_ptr->data_4) << 24);

    cmd_flt_data = *(FLOAT32*)(void*)&cmd_data;

    cmd_idx = (cmd_ptr->idx_1) | ((cmd_ptr->idx_2)<<2);

    // Check that this is the answer to the custom command, before treating everything else
    if(SERVOCmd_ptr && cmd_id == ((SERVOCmdData*)SERVOCmd_ptr)->opcode) 
        {
	    


        // save the data recieved in the strtucture
        if(cmd_ptr->data_type == SERVO_INT) 
            {
            ((SERVOCmdData*)SERVOCmd_ptr)->int_res = *(INT32*)&cmd_data;
            }
        else
            {
            void *vptr = &cmd_data; // needed for warning removal
            ((SERVOCmdData*)SERVOCmd_ptr)->flt_res = *(FLOAT32*)vptr;
            }


            ((SERVOCmdData*)SERVOCmd_ptr)->err_res = cmd_ptr->error;
            // Nullify the pointer to command data, in order to save only the first recieved result
            SERVOCmd_ptr = NULL;
        }


    switch(cmd_id)
        {
		    case SERVO_3PNT_MOVE_CMD: // fall through
		    case SERVO_PX_CMD: // answer for PX request
		        cmd_i_data = *(INT32*)&cmd_data; // UINT to int
		        SetPx(cmd_i_data);
		        break;
		    case SERVO_STATUS_CMD:// answer for status request
		        status.stat = cmd_data;
		        // check status. if the motor failed, set appropriate flag and read MF reason.
		
		        break;
		    case SERVO_TI_CMD:// answer for temperature request
		        MotorTemp = (INT32)(100.0 * cmd_flt_data);
		        break;
		    case SERVO_MF_CMD:
		        MotorFailStatusWord.stat = cmd_data;
		        mf = cmd_data;
		        
		        break;
		    case SERVO_TW_CMD: // answer for homing/deicing status request
		        cmd_i_data = *(INT32*)&cmd_data; // UINT to int
		}
        switch(cmd_idx) 
           {
	        case ACU_HOME_TW_IDX: homing_status = (ACU_homing_stat_t)cmd_i_data; break;
	        case ACU_DEICE_TW_IDX: deicing_status = (ACU_deicing_stat_t)cmd_i_data; break;
	        case ACU_FW_CHKSM_IDX: 
	            
	            VerCheckSumUpdated = TRUE;
	
	            break;
	        case ACU_BL_CHKSM_IDX:
	
	            BootVerCheckSumUpdated = TRUE;
	 
	            break;
	        case ACU_BL_VER_IDX:
	
	            BootVerUpdated = TRUE;
	
	            break;

        	
        break;
    case SERVO_ACCL_CMD:
        accl = cmd_data;
        if(ConfigCheckStat & SERVO_CHECK_ACCL_BIT)
            {
            if(accl == ConfigAccl) 
                ConfigCheckStat &= ~SERVO_CHECK_ACCL_BIT;
            }
        break;
    case SERVO_DECCL_CMD:
        deccl = cmd_data;
        if(ConfigCheckStat & SERVO_CHECK_DECL_BIT)
            {
            if(cmd_data == ConfigDecl) 
                ConfigCheckStat &= ~SERVO_CHECK_DECL_BIT;
            }
        break;
    case SERVO_STOP_DECCL_CMD:
        if(ConfigCheckStat & SERVO_CHECK_STOP_DECL_BIT)
            {
            if(cmd_data == ConfigStopDecl) 
                ConfigCheckStat &= ~SERVO_CHECK_STOP_DECL_BIT;
            }
        break;
    case SERVO_KP_CMD:
        kp = (INT32)(cmd_flt_data * 1000.0);
        break;
    case SERVO_PM_CMD:
        pm = cmd_data;
        if(ConfigCheckStat & SERVO_CHECK_PM_BIT)
            {
            if(cmd_data == ConfigPM) 
                ConfigCheckStat &= ~SERVO_CHECK_PM_BIT;
            }
        break;
    case SERVO_SPP_CMD:
        spp = cmd_data;
        if(ConfigCheckStat & SERVO_CHECK_SPP_BIT)
            {
            if(cmd_data == ConfigSPP)
                ConfigCheckStat &= ~SERVO_CHECK_SPP_BIT;
            }
        break;
    case SERVO_UM_CMD:
        if(ConfigCheckStat & SERVO_CHECK_UM_BIT)
            {
            if(cmd_data == ConfigUm) 
                ConfigCheckStat &= ~SERVO_CHECK_UM_BIT;
            }
        break;
    case SERVO_VER_CMD:

        VerUpdated = TRUE;


        if(ConfigCheckStat & SERVO_CHECK_VER_BIT)
            {
            if(cmd_data >= ConfigVer) 
                ConfigCheckStat &= ~SERVO_CHECK_VER_BIT;
            }

        break;
    case SERVO_MO_CMD:
        mo = cmd_data;
        if(MotorFail == FALSE && mo == 0) 
           {
           ReadMotorFail = TRUE;
       MotorFail = mo ? FALSE : TRUE; 
			}
        break;
    case SERVO_AA_CMD:
        if( cmd_data == 0xAAAA5555)
            CommVerified = TRUE;
        break;
    case SERVO_XMOD_CMD:
        if((cmd_idx == SERVO_XMOD_LOW_IDX) && (ConfigCheckStat & SERVO_CHECK_XMOD_LOW_BIT)) 
            {
            if(cmd_data == ConfigXmodLow) 
                ConfigCheckStat &= ~SERVO_CHECK_XMOD_LOW_BIT;
            }
        else if((cmd_idx == SERVO_XMOD_HIGH_IDX) && (ConfigCheckStat & SERVO_CHECK_XMOD_HIGH_BIT)) 
            {
            if(cmd_data == ConfigXmodHigh)
                ConfigCheckStat &= ~SERVO_CHECK_XMOD_HIGH_BIT;
            }
        break;
    case SERVO_YMOD_CMD:
        if((cmd_idx == SERVO_YMOD_LOW_IDX) && (ConfigCheckStat & SERVO_CHECK_YMOD_LOW_BIT)) 
            {
            if(cmd_data == ConfigYmodLow) 
                ConfigCheckStat &= ~SERVO_CHECK_YMOD_LOW_BIT;
            }
        else if((cmd_idx == SERVO_YMOD_HIGH_IDX) && (ConfigCheckStat & SERVO_CHECK_YMOD_HIGH_BIT)) 
            {
            if(cmd_data == ConfigYmodHigh)
                ConfigCheckStat &= ~SERVO_CHECK_YMOD_HIGH_BIT;
            }
        break;
    case SERVO_TORQ_LIM_CMD:
        if((cmd_idx == SERVO_TORQ_LIM1_IDX) && (ConfigCheckStat & SERVO_CHECK_TORQ_LIM1_BIT)) 
            {
            if(cmd_flt_data == ConfigTL1) 
                ConfigCheckStat &= ~SERVO_CHECK_TORQ_LIM1_BIT;
           }
        else if((cmd_idx == SERVO_TORQ_LIM2_IDX) && (ConfigCheckStat & SERVO_CHECK_TORQ_LIM2_BIT)) 
            {
            if(cmd_flt_data == ConfigTL2)
                ConfigCheckStat &= ~SERVO_CHECK_TORQ_LIM2_BIT;
            }
        else if((cmd_idx == SERVO_TORQ_LIM3_IDX) && (ConfigCheckStat & SERVO_CHECK_TORQ_LIM3_BIT)) 
            {
            if(cmd_flt_data == ConfigTL3)
                ConfigCheckStat &= ~SERVO_CHECK_TORQ_LIM3_BIT;
            }
        break;
    case SERVO_PEAK_CMD:
        if((cmd_idx == SERVO_PEAK_DUR_IDX) && (ConfigCheckStat & SERVO_CHECK_PEAK_DUR_BIT)) 
            {
            if(cmd_flt_data >= ConfigPeakDurMin && cmd_flt_data <= ConfigPeakDurMax) 
                ConfigCheckStat &= ~SERVO_CHECK_PEAK_DUR_BIT;
            }
        else if((cmd_idx == SERVO_PEAK_LIM_IDX) && (ConfigCheckStat & SERVO_CHECK_PEAK_LIM_BIT)) 
            {
            if(cmd_flt_data >= ConfigPeakLimMin && cmd_flt_data <= ConfigPeakLimMax)
                ConfigCheckStat &= ~SERVO_CHECK_PEAK_LIM_BIT;
            }
        break;
    case SERVO_REF_MODE_CMD:
        if(ConfigCheckStat & SERVO_CHECK_REF_MODE_BIT) 
            {
            if(cmd_data == ConfigRefMode) 
                ConfigCheckStat &= ~SERVO_CHECK_REF_MODE_BIT;
           }
        break;
    case SERVO_UI_CMD:
        if((cmd_idx == SERVO_UI_ZERO_CAL_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_1_BIT)) 
            {
            ZeroCalRead = cmd_data;
            if(ZeroCalRead == ZeroCal) // verify the read value VS the value from the user config
                ConfigCheckStat &= ~SERVO_CHECK_UI_1_BIT;
			}
     	else if((cmd_idx == SERVO_UI_6_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_6_BIT)) 
            {
            if(cmd_data == ConfigUI_6) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_6_BIT;
            }
        else if((cmd_idx == SERVO_UI_7_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_7_BIT)) 
            {
            if(cmd_data == ConfigUI_7) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_7_BIT;
            }
        else if((cmd_idx == SERVO_UI_8_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_8_BIT)) 
            {
            if(cmd_data == ConfigUI_8) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_8_BIT;
            }
        else if((cmd_idx == SERVO_UI_9_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_9_BIT)) 
            {
            if(cmd_data == ConfigUI_9) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_9_BIT;

            }
        else if((cmd_idx == SERVO_UI_10_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_10_BIT)) 
            {
            if(cmd_data == ConfigUI_10) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_10_BIT;
           }
        else if((cmd_idx == SERVO_UI_15_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_15_BIT)) 
            {
            if(cmd_data == ConfigUI_15) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_15_BIT;
           }
        else if((cmd_idx == SERVO_UI_16_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_16_BIT)) 
            {
            if(cmd_data == ConfigUI_16) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_16_BIT;
            }
        else if((cmd_idx == SERVO_UI_17_IDX) && (ConfigCheckStat & SERVO_CHECK_UI_17_BIT)) 
            {
            if(cmd_data == ConfigUI_17) // only check here that it is not zero. TODO -> add the calibrations values to configuration
                ConfigCheckStat &= ~SERVO_CHECK_UI_17_BIT;
            }
        else if((cmd_idx == SERVO_UI_CAL_VER_IDX) && (ConfigCheckStat & SERVO_CHECK_CAL_VER_BIT)) 
            {
            if(cmd_data == ConfigUI_CalVer) 
                ConfigCheckStat &= ~SERVO_CHECK_CAL_VER_BIT;


            }
        else if(cmd_idx == SERVO_UI_ENC_SN_IDX) 
            {

            EncoderSNUpdated = TRUE;
            }
        else if(cmd_idx == SERVO_UI_SYSTEM_SN_IDX) 
            {

            SystemSNUpdated = TRUE;

            }
        break;
    case SERVO_INFO_CMD:
        if(cmd_idx == SERVO_INFO_REV_IDX) 
            {

            DriverRevUpdated = TRUE;
            }
        else if(cmd_idx == SERVO_INFO_SN_IDX) 
            {
;
            DriverSNUpdated = TRUE;
            }
        break;
	 case SERVO_CP_CMD:
        if((cmd_idx == SERVO_CP_PROTECT_IDX) && (ConfigCheckStat & SERVO_CHECK_CP_17_BIT))
	        {
           if(cmd_data == ConfigCP_17) 
               ConfigCheckStat &= ~SERVO_CHECK_CP_17_BIT;

               
	        }

	    
    default:
        break;

        }

}

void SetPx(INT32 new_px)
{
    //Store PX
    px = new_px;
    // Calculate real angle
    px_deg = (double)(px)/TICS_PER_DEGREE; 


// the value returned from the motors is 0 to 360 degrees. normalize the px to -180 to 180
    if(px_deg > 180.0) 
        px_deg -= 360.0;

// Calculate pointing error

    px_deg_err = curr_deg - px_deg;
    px_est_deg_err = pred_est_deg - px_deg;

    if(px_deg_err > 180.0)
        {
        px_deg_err = 360.0 - px_deg_err;
        }
    else if(px_deg_err < -180.0)
        {
        px_deg_err = 360.0 + px_deg_err;
        }

    if(px_est_deg_err > 180.0)
        {
        px_est_deg_err = 360.0 - px_est_deg_err;
        }
    else if(px_est_deg_err < -180.0)
        {
        px_est_deg_err = 360.0 + px_est_deg_err;
        }

    // Update integrator
    MotorErrAccumulator += (px_deg_err - MotorErrHist[MotorErrIdx]);
    if (fabsf(MotorErrAccumulator) > MOTOR_ERROR_HISTORY_SIZE * allowed_err)
        {
        isNIP = TRUE;
        }
    else 
        {
        isNIP = FALSE;
        }

    // Update the history buffer with the new error
    MotorErrHist[MotorErrIdx++] = px_deg_err;
    if(MotorErrIdx == MOTOR_ERROR_HISTORY_SIZE) 
        MotorErrIdx = 0;
}


/*********************************************************************************************************
* Name       : float cMotorInfo::GetPxErr(void)
* Description: returns the error between the needed location and the current location 
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
double GetPxErr(void)
{
    return px_deg_err;
}
/*********************************************************************************************************
* Name       : float cMotorInfo::GetEstPxErr(void)
* Description: returns the error between the estimated needed location and the current location. measures if the motor actually got to the estimated point.
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
double GetEstPxErr(void)
{
    return px_est_deg_err;
}

/*********************************************************************************************************
* Name       : BOOL cMotorInfo::GetNIP(void)
* Description: returns TRUE is a NIP condition occurs
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
BOOL GetNIP(void)
{
    return isNIP;
}
/*********************************************************************************************************
* Name       : BOOL cMotorInfo::GetMF(void)
* Description: returns TRUE is Motor Fail has occured
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
BOOL GetMF(void)
{
    return MotorFail;
}
/*********************************************************************************************************
* Name       : float cMotorInfo::GetPx ()
* Description: Returns the current angle of the Motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
double GetPx(void)
{
    return px_deg;
}


/*********************************************************************************************************
* Name       : UINT32 cMotorInfo::GetPX_forApp(void)
* Description: Returns the current angle of the Motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
UINT32 GetPX_forApp(void)
{
    // todo, add good treatment for negative azimuth angles
    return (UINT32)(px_deg * 1024.0 * 1024.0 / 360.0 ); 
}


/*********************************************************************************************************
* Name       : void cMotorInfo::SetStatus(UINT32 new_status)
* Description: Sets the current status of the motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
void SetStatus(UINT32 new_status)
{
    status.stat = new_status;
}

/*********************************************************************************************************
* Name       : MotorStatus_t cMotorInfo::GetStatus(void)
* Description: Gets the current status of the Motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
MotorStatus_t GetStatus(void)
{
    return status;
}

/*********************************************************************************************************
* Name       : MotorStatus_t cMotorInfo::GetTemp(void)
* Description: Gets the current temperature of the Motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
INT32 GetTemp(void)
{
    return MotorTemp;
}


/*********************************************************************************************************
* Name       : void cMotorInfo::SetHomingStatus(INT32 new_status)
* Description: Sets the homing status
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
void SetHomingStatus(INT32 new_status)
{
    homing_status = (ACU_homing_stat_t)new_status;
}
/*********************************************************************************************************
* Name       : ACU_homing_stat_t cMotorInfo::GetHomingStatus(void)
* Description: Gets the homing status
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
ACU_homing_stat_t GetHomingStatus(void)
{
    return homing_status;
}
/*********************************************************************************************************
* Name       : void cMotorInfo::SetDeicingStatus(INT32 new_status)
* Description: Sets the deicing status
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
void SetDeicingStatus(INT32 new_status)
{
    deicing_status = (ACU_deicing_stat_t)new_status;
}
/*********************************************************************************************************
* Name       : ACU_deicing_stat_t cMotorInfo::GetDeicingStatus(void)
* Description: Gets the deicing status
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
ACU_deicing_stat_t GetDeicingStatus(void)
{
    return deicing_status;
}
/*********************************************************************************************************
* Name       : void cMotorInfo::SetEstimation(double current_deg, double est_deg_5ms)
* Description: Sets the target and the target estimated position of the Motor
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
void SetEstimation(double current_deg, double est_deg_5ms)
{
    // save the previous estimated location, to compare with the current
    pred_est_deg = est_deg;

    est_deg = est_deg_5ms;
    curr_deg = current_deg;
}





RC memCopy(void *dst, const void *src, UINT32 size)
{
   int  i;
   BYTE* Src = (BYTE *)src;
   BYTE* Dst = (BYTE *)dst;


   if (dst == src)
      return(OK);

   if (dst < src)
      {
      for (i = 0; i < size;)
         {
         // if not aligned to 4 bytes - copy one byte
         if ((((UINT32)Src) & 0x0003) || (((UINT32)Dst) & 0x0003) || ((size - i) < 4))
            {
            Dst[0] = Src[0];

            Src++;
            Dst++;
            i++;
            }
         else // if aligned to 4 bytes
            {
            ((UINT32 *)Dst)[0] = ((UINT32*)Src)[0];

            Src += 4;
            Dst += 4;
            i += 4;
            }
         }
      }
   else // (src > dst)
      {
      Src += size;
      Dst += size;

      for (i = 0; i < size;)
         {
         // if  not aligned to 4 bytes - copy one byte
         if ((((UINT32)Src) & 0x0003) || (((UINT32)Dst) & 0x0003) || ((size - i) < 4))
            {
            Src--;
            Dst--;
            i++;

            Dst[0] = Src[0];
            }
         else // if aligned to 4 bytes - copy 4 bytes
            {
            Src -= 4;
            Dst -= 4;
            i += 4;

            ((UINT32 *)Dst)[0] = ((UINT32*)Src)[0];
            }
         }
      }


   return(OK);
}



RC memClear(void *MemPtr, UINT32 size)
{
   BYTE *bP;
   UINT32 *iP;
   UINT32 i = 0;
   UINT32 no = 0, sum = 0;

   if (!MemPtr)
      return(rcMemParamErr);

   // if small amount of bus access are needed
   if (size < 3)
      {
      bP = (BYTE *)MemPtr;
      for (i = 0; i < size; i++)
         bP[i] = 0;
      return(OK);
      }

   // if address not aligned
   else if ((UINT32)MemPtr & 0x3)
      {
      no = (UINT32)MemPtr % sizeof(UINT32);
      bP = (BYTE *)MemPtr;
      for (i = 0; i < no; i++)
         bP[i] = 0;
      MemPtr = &(bP[i]);
      }
   sum += i;

   // clear ...
   no = (size - no) / sizeof (UINT32);
   iP = (UINT32 *)MemPtr;
   for (i = 0; i < no; i++)
      iP[i] = 0;
   MemPtr = &(iP[i]);
   sum += i * sizeof(UINT32);

   // if size is not in the size of bus access
   bP = (BYTE *)MemPtr;
   for (i = 0; (i + sum) < size; i++)
      bP[i] = 0;

   return(OK);
}



static const UINT16 servo_crc16_table[16] = {
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 
};

INT16 calc_servo_crc16(UINT16 crc, UINT8 const *buffer, UINT16 len)
{
    while(len--) 
        {
        // CRC the lower 4 bits
        crc = (crc >> 4) ^ servo_crc16_table[((crc ^ (*buffer & 0xF)) & 0xF)];
        // CRC the upper 4 bits
        crc = (crc >> 4) ^ servo_crc16_table[((crc ^ (*buffer >> 4)) & 0xF)];
        // Move on to the next element
        buffer++;
        }
// Return the cumulative CRC-16 value
    return ((crc & 0xFF00)>>8 | (crc & 0x00FF)<<8 );
}

