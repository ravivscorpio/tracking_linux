#ifndef __MotorInfo__H__
#define __MotorInfo__H__


#include "defs.h"
#include "defs_p.h"
#include "rc.h"
#include "Comm.h"
#include "matrix.h"
#include "serial.h"
//#include "UartDrv.h"

//#define MOTOR_PORT UART_PORT1
#define Motor_PRE_0 0x8B
#define Motor_PRE_1 0x3C
#define Max_RxMotorMsg 11


#define rcMemErr              (rcErr | rcMemMgrBase | 0x01)
#define rcMemParamErr         (rcErr | rcMemMgrBase | 0x02)



#define TICS_PER_DEGREE 136 // 49,152/360

#define MOTOR_ERROR_HISTORY_SIZE	10	// 50 ms

#define HOMING_TIMEOUT 		60.0
#define DEICING_TIMEOUT 	180.0
#define DEICING_TEMPERATURE_THRESH ((INT32)0)
#define RESET_TIMEOUT		1.0

#define MOTOR_CMD_DATA_LENGTH	11
#define MOTOR_CMD_LENGTH		7

#define SERVO_3PNT_BITS          32   
#define SERVO_3PNT_DELTA_BITS    7
#define SERVO_3PNT_POS_BITS      18

#define SERVO_CHECK_ACCL_BIT			0x00000001
#define SERVO_CHECK_DECL_BIT			0x00000002
#define SERVO_CHECK_STOP_DECL_BIT		0x00000004
#define SERVO_CHECK_SPP_BIT 			0x00000008
#define SERVO_CHECK_UM_BIT				0x00000010
#define SERVO_CHECK_VER_BIT				0x00000020
#define SERVO_CHECK_PM_BIT				0x00000040
#define SERVO_CHECK_CP_17_BIT			0x00000080
#define SERVO_CHECK_HRL_2_BIT			0x00000100
#define SERVO_CHECK_LRL_1_BIT			0x00000200
#define SERVO_CHECK_LRL_2_BIT			0x00000400
#define SERVO_CHECK_XMOD_LOW_BIT		0x00000800
#define SERVO_CHECK_XMOD_HIGH_BIT		0x00001000
#define SERVO_CHECK_YMOD_LOW_BIT		0x00002000
#define SERVO_CHECK_YMOD_HIGH_BIT		0x00004000
#define SERVO_CHECK_TORQ_LIM1_BIT		0x00008000
#define SERVO_CHECK_TORQ_LIM2_BIT		0x00010000
#define SERVO_CHECK_TORQ_LIM3_BIT		0x00020000
#define SERVO_CHECK_PEAK_DUR_BIT		0x00040000
#define SERVO_CHECK_PEAK_LIM_BIT		0x00080000
#define SERVO_CHECK_REF_MODE_BIT		0x00100000
#define SERVO_CHECK_ECHO_BIT			0x00200000
#define SERVO_CHECK_UI_1_BIT			0x00400000
#define SERVO_CHECK_UI_6_BIT			0x00800000
#define SERVO_CHECK_UI_7_BIT			0x01000000
#define SERVO_CHECK_UI_8_BIT			0x02000000
#define SERVO_CHECK_UI_9_BIT			0x04000000
#define SERVO_CHECK_UI_10_BIT			0x08000000
#define SERVO_CHECK_UI_15_BIT			0x10000000
#define SERVO_CHECK_UI_16_BIT			0x20000000
#define SERVO_CHECK_UI_17_BIT			0x40000000
#define SERVO_CHECK_CAL_VER_BIT			0x80000000

#define SERVO_CHECK_ALL_FAIL			0xFFDFF8FF
//#define SERVO_CHECK_ALL_FAIL			0x7FFFFFFF

#define SERVO_CHECK_ALL_PASS			0x00000000

/***** Typedefs ********************************************************/
typedef struct ACU_test_bit{ // UINT16
// azimuth/elevation sweeps -> if set, bypass the pointing algorith and arinc
   UINT8 el_sweep : 1; 
   UINT8 az_sweep : 1;

// bypass_pointing -> if set, bypass the pointing algorithm ->
   UINT8 bypass_pointing_YAW_TO_AZZ : 1;
   UINT8 bypass_pointing_PITCH_TO_EL : 1;
   
// test pointing algorith by generating 
   UINT8 bypass_ARINC_YAW_SWEEP : 1;
   UINT8 bypass_ARINC_PITCH_SWEEP : 1;

   UINT8 use_MIJ : 1;
   UINT8 unused :1;

} ACU_test_bit_t;

typedef struct
   {

   UINT16  opcode;
   UINT8   index;
   INT32   int_data;
   FLOAT32 flt_data;
   UINT8 int_flt_sel;
   UINT8 drv_sel;
   void    *data_ptr;
   UINT8   cmd_type;

   INT32 int_res;
   FLOAT32  flt_res;
   UINT8   err_res;

   } SERVOCmdData;

typedef enum cmd_id {
   SERVO_ACCL_CMD =                200,
   SERVO_DECCL_CMD =               201,
   SERVO_PA_CMD =                  205,
   SERVO_STOP_DECCL_CMD =          207,
   SERVO_SPP_CMD =                 208,
   SERVO_3PNT_MOVE_CMD =           210,
   SERVO_BG_CMD =                  238,
   SERVO_MO_CMD =                  284,
   SERVO_UM_CMD =                  289,
   SERVO_MF_CMD =                  2216,
   SERVO_INFO_CMD =                2219,
   SERVO_STATUS_CMD =              2223,
   SERVO_TI_CMD =                  2225,
   SERVO_VER_CMD =                 2230,
   SERVO_PX_CMD =                  3200,
   SERVO_AA_CMD =                  3208,
   SERVO_PM_CMD =                  4203,
   SERVO_HRL_CMD =                 4206, // High Refference Limit
   SERVO_LRL_CMD =                 4209, // Low Refference Limit
   SERVO_XMOD_CMD =                4212,
   SERVO_YMOD_CMD =                4214,
   SERVO_KP_CMD =                  4224,
   SERVO_CP_CMD =                  4227,
	SERVO_TORQ_LIM_CMD =            4228,
   SERVO_PEAK_CMD =                4229,
   SERVO_REF_MODE_CMD =            4230,
   SERVO_TP_CMD =                  4234, // tp[6] = 1
   SERVO_ECHO_CMD =                4236,
   SERVO_TW_CMD =                  4237,
   SERVO_UI_CMD =                  4243,
   SERVO_RST_CMD =                 4244

} cmd_id_t;


typedef enum
{
   SERVO_HRL_1_IDX = 1,
   SERVO_HRL_2_IDX = 2,
   SERVO_LRL_1_IDX = 1,
   SERVO_LRL_2_IDX = 2,

   SERVO_XMOD_LOW_IDX = 1,
   SERVO_XMOD_HIGH_IDX = 2,

   SERVO_YMOD_LOW_IDX = 1,
   SERVO_YMOD_HIGH_IDX = 2,

   SERVO_TORQ_LIM1_IDX = 1,
   SERVO_TORQ_LIM2_IDX = 2,
   SERVO_TORQ_LIM3_IDX = 3,

   SERVO_PEAK_DUR_IDX = 1,
   SERVO_PEAK_LIM_IDX = 2,

   SERVO_INFO_REV_IDX = 3,
   SERVO_INFO_SN_IDX = 4,

   SERVO_UI_ZERO_CAL_IDX = 1,
   SERVO_UI_6_IDX = 6,
   SERVO_UI_7_IDX = 7,
   SERVO_UI_8_IDX = 8,
   SERVO_UI_9_IDX = 9,
   SERVO_UI_10_IDX = 10,
   SERVO_UI_15_IDX = 15,
   SERVO_UI_16_IDX = 16,
   SERVO_UI_17_IDX = 17,
   SERVO_UI_ENC_SN_IDX = 22,
   SERVO_UI_SYSTEM_SN_IDX = 23,
   SERVO_UI_CAL_VER_IDX = 4,
	
   SERVO_CP_PROTECT_IDX = 17

} cmd_idx_t;

typedef enum{
	ACU_HOME_TW_IDX	=	27,
	ACU_DEICE_TW_IDX =	30,
	ACU_FW_CHKSM_IDX =	31,
	ACU_BL_CHKSM_IDX =	32,
	ACU_BL_VER_IDX =	33
}TW_index_t;

typedef enum{
	ACU_HOMING_STATE_NO_CONNECTION = -100,
	ACU_HOMING_STATE_ERR_TIMEOUT = -3, // fatal 
	ACU_HOMING_STATE_ERR_LIMITED_MOTION = -2, // deicing is needed
	ACU_HOMING_STATE_ERR_INDEX = -1, // need to retry
	ACU_HOMING_STATE_IN_PROCESS = 1,
	ACU_HOMING_STATE_IN_PROCESS_FIRST_INDEX = 2,
	ACU_HOMING_STATE_HOMED = 3 		// homing sucessfull 
}ACU_homing_stat_t;

typedef enum{
	ACU_DEICING_NOT_RUN = -1000,
	ACU_DEICING_STATE_NO_CONNECTION = -100,

	ACU_DEICING_STATE_ERR_TIMEOUT = -3,
	ACU_DEICING_STATE_ERR_MOTOR_FAIL = -2,
	ACU_DEICING_STATE_ERR_FAIL = -1,

	ACU_DEICING_STATE_IN_PROCESS_ELEVATION = 1,
	ACU_DEICING_STATE_IN_PROCESS_AZIMUTH = 2,

	ACU_DEICING_STATE_SUCCESS = 3 

}ACU_deicing_stat_t;


typedef union MotorStatus
    {
	UINT32 stat;
	struct MotorStatusBits
		{
		UINT32 unused_3 : 4;
		UINT32 CPU_stat : 1;
		UINT32 hall_c : 1;
		UINT32 hall_b : 1;
		UINT32 hall_a : 1;

		UINT32 unused_22: 8;

		UINT32 unused_21 : 2;
		UINT32 LC : 1;
		UINT32 unused_1 : 3;
		UINT32 UM_2 : 2;

		UINT32 UM_1 : 1;
		UINT32 MF : 1;
		UINT32 RM : 1;
		UINT32 motor_on : 1;
		UINT32 drive_status_detail : 3;
		UINT32 drive_read : 1;

	    } bits;
    }MotorStatus_t;

typedef union MotorFailStatus
	{
	UINT32 stat;
	struct MotorFailStatusBits
		{

		UINT32 unused_4 : 10;
		UINT32 motor_stuck : 1;
		UINT32 unused_3 : 1;
		UINT32 cpu_exception : 1;
		UINT32 unused_2 : 3;
		UINT32 fault_detail_bits : 3;
		UINT32 unused_1 : 12;
		UINT32 halls_feedback_not_ready : 1;

		} bits;

	}MotorFailStatus_t;

typedef enum{
	SERVO_RESET,
	SERVO_HOME,
	SERVO_HOME_STATUS,
	SERVO_DEICE,
	SERVO_DEICE_STATUS,
	SERVO_STATUS,
	SERVO_MOVE,
	SERVO_STOP,
	SERVO_GET_STATE,
	SERVO_GET_TEMP,
	SERVO_SET_HOME,
	SERVO_CUSTOM,
	SERVO_ACCL,
	SERVO_DECCL,
	SERVO_PA,
	SERVO_KP,
	SERVO_PM,
	SERVO_SPP,
	SERVO_MO,
	SERVO_MF,
	SERVO_AA,
	SERVO_CAL_CHECK1,
	SERVO_CAL_CHECK2,
	SERVO_CAL_CHECK3,
	SERVO_CAL_CHECK4,
	SERVO_ENABLE_ENC_PROTECT
} ACU_SERVO_cmd_t;






typedef struct servo_data_cmd{

    UINT8 magic_1;
    UINT8 magic_2;

    UINT8 cmd_id_2;
    UINT8 cmd_id_1 :6;
    UINT8 idx_1 : 2;

    UINT8 idx_2: 4;
    UINT8 cmd_type : 1;
    UINT8 data_type : 1;
    UINT8 error : 1;
    UINT8 command_dir : 1;





    UINT8 data_1;
    UINT8 data_2;
    UINT8 data_3;
    UINT8 data_4;

    UINT8 crc_16_1;
    UINT8 crc_16_2;
} servo_data_cmd_t;

typedef struct servo_cmd{
    UINT8 magic_1;
    UINT8 magic_2;

    UINT8 cmd_id_2;

    UINT8 cmd_id_1 :6;
    UINT8 idx_1 : 2;
    
    UINT8 idx_2: 4;
    UINT8 cmd_type : 1;
    UINT8 data_type : 1;
    UINT8 error : 1;
    UINT8 command_dir : 1;

    UINT8 crc_16_1;
    UINT8 crc_16_2;
} servo_cmd_t;


typedef enum servo_cmd_type{
    SERVO_SET = 0,
    SERVO_GET = 1
} servo_cmd_type_t;

typedef enum servo_cmd_dir{
    SERVO_TO_DRIVER = 0,
    SERVO_FROM_DRIVER = 1
} servo_cmd_dir_t;

typedef enum servo_cmd_data_type{
    SERVO_INT = 0,
    SERVO_FLT = 1
}servo_cmd_data_type_t;

typedef enum servo_cmd_error{
    SERVO_NO_ERROR = 0,
    SERVO_ERROR = 1
} servo_cmd_error_t;

RC MotorDrvRx (MSG **msg);
void RxMsghandler(void *cmd_p);
UINT8* BuildServoCmd(UINT8* ptr, UINT16 cmd_id, UINT8 idx, UINT8 type, UINT8 data_type);
UINT8* BuildServoDataCmd(UINT8* ptr, UINT16 cmd_id, UINT8 idx, UINT8 data_type, void* data_ptr, UINT8 cmd_dir, UINT8 cmd_type);
RC memClear(void *MemPtr, UINT32 size);
RC memCopy(void *dst, const void *src, UINT32 size);
INT16 calc_servo_crc16(UINT16 crc, UINT8 const *buffer, UINT16 len);
void SetPx(INT32 new_px);
double GetPx(void);
RC SendMotorData(VEC *ant_angles);
#endif

