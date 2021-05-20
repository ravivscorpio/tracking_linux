/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Utils.c
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 11/10/2020 09:20:22
************************************************************************/


/***** Include files ***************************************************/
#include "Utils_p.h"

UINT16 NastedISRcounter;
UINT16 NastedWDOGcounter;
volatile RC lastRc;

const unsigned int crc16_tab[256] =
{
   0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
   0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
   0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,
   0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
   0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
   0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
   0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
   0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
   0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
   0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
   0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
   0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
   0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
   0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
   0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
   0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
   0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
   0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
   0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
   0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
   0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
   0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
   0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
   0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
   0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
   0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
   0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
   0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
   0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
   0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
   0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
   0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040
};

/************************************************************************
* Name       : Utils_Init (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC Utils_Init (void)
{

   /* initiate the interrupts parameters */



   return(OK);
}

/************************************************************************
* Name       : Utils_Start (void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC Utils_Start (void)
{
   /* Fix the ISR nasted counter */


   return(OK);
}

/************************************************************************
* Name       : RC DisableISR(UINT16 Param)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC DisableISR(UINT16 Param)
{



   return OK;
}

/************************************************************************
* Name       : RC EnableISR(UINT16 Param)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC EnableISR(UINT16 Param)
{



   return(OK);
}

/*********************************************************************************************************
* Name       : RC WdogEnable(void)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC WdogEnable(void)
{



   return OK;
}
/*********************************************************************************************************
* Name       : RC WdogDisable(void)
* Description: Disable the Wdog interrupt
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC WdogDisable(void)
{



   return OK;
}
/*********************************************************************************************************
* Name       : RC WdogRefresh(void)
* Description: Refresh the wdog mechanism to reset
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC WdogRefresh(void)
{


   return(OK);
}

/*********************************************************************************************************
* Name       : BOOL isWdogRestart (void)
* Description: check if the system was start because of WDOG
* Arguments  : None
* Effected   : None
* Note(s)    :
********************************************************************************************************/
BOOL isWdogRestart (void)
{
   return TRUE;
}

/************************************************************************
* Name       : RC CalcCRC16(BYTE *data, UINT32 len, UINT16 *res)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
************************************************************************/
RC CalcCRC16(BYTE *buf, UINT16 len, UINT16 *res)
{

   if (!buf || !res)
      return(rcUtilsWrongPtr);

   UINT16 CRC = 0;
   while (len--)
      CRC = (CRC >> 8)^crc16_tab[(CRC^(*buf++)) & 0xFF];

   *res = CRC;


   return(OK);
}

/*********************************************************************************************************
* Name       : CalcCheckSum()
* Description: Calculate check sum for buffer to send/receive
* Arguments  : void *pData - pointer to buffer
*              INT16 DataLen - size of buffer in bytes
* Effected   : UINT16 *pCheckSum - result of Work
* Note(s)    :
********************************************************************************************************/
RC CalcCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum)
{
   int i;

   if (!pData || !pCheckSum)
      return(rcUtilsWrongPtr);

   /* calculate sum16 of bytes */
   for (*pCheckSum = 0, i = 0 ; i < DataLen/2 ; i++)
      *pCheckSum += ((UINT16 *)pData)[i];

   *pCheckSum = ~(*pCheckSum);

   return(OK);
}

/*********************************************************************************************************
* Name       : CalcMsgCheckSum()
* Description: Calculate check sum for buffer to send/receive
* Arguments  : void *pData - pointer to MSG
* Effected   : UINT16 *pCheckSum - result of Work
* Note(s)    :
********************************************************************************************************/
RC CalcMsgCheckSum(MSG *pData, UINT16 *pCheckSum)
{
   int i;

   if (!pData || !pCheckSum)
      return(rcUtilsWrongPtr);

   /* calculate sum16 of bytes for message header */
   *pCheckSum = PREAMBLE_VAL1;
   *pCheckSum += PREAMBLE_VAL2;
   for (i = 2 ; i < 3+2 ; i++)
      *pCheckSum += ((UINT16 *)pData)[i];

   /* calculate sum16 of bytes for message data */
   if (pData->Header.DataLen)
      {
      for (i = 0; i < pData->Header.DataLen / 2; i++)
         *pCheckSum += ((UINT16 *)(pData->Data))[i];
      }
   if (pData->Header.DataLen & 1)
      *pCheckSum += ((UINT8 *)(pData->Data))[pData->Header.DataLen];

   *pCheckSum = ~(*pCheckSum);

   return(OK);
}

/*********************************************************************************************************
* Name       : CalcSwappedCheckSum()
* Description: Calculate check sum for a swapped buffer to send/receive
* Arguments  : void *pData - pointer to MSG
* Effected   : UINT16 *pCheckSum - result of Work
* Note(s)    :
********************************************************************************************************/
RC CalcSwappedCheckSum(MSG *pData, UINT16 *pCheckSum)
{
   int i;
   UINT16 msgLen;

   if (!pData || !pCheckSum)
      return(rcUtilsWrongPtr);

   msgLen = LittleXBig16(pData->Header.DataLen);

   /* calculate sum16 of bytes for message header */
   *pCheckSum = (PREAMBLE_VAL1);
   *pCheckSum += (PREAMBLE_VAL2);
   for (i = 2 ; i < 3+2 ; i++)
      *pCheckSum += LittleXBig16(((UINT16 *)pData)[i]);

   /* calculate sum16 of bytes for message data */
   if (msgLen)
      {
      for (i = 0; i < msgLen / 2; i++)
         *pCheckSum += LittleXBig16(((UINT16 *)(pData->Data))[i]);
      }
   if (msgLen & 1)
      *pCheckSum += LittleXBig16(((UINT8 *)(pData->Data))[msgLen]);

   *pCheckSum = LittleXBig16(~(*pCheckSum));

   return(OK);
}

/*********************************************************************************************************
* Name       : RC CalcFletcherCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum)
* Description: Calculate check sum for buffer to send/receive
* Arguments  : void *pData - pointer to buffer
*              INT16 DataLen - size of buffer in bytes
* Effected   : UINT16 *pCheckSum - result of Work
* Note(s)    :
********************************************************************************************************/
RC CalcFletcherCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum)
{
   BYTE CS0, CS1;
   int i;

   if (!pData || !pCheckSum)
      return(rcUtilsWrongPtr);

   CS0 = CS1 = 0;

   // calculate sum of bytes
   for (i = 0; i < DataLen; i++)
      {
      CS0 += ((BYTE *)pData)[i];
      CS1 += CS0;
      }

   *pCheckSum = (CS1 << 8) | CS0;

   return(OK);
}

/*********************************************************************************************************
* Name       : RC CalcAsciiOrBitsCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum)
* Description: Calculate check sum for buffer
* Arguments  : void *pData - pointer to buffer
*              INT16 DataLen - size of buffer in bytes
* Effected   : UINT16 *pCheckSum - result of Work
* Note(s)    :
********************************************************************************************************/
RC CalcAsciiOrBitsCheckSum(void *pData, INT16 DataLen, UINT16 *pCheckSum)
{
   BYTE CS0;
   int i;
   char CSh, CSl;

   if (!pData || !pCheckSum)
      return(rcUtilsWrongPtr);

   CS0 = 0;

   // calculate sum of bytes
   for (i = 0; i < DataLen; i++)
      CS0 ^= ((BYTE *)pData)[i];

   // convert it to ASCII
   CSl = (CS0 & 0xF) <= 9 ? (CS0 & 0xF) + '0' : (CS0 & 0xF) - 10 + 'A';
   CSh = (CS0 >> 4) <= 9 ? (CS0 >> 4) + '0' : (CS0 >> 4) -10 + 'A';

   *pCheckSum = (CSh << 8) | CSl;

   return(OK);
}

/************************************************************************
* Name       : RC MemClear(void *MemPtr, UINT32 size)
* Description: Quick memory clear
* Arguments  : void *MemPtr - destination address
*              UINT16 size in bytes count for memory clear
* Effected   :
* Note(s)    :
*************************************************************************/
RC MemClear(BYTE *MemPtr, UINT16 size)
{
   BYTE *bP;
   UINT16 *iP;
   UINT16 i = 0;
   UINT16 no = 0, sum = 0;
   no=size;
   bP=MemPtr;
   for (i = 0; i < no; i++)
      bP[i] = 0;
  
   return(OK);
}

/************************************************************************
* Name       : RC MemCopy(void *dst, const void *src, UINT16 size)
* Description: Safe and smart memory copy
* Arguments  : void *dst - destination address
*              void *src - source address
*              INT32 size in bytes count for memory copy
* Effected   :
* Note(s)    :
*************************************************************************/
RC MemCopy(void *dst, const void *src, UINT16 size)
{
   int  i;
   BYTE* Src = (BYTE *)src;
   BYTE* Dst = (BYTE *)dst;


   if (dst == src)
      return(OK);

   if (dst < src)
   {
      for (i = 0; i < size; )
      {
         // if not aligned to 2 bytes - copy one byte
         //if ((((UINT16)Src) & 0x0001) || (((UINT16)Dst) & 0x0001) || ((size - i) < 2))
         {
         Dst[0] = Src[0];

         Src++;
         Dst++;
         i++;
         }

      }
   }
   return(OK);
}



/*********************************************************************************************************
* Name       : RC Delay (UINT32 delay)
* Description:
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC Delay (UINT32 delay)
{

        RC rc=OK;
        for (; delay; delay--)
      //ClrWdt();


   return(rc);
}

/*********************************************************************************************************
* Name       : RC SwapMsg (MSG *pOrigin, MSG *pSwaped)
* Description: swap message endian
* Arguments  :
* Effected   :
* Note(s)    :
********************************************************************************************************/
RC SwapMsg (MSG *pOrigin, MSG *pSwaped)
{
   int i;
   int swapDir;

   if (!pOrigin)
      return(rcUtilsWrongPtr);
   if (!pSwaped)
      return(rcUtilsWrongPtr);

   for (i = 0; i < sizeof(MSG_HDR); i++)
      ((BYTE*)&(pSwaped->Header))[i] = ((BYTE*)&(pOrigin->Header))[i];

   if (pOrigin->Header.DataLen && pOrigin->Header.DataLen <= MAX_DATA_LEN)
      {
      swapDir = 1;
      for (i = 0; i < pOrigin->Header.DataLen; i++)
         {
         (pSwaped->Data)[i + swapDir] = (pOrigin->Data)[i];
         swapDir *= -1;
         }
      }

   return(OK);
}

/*********************************************************************************************************
* Name       : UINT32 LittleXBig32(UINT32 val)
* Description: Convert little to big for 32 bit
* Arguments  : None
* Effected   : None
* Note(s)    :
********************************************************************************************************/
UINT32 LittleXBig32(UINT32 val)
{
   UINT32 retVal, tmpVal;

   // MSB
   retVal = val & 0xFF;
   retVal = retVal << 24;

   // LSB
   tmpVal = val >> 24;
   retVal |= tmpVal;

   // MSB - 1
   tmpVal = (val << 8) & 0xFF0000;
   retVal |= tmpVal;

   // LSB + 1
   tmpVal = (val >> 8) & 0xFF00;
   retVal |= tmpVal;

   return(retVal);
}

/*********************************************************************************************************
* Name       : RC Error (RC rc)
* Description: Handle system errors
* Arguments  : None
* Effected   : None
* Note(s)    :
********************************************************************************************************/
RC Error (RC rc)
{

   if (rcIsErr(rc))
      {
      // Major Error so we wait for Wdog
      DisableISR(NULL);
  ////    while (1);
      }

   return(OK);
}

void clock_measure()
{
       time_t startTime = clock();
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    int hr = now->tm_hour;
	int min = now->tm_min;
	int sec = now->tm_sec;
    int secs=hr*3600+min*60+sec;
    int d_secs=0;
    BOOL first=TRUE;
    while (d_secs<11)
    {
            std::time_t t = std::time(0);   // get time now
            std::tm* now = std::localtime(&t);
            hr = now->tm_hour;
            min = now->tm_min;
            sec = now->tm_sec;
            d_secs=hr*3600+min*60+sec-secs;
            if ((d_secs>=1)&&(first))
            {
                startTime = clock();
                first = FALSE;
            }

    }
    std::cout<<"number of clocks per 10sec"<< clock()-startTime<<std::endl;
    exit(0);
}


