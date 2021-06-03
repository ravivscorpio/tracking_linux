#ifndef __TRACKING__H__
#define __TRACKING__H__


#include "defs.h"
#include "defs_p.h"
#include "rc.h"
#include "Comm.h"
#include "matrix.h"
#include "serial.h"
#include "motorInfo.h"
#include "Utils.h"
#include "aim.h"
#include "Comm.h"
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>



#define TIME		10000 //10sec
#define PROCESS_CYCLE 5
#define MAX_ELAPSED_TIME 1000
#define MAX_TOTAL_PERIOD 600000
#define MAX_timer 2000
#define ANGLE_SCALE 0.01
#define TIME_SCALE 0.01
#define SEC_MILI 1000
#define MILISEC 0.001

#define CLOCKS_PER_SEC1 CLOCKS_PER_SEC/3.8
 
/***** Typedefs ********************************************************/

typedef enum{
	MODE_PNT,
	MODE_TRK,
	MODE_SIN,
	MODE_RSSI
	
}SYSTEM_MODE;

void Tracking_init();
void* threadRcv(void* args);
void* threadSend(void* args);
void* thread_SGP_latlon(void* args);
RC  Pred_Angle_Data(VEC *InsInfo,VEC* delta_t);
RC Set_motor_params(SYSTEM_MODE system_mode,INT16 amplitude,INT16 period,INT16 rep,INT16 total_period);
#endif

