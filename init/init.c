#include "motorInfo.h"
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include "serial.h"
using namespace std;


int main()
{
    VEC motor_angles;
    RC rc;
    BYTE  azimuth_angle;
    time_t startTime = clock();
    time_t check = 0;

     char c;
unsigned char kkk=0 ;
cout << "Azimuth angle? ";
cin >> azimuth_angle;
while(1) //program main loop
    {
        time_t check = float(clock() - startTime)/CLOCKS_PER_SEC*1000;
        while( check >= 5 ) //after 5 milliseconds have elapsed
        {
            //cout << kkk << endl;
            startTime = clock();
              motor_angles.A[0]=float(azimuth_angle);
			  rc=SendMotorData(&motor_angles);
            check = 0;
            kkk++;
        }
    }

    return 0;


    

}


