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
#include <Utils.h>





int main(int argc, char *argv[])
{

    VEC motor_angles;
    RC rc;
    BYTE  azimuth_angle;
  
    Motor_init();
    Comm_Init();
    MidjDrv_Init();
    TermDrv_Init();
   
    while (true)
    {

        sleep(1);
    }

    return 0;


    

}


