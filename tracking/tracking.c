
/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : MotorInfo.c
* AUTHORS       : 
* CREATE TIME   : 
************************************************************************/

/***** Include files ***************************************************/

#include "tracking.h"

using namespace std;







MAT DCM,DCM_fix;
VEC Euler,Vned,Vant,ant_angles;
//extern VEC t_midg;
extern struct timeval t_midg;
extern bool midg_msg;
//VEC t_ins,t_gps;
long t_ins_sec,t_ins_usec,t_gps;
UINT32 t0;
SYSTEM_MODE system_mode=MODE_TRK;
INT16 sin_amplitude=9000;//deg*100
INT16 sin_period=300;//sec*100
INT16 sin_repetitions=1;
INT16 total_period=3;//sec
INT16 point_angle=0;

struct timeval zulu_time;

VEC motor_angles;

#define PI 3.1415926535897932384626433832795

MSG msg_data;
MSG* msg=NULL;


void Tracking_init()
{
    RC rc;

    /* thread vars declerations*/
    int rcc_sgp_latlon,rcc_motor_tx,rcc_motor_rx;
    pthread_mutex_t myMutex;
    pthread_attr_t attr_sgp_latlon,attr_motor_tx,attr_motor_rx;
    struct sched_param param_sgp_latlon,param_motor_tx,param_motor_rx;
    pthread_t id_sgp_latlon,id_motor_tx,id_motor_rx;
    int ret_sgp_latlon,ret_motor_tx,ret_motor_rx;
    int max_prio = sched_get_priority_max(SCHED_RR);

    //pthread_mutex_init(&myMutex,0);
    
    gettimeofday(&zulu_time,NULL);
    rc=aim_init(&DCM_fix,&DCM,&ant_angles,&Vned,&Vant);
  
    
    /* SGP4 thread*/
    rcc_sgp_latlon = pthread_attr_init (&attr_sgp_latlon);
    rcc_sgp_latlon = pthread_attr_setschedpolicy(&attr_sgp_latlon,SCHED_RR);
    rcc_sgp_latlon = pthread_attr_getschedparam (&attr_sgp_latlon, &param_sgp_latlon);
    (param_sgp_latlon.sched_priority)=1;
    rcc_sgp_latlon = pthread_attr_setschedparam (&attr_sgp_latlon, &param_sgp_latlon);

    /*Motor TX Thread*/
    rcc_motor_tx = pthread_attr_init (&attr_motor_tx);
    rcc_motor_tx = pthread_attr_setschedpolicy(&attr_motor_tx,SCHED_RR);
    rcc_motor_tx = pthread_attr_getschedparam (&attr_motor_tx, &param_motor_tx);
    (param_motor_tx.sched_priority)= max_prio;
    rcc_motor_tx = pthread_attr_setschedparam (&attr_motor_tx, &param_motor_tx);
    /*Motor RX Thread*/
    rcc_motor_rx = pthread_attr_init (&attr_motor_rx);
    rcc_motor_rx = pthread_attr_setschedpolicy(&attr_motor_rx,SCHED_RR);
    rcc_motor_rx = pthread_attr_getschedparam (&attr_motor_rx, &param_motor_rx);
    (param_motor_rx.sched_priority)=max_prio;
    rcc_motor_rx = pthread_attr_setschedparam (&attr_motor_rx, &param_motor_rx);

    //ret_motor_rx=pthread_create(&id_motor_rx,&attr_motor_rx,&threadRcv, NULL);
    //ret_motor_tx=pthread_create(&id_motor_tx,&attr_motor_tx,&threadSend, NULL);
    //ret_sgp_latlon=pthread_create(&id_sgp_latlon,&attr_sgp_latlon,&thread_SGP_latlon, NULL);

    
}



void* thread_SGP_latlon(void* args)
{
    BYTE  azimuth_angle=0;
    time_t startTime = clock();
    time_t check = 0;
    RC rc;
    INT16 Ts=1000;//ms
    char satname[20];
    VEC xyz_sgp,lla1,xyz_station,lla_station;
    lla_station.A[0]=32;//lat
    lla_station.A[1]=35;//long
    lla_station.A[2]=0;//alt

    

    while(1) //program main loop
    {
	
        check = float(clock() - startTime)/CLOCKS_PER_SEC1*1000;
       
        if( check >= Ts) //after 1sec
        {
            Get_SatLatLong(&xyz_sgp,satname);
            rc=ecef_to_lla(&xyz_sgp,&lla1);
            std::cout<<"SATNAME= "<<satname<<" lat: "<<lla1.A[0]/pi*180<<" lon: "<<lla1.A[1]/pi*180<<" alt: "<<lla1.A[2]<<std::endl;
            //std::cout<<"lat: "<<xyz_sgp.A[0]/pi*180<<" lon: "<<xyz_sgp.A[1]/pi*180<<" alt: "<<xyz_sgp.A[2]<<std::endl;
            rc=lla_to_ned(&Vned ,&xyz_sgp,&lla_station);
            rc=update_angles(&DCM_fix,&DCM,&ant_angles,&Vned,&Vant);
            startTime = clock();
        }
    }
}



void* threadRcv(void* args)
{
    int bytelen;
    RC rc=OK;
    struct timeval sssR;
    while(1)
    {
        
        MotorDrvRx(&msg);
        if (msg)
            if (msg->Header.OpCode == 0x11){
                gettimeofday(&sssR,NULL); 
                RxMsghandler(msg->Data);
                //std::cout<<clock()<<": Motor Info: "<<px_deg<<endl;
                cout<<"az received: "<<(sssR.tv_sec-zulu_time.tv_sec)*1000000+(sssR.tv_usec-zulu_time.tv_usec)<<" "<<float( GetPx())<<endl;
                rc=Comm_BuffFree(msg,TRUE);
            
        }
        
        
        usleep(5000);
    }
}


void* threadSend(void* args)
{
    
    BYTE  azimuth_angle=90;
    time_t startTime = clock(),startTime1 = clock();
    time_t check,check_t_Period = 0;
    unsigned int kkk,n;
    RC rc;
    INT16 Ts=5;//ms
    long  dt,dT;
    VEC delta_t;

    struct timeval sss1,sss2,sssT;
    gettimeofday(&sss1,NULL);
    gettimeofday(&sssT,NULL);
    while(1) //program main loop
    {
	    
        check = double(clock() - startTime)/CLOCKS_PER_SEC1*1000;
        check_t_Period = double(clock() - startTime1)/CLOCKS_PER_SEC1*1000;
        //clock_measure();
        gettimeofday(&sss2,NULL);
        dt=(sss2.tv_sec-sss1.tv_sec)*1000000+sss2.tv_usec-sss1.tv_usec;
        dT=(sss2.tv_sec-sssT.tv_sec)*1000000+sss2.tv_usec-sss1.tv_usec;
        if (dt>5000)//if( check >= Ts) //after 5 milliseconds have elapsed
        {
            gettimeofday(&sss1,NULL); 
            startTime = clock();
            //motor_angles.A[0]=(double)azimuth_angle*sin(2*PI*(1/T)*(Ts/1000.0)*n);
            switch (system_mode)
            {
                case MODE_PNT:
                        motor_angles.A[0]=(FLOAT32)sin_amplitude;
                        rc=SendMotorData(&motor_angles);
                        
                        break;
                case MODE_TRK:
                   /* t_ins.A[0]=float(t_midg.A[0])/CLOCKS_PER_SEC1;//ms
                    t_ins.A[1]=float(t_midg.A[1])/CLOCKS_PER_SEC1;
                    t_ins.A[2]=float(t_midg.A[2])/CLOCKS_PER_SEC1;*/
                    //t_ins.A[0]=t_midg.A[0];
                    //t_ins.A[1]=t_midg.A[1];
                    //t_ins.A[2]=t_midg.A[2];

                    //t_ins=vec_add_scalar(t_ins,-float(clock())/CLOCKS_PER_SEC1,MAX_ELAPSED_TIME*MILISEC);
                    //t_ins=vec_add_scalar(t_ins,-(sss1.tv_sec+double(sss1.tv_usec)/1000000.0),MAX_ELAPSED_TIME*MILISEC);
                    if (midg_msg)
                    {
                        midg_msg=false;
                        t_ins_sec=(sss1.tv_sec-t_midg.tv_sec);
                        t_ins_usec=sss1.tv_usec-t_midg.tv_usec;
                    }else{
                        delta_t.A[0]=dt;
                        delta_t.A[1]=dt;                       
                        delta_t.A[2]=dt;

                    }

                    delta_t.A[0]=t_ins_sec+double(t_ins_usec)/1000000;
                    delta_t.A[1]=t_ins_sec+double(t_ins_usec)/1000000;
                    delta_t.A[2]=t_ins_sec+double(t_ins_usec)/1000000;

                    
                    rc=Pred_Angle_Data(&Euler,&delta_t);
                    rc=SendMotorData(&ant_angles);
                    //cout<<"az send: "<<float(delta_t.A[2])<<" "<<ant_angles.A[0]<<"----"<<float(px_deg)<<endl;
                    cout<<"az send: "<<(sss1.tv_sec-zulu_time.tv_sec)*1000000+(sss1.tv_usec-zulu_time.tv_usec)<<" "<<ant_angles.A[0]<<endl;
                    usleep(3000);
                    
                    //./motorcout<<"-------------------------------------"<<dt<<endl;
                    break;
                case MODE_SIN:
                    if  (check_t_Period<=sin_repetitions*sin_period*TIME_SCALE*SEC_MILI)
                    {
                        motor_angles.A[0]=(FLOAT32)sin_amplitude*ANGLE_SCALE*sin(2*pi/(sin_period*ANGLE_SCALE)*check_t_Period*MILISEC);
                        rc=SendMotorData(&motor_angles);
                        
                        std::time_t t = std::time(0);   // get time now
                        std::tm* now = std::localtime(&t);
                        int hr = now->tm_hour;
                        int min = now->tm_min;
                        int sec = now->tm_sec;
                        cout<<hr*3600+min*60+sec<<": "<<check_t_Period<<"sending: "<<motor_angles.A[0]<<endl;

                    }
                    if  (check_t_Period>total_period*SEC_MILI)
                            startTime1 = clock();
                    if (dT>total_period*SEC_MILI*SEC_MILI)
                         gettimeofday(&sssT,NULL); 
                    break;
                default:
                    break;
            }  
            check = 0;

        }
    }
}




RC Set_motor_params(SYSTEM_MODE s_mode,INT16 amplitude,INT16 period,INT16 rep,INT16 tot_period)
{
    system_mode=s_mode;
    sin_amplitude=amplitude;
    sin_period=period;
    sin_repetitions=rep;
    total_period=tot_period;
}

RC  Pred_Angle_Data(VEC *InsInfo,VEC* delta_t)

{
   
   RC rc;
   VEC rates;
   rates.A[0]=double(InsInfo->A[0])/100.0*DEG2RAD*delta_t->A[0];
   rates.A[1]=double(InsInfo->A[1])/100.0*DEG2RAD*delta_t->A[1];
   rates.A[2]=double(InsInfo->A[2])/100.0*DEG2RAD*delta_t->A[2];
   rc=predict_angles(&DCM,&ant_angles,&Vned,&Vant,&rates);
 
   //Vant.A[0]=ant_angles.A[0];
   //rc=Algo_SendMotorData(&ant_angles);
   return rc;
}


