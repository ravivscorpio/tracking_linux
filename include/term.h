#ifndef __TERM__H__
#define	__TERM__H__

#include "defs.h"
#include "defs_p.h"
#include "rc.h"
#include "comm.h"
#include "UartDrv.h"
#include "rc.h"

#define PORT_TERM UART_PORT2
#define Term_PRE_0 0x8B//for Rx msg
#define Term_PRE_1 0x3C
#define Max_RxTermMsg 9


typedef enum{
	OP_PNT=0x10,
	OP_TRK=0x1,
	OP_SIN=0x2,
	OP_MODE_RSSI=0x3
	
}TERM_OPCODE;



typedef struct {

    UINT8 magic_1;//0x49
    UINT8 magic_2;//0x5D
    UINT8 cmd_id;
    UINT8 data_1;
    UINT8 data_2;
    UINT8 data_3;
    UINT8 data_4;

    UINT8 crc_16_1;
    UINT8 crc_16_2;
}__attribute__ ((packed)) term_msg;


RC TermDrvRx (MSG **msg);
RC Term_Tx(term_msg* tmsg);
void BuildTermMsg(term_msg* tmsg, UINT8 cmd_id, INT32 data);


#endif	//	_TERM_H

