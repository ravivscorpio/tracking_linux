/************************************************************************
*        Gsatcom LTD., All Rights Reserved                  
* -----------------------------------------------------------------------
* FILE NAME     : Protocol.h
* AUTHORS       : Raviv Kiron        
* CREATE TIME   : 16/10/2020 15:45:05
************************************************************************/
#ifndef __Protocol__H__
#define __Protocol__H__

// ##################################################################
// !! Care for all units must be taken when change this file ...
// !! This file is parsing during compilation of all system units as
// !! well for the PCsim program
// ##################################################################

/***** Include files ***************************************************/

#include "defs.h"

/***** Defines  ********************************************************/
#define MAX_DATA_LEN  228
 
#define PREAMBLE_1 0x66 
#define PREAMBLE_2 0x99
#define PREAMBLE_3 0x5A 
#define PREAMBLE_4 0xA5
#define PREAMBLE_VAL1 0x6699         // ! Do not change (PC use)
#define PREAMBLE_VAL2 0x5AA5         // ! Do not change (PC use)
#define PREAMBLE32    ((PREAMBLE_VAL1 << 16) | PREAMBLE_VAL2)

#define SRC_ADDR_SHIFT 0
#define DST_ADDR_SHIFT 8
#define SRC_ADDR(addr) (addr & 0xff)
#define DST_ADDR(addr) ((addr >> DST_ADDR_SHIFT) & 0xff)

#define OP_CODE_SHIFT   0
#define RESP_CODE_SHIFT 8
#define OP_CODE(OpCode)   (OpCode & 0xff)
#define RESP_CODE(OpCode) ((OpCode >> RESP_CODE_SHIFT) & 0xff)
                                
/* Users Addresses */
#define BROADECAST_ADDR           0xFF

#define PTM_MULTICAST_ADDR        0xFE
#define BUC_MULTICAST_ADDR        0xFD
#define LNB_MULTICAST_ADDR        0xFC
#define PLATE_1_MULTICAST_ADDR    0xFB
#define PLATE_2_MULTICAST_ADDR    0xFA
#define PLATE_3_MULTICAST_ADDR    0xF9

#define PTM_A_NA_CONT_ADDR        0xF3
#define LNB_NA_CONT_ADDR          0xF2
#define BUC_NA_CONT_ADDR          0xF1 
#define PLATE_NA_MULTICAST_ADDR   0xF0


#define MCM_CONT_ADDR             0x01
#define MCM_POWER_CONT_ADDR       0x02 
#define MCM_DSP_ADDR              0x03

#define ACU_ADDR                  0x05
#define INS_ADDR                  0x06
#define MODEM_ADDR                0x07

#define PC_ADDR                   0x08
#define LABVIEW_ADDR              0x09
                            
#define CCM_A_CONT_ADDR           0x10
#define CCM_P_CONT_ADDR           0x11
                            
#define PPM_A_CONT_ADDR           0x20
#define PPM_P_CONT_ADDR           0x21
                            
#define PTM_1A_CONT_ADDR          0x30
#define PTM_1P_CONT_ADDR          0x31
                            
#define PTM_2A_CONT_ADDR          0x40
#define PTM_2P_CONT_ADDR          0x41
                            
#define PTM_3A_CONT_ADDR          0x50
#define PTM_3P_CONT_ADDR          0x51
 
#define LNB_1_CONT_ADDR           0x60

#define LNB_2_CONT_ADDR           0x70

#define LNB_3_CONT_ADDR           0x80

#define BUC_1_CONT_ADDR           0x90

#define BUC_2_CONT_ADDR           0xA0

#define BUC_3_CONT_ADDR           0xB0


typedef enum
{
   // 0x00 & 0xFF used for Global Successive by all bus types
   ACK                            = 0x00,
   NACK                           = 0xFF,
                         
   // 0x01 - 0x0F range for use by the BOOTbus
   BOOTBUS_OPCODE_RANGE           = 0x01,
   BOOTBUS_OPCODE_RANGE_END       = 0x08,
   BOOTBUS_POWER_OPCODE_RANGE     = 0x09,
   BOOTBUS_POWER_OPCODE_RANGE_END = 0x0F,
         
   // 0x10 - 0x18 range for use by the Pbus
   PBUS_OPCODE_RANGE              = 0x10,
   PBUS_OPCODE_RANGE_END          = 0x18,

   // 0x19 - 0x1F range for use by the ACUbus
   ACUBUS_OPCODE_RANGE            = 0x19,
   ACUBUS_OPCODE_RANGE_END        = 0x1F,   
   
   // 0x20 - 0x2F range for use by the MCM Power Controller   
   MCM_POWER_OPCODE_RANGE         = 0x20,
   MCM_POWER_OPCODE_RANGE_END     = 0x2F,
   
   // 0x30 - 0x4F range for use by the DSPbus
   DSPBUS_OPCODE_RANGE            = 0x30,
   DSPBUS_OPCODE_RANGE_END        = 0x4F,

   // 0x50 - 0x5F range for use by the Abus
   ABUS_OPCODE_RANGE              = 0x50,
   ABUS_OPCODE_RANGE_END          = 0x5F,

   // 0x60 - 0x7F range for use by the AMIP modem
   AMIP_OPCODE_RANGE              = 0x60,
   AMIP_OPCODE_RANGE_END          = 0x7F,     

   // 0x80 - 0x8F range for use by the SNMP
   SNMP_OPCODE_RANGE              = 0x80,
   SNMP_OPCODE_RANGE_END          = 0x8F,
   
   // 0x90 - 0x9F range for use by the INSbus
   INSBUS_OPCODE_RANGE            = 0x90,
   INSBUS_OPCODE_RANGE_END        = 0x9F,

   // 0xA0 - 0xAF range for use by the LoaderBus
   LOADERBUS_OPCODE_RANGE         = 0xA0,
   LOADERBUS_OPCODE_RANGE_END     = 0xAF,
   
   // 0xB0 - 0xFE range for use by the PCbus      
   PCBUS_OPCODE_RANGE             = 0xB0,
   PCBUS_OPCODE_RANGE_END         = 0xFE
}MSG_Commands;

/***** Typedefs ********************************************************/
 
#ifndef MJDSP 
typedef struct /* ! must be 32 bit aligned due to DSP limit */
{
   UINT16 Preamble1;
   UINT16 Preamble2;   
   UINT16 Addr;         // Dest & Src Addr
   UINT16 OpCode;       // OpCode & Resp_Code
   UINT16 DataLen;
   UINT16 CheckSum;
}MSG_HDR;
#else
typedef struct /* ! must be 32 bit aligned due to DSP limit */
{
   UINT16 Preamble1 :16;
   UINT16 Preamble2 :16;   
   UINT16 Addr      :16;  // Dest & Src Addr
   UINT16 OpCode    :16;  // OpCode & Resp_Code
   UINT16 DataLen   :16;
   UINT16 CheckSum  :16;
}MSG_HDR;
#endif

/* Message packet */
#ifndef MJDSP
typedef struct
{
   MSG_HDR Header;
   BYTE    Data[MAX_DATA_LEN];
}MSG;
#else
/* Message packet */
typedef struct
{
   MSG_HDR Header;
   BYTE    Data[MAX_DATA_LEN/SIZEOF(BYTE)];
}MSG;
#endif

/***** Private Return Code *********************************************/


/***** Functions Declarations ******************************************/


#endif  /* __Protocol__H__ */
/* Do not add any thing below this line ! */
