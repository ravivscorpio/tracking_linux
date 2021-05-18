#include "motorInfo.h"
#include "MidjDrv.h"
#include "aim.h"
#include "term.h"
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>


//#define _LOG_

int rcc;
pthread_attr_t attr1,attr2,attr3,attr4,attr5;
struct sched_param param1,param2,param3,param4,param5;





int main()
{

    VEC motor_angles;
    RC rc;
    BYTE  azimuth_angle;
    time_t startTime = clock();
    time_t check = 0;
    pthread_t id1,id2,id3,id4,id5;
    int ret1,ret2,ret3,ret4,ret5;


    unsigned char kkk=0 ;
    //cout << "Azimuth angle? ";
    //cin >> azimuth_angle;
    Motor_init();
    Comm_Init();
    MidjDrv_Init();

    
    int n=0;
    int max_prio = sched_get_priority_max(SCHED_RR);

    rcc = pthread_attr_init (&attr1);
    rcc = pthread_attr_setschedpolicy(&attr1,SCHED_RR);
    rcc = pthread_attr_getschedparam (&attr1, &param1);
    (param1.sched_priority)= max_prio;
    rcc = pthread_attr_setschedparam (&attr1, &param1);

    rcc = pthread_attr_init (&attr2);
    rcc = pthread_attr_setschedpolicy(&attr2,SCHED_RR);
    rcc = pthread_attr_getschedparam (&attr2, &param2);
    (param2.sched_priority)=max_prio;
    rcc = pthread_attr_setschedparam (&attr2, &param2);

    rcc = pthread_attr_init (&attr3);
    rcc = pthread_attr_setschedpolicy(&attr3,SCHED_RR);
    rcc = pthread_attr_getschedparam (&attr3, &param3);
    (param3.sched_priority)=max_prio;
    rcc = pthread_attr_setschedparam (&attr3, &param3);

    rcc = pthread_attr_init (&attr4);
    rcc = pthread_attr_setschedpolicy(&attr4,SCHED_RR);
    rcc = pthread_attr_getschedparam (&attr4, &param4);
    (param4.sched_priority)=10;
    rcc = pthread_attr_setschedparam (&attr4, &param4);

    rcc = pthread_attr_init (&attr5);
    rcc = pthread_attr_setschedpolicy(&attr5,SCHED_RR);
    rcc = pthread_attr_getschedparam (&attr5, &param5);
    (param5.sched_priority)=10;
    rcc = pthread_attr_setschedparam (&attr5, &param5);


    ret1=pthread_create(&id1,&attr1,&threadRcv, NULL);
    ret2=pthread_create(&id2,&attr2,&threadSend, NULL);
    ret3=pthread_create(&id3,&attr3,&threadMIDGHandler, NULL);
    TermDrv_Init();
    ret4=pthread_create(&id4,&attr4,&ThreadTermRx, NULL);
    ret5=pthread_create(&id5,&attr5,&ThreadTermTx, NULL);
    



    while (true)
    {

        sleep(1);
    }

    return 0;


    

}


