extern "C" {
//#include <asm/termbits.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
//#include <termios.h>

}

using namespace std;
#include <iostream>
#include "serial.h"
struct termios2 {
        tcflag_t c_iflag;                /* input mode flags */
        tcflag_t c_oflag;                /* output mode flags */
        tcflag_t c_cflag;                /* control mode flags */
        tcflag_t c_lflag;                /* local mode flags */
        cc_t c_line;                        /* line discipline */
        cc_t c_cc[NCCS];                /* control characters */
        speed_t c_ispeed;                /* input speed */
        speed_t c_ospeed;                /* output speed */
};
#define    BOTHER 0010000
mySerial::mySerial(string deviceName, int baud)
{
   handle=-1;
   Open(deviceName,baud);
}

mySerial::~mySerial()
{
  if(handle >=0)
      Close();
}

void mySerial::Close(void)
{
   if(handle >=0)
      close(handle);
   handle = -1;
}


bool mySerial::Open(string deviceName , int baud)
{
    struct termios tio;
    struct termios2 tio2;
    this->deviceName=deviceName;
    this->baud=baud;
    handle  = open(this->deviceName.c_str(),O_RDWR | O_NOCTTY /* | O_NONBLOCK */);

    if(handle <0)
       return false;
    tio.c_cflag = 0;
    tio.c_cflag =  CS8 | CLOCAL | CREAD; //& ~PARENB & ~CSTOPB & ~CRTSCTS & ~ISIG & ~CSIZE;
    tio.c_oflag = 0;
    tio.c_lflag = 0;       //ICANON;

    tio.c_iflag = 0;
    tio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tio.c_cc[VMIN]=0;
    tio.c_cc[VTIME]=0;     // time out every .1 sec
    ioctl(handle,TCSETS,&tio);

    ioctl(handle,TCGETS2,&tio2);
    tio2.c_cflag &= ~CBAUD;
    tio2.c_cflag |= BOTHER;
    tio2.c_ispeed = baud;
    tio2.c_ospeed = baud;
    ioctl(handle,TCSETS2,&tio2);

//   flush buffer
    ioctl(handle,TCFLSH,TCIOFLUSH);

    return true;
}

bool mySerial::IsOpen(void)
{
   return( handle >=0);
}

bool mySerial::Send( unsigned char  * data,int len)
{
   if(!IsOpen()) return false;
   int rlen= write(handle,data,len); 
   tcdrain(handle);
   
   return(rlen == len);
}




int  mySerial::Receive( unsigned char  * data, int len)
{
   int rlen;
   if(!IsOpen()) return -1;

   // this is a blocking receives
   //int lenRCV=0;


       rlen = read(handle,data,len);
      // lenRCV+=rlen;
      

   return  rlen;
}

bool mySerial::NumberByteRcv(int &bytelen)
{
   if(!IsOpen()) return false;
   ioctl(handle, FIONREAD, &bytelen);
   return true;
}

