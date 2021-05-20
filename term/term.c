#include "term.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include <string.h>
#include "motorInfo.h"
#include <pthread.h>

using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int sockfd, newsockfd, portno;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int nn;

MSG* TermRxMsg;
BYTE TermRxIdx;
//BYTE val;

RC TermDrv_Init (void)
{
    int rcc_term_tx,rcc_term_rx;
    pthread_attr_t attr_term_tx,attr_term_rx;
    struct sched_param param_term_tx,param_term_rx;
    pthread_t id_term_tx,id_term_rx;
    int ret_term_tx,ret_term_rx;

    rcc_term_rx = pthread_attr_init (&attr_term_rx);
    rcc_term_rx = pthread_attr_setschedpolicy(&attr_term_rx,SCHED_RR);
    rcc_term_rx = pthread_attr_getschedparam (&attr_term_rx, &param_term_rx);
    (param_term_rx.sched_priority)=10;
    rcc_term_rx = pthread_attr_setschedparam (&attr_term_rx, &param_term_rx);

    rcc_term_tx = pthread_attr_init (&attr_term_tx);
    rcc_term_tx = pthread_attr_setschedpolicy(&attr_term_tx,SCHED_RR);
    rcc_term_tx = pthread_attr_getschedparam (&attr_term_tx, &param_term_tx);
    (param_term_tx.sched_priority)=10;
    rcc_term_tx = pthread_attr_setschedparam (&attr_term_tx, &param_term_tx);

    TermRxMsg = NULL;
    TermRxIdx = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 50001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);



    
    ret_term_rx=pthread_create(&id_term_rx,&attr_term_rx,&ThreadTermRx, NULL);
    ret_term_tx=pthread_create(&id_term_tx,&attr_term_tx,&ThreadTermTx, NULL);
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    if (newsockfd < 0)
    { 
        error("ERROR on accept");

    }
    return(OK);
}

void * ThreadTermRx(void* args)
{
    MSG* TMsg;
    term_msg *tmsg;
    RC rc=OK;
    while(TRUE)
    {
        
        rc=TermDrvRx(&TMsg);
        if (rc)
            Comm_BuffFree(TMsg,TRUE);
        if (TMsg)
            if (TMsg->Header.OpCode == 0x11)
            {
                tmsg =(term_msg*)TMsg->Data;
                //all angle units are send in deg*100
                switch ((TERM_OPCODE)tmsg->cmd_id)
                {
                    case OP_SIN:
                   
                        Set_motor_params(MODE_SIN,tmsg->data_1*100,tmsg->data_2*100,tmsg->data_3,tmsg->data_4);
                        break;
                    case OP_TRK:
                        Set_motor_params(MODE_TRK,0,0,0,0);
                        break;
                    case OP_PNT:
                        Set_motor_params(MODE_PNT,(INT16)tmsg->data_1+(INT16)tmsg->data_2*256,0,0,0);
                        break;
                    default:
                        Set_motor_params(MODE_TRK,0,0,0,0);	
                        break;
                }


			    Comm_BuffFree (TMsg, FALSE);

            }

    
        
    }
    close(newsockfd);
    close(sockfd);
}

void * ThreadTermTx(void* args)
{
    INT16 az100;
    term_msg tm1;
    int check=0,startTime=clock();
    RC rc=OK;
    while(TRUE)
    {
        check = float(clock() - startTime)/CLOCKS_PER_SEC1*1000;
            if (check>100)
            {
                startTime=clock();
                az100=(INT16)GetPx()*100;
                BuildTermMsg(&tm1, 0xA, az100);
                rc=Term_Tx(&tm1);
                
            }
    }

}

RC TermDrvRx (MSG **msg)
{
    RC rc=OK;
    BYTE len;


   *msg = NULL;

    if (!TermRxMsg)
      {
        TermRxMsg = Comm_BuffAlloc (&rc, FALSE);
        TermRxIdx = 0;
      }
    if (!TermRxMsg)
        return(rc);
    while (TermRxIdx < 2)
    {
        len = 1;
        //rc=UartDrv_Rx (PORT_TERM, &(TermRxMsg->Data[TermRxIdx]), &len);
        len = read(newsockfd,&(TermRxMsg->Data[TermRxIdx]),len);
        if (TermRxIdx == 0)
        {
            if (TermRxMsg->Data[0] == Term_PRE_0)
                TermRxIdx++;
        }
        else if (TermRxIdx == 1)
        {
            if (TermRxMsg->Data[1] ==Term_PRE_1)
                TermRxIdx++;
        	else
            	TermRxIdx = 0;
        }
	}
      
    if (TermRxIdx > 1)    
	{
		len = Max_RxTermMsg - TermRxIdx;
        len = read(newsockfd,&(TermRxMsg->Data[TermRxIdx]),len);
        TermRxIdx += len;
	}
	if (TermRxIdx >=  Max_RxTermMsg)
    {
      TermRxIdx = 0;
      TermRxMsg->Header.OpCode = 0x11;
      *msg = TermRxMsg;

      TermRxMsg = NULL;
      return(OK);
     }

   return(1);
}

UINT8 TermHandleRx (term_msg *msg,UINT8* msg_data)
{
    RC rc=OK;
	UINT8 cmd_id;
	cmd_id=0x00;
	
	cmd_id=msg->cmd_id;
	msg_data[0]=msg->data_1;
	msg_data[1]=msg->data_2;
	msg_data[2]=msg->data_3;
	msg_data[3]=msg->data_4;

	

	return cmd_id;

}

RC Term_Tx(term_msg* tmsg)


{
   RC rc=OK;
   //rc=UartDrv_Tx (PORT_TERM, (BYTE*)tmsg, sizeof(*tmsg));
   write(newsockfd,(BYTE*)tmsg,sizeof(term_msg));
   return rc;
	
}


void BuildTermMsg(term_msg* tmsg, UINT8 cmd_id, INT32 data)
{
    // pointers were verified on a higher level


    UINT16 crc_16;

	term_msg* cmd_ptr;
    cmd_ptr = tmsg;



    cmd_ptr->magic_1 = 0x49;
    cmd_ptr->magic_2 = 0x5D;
    cmd_ptr->cmd_id = (UINT8)((cmd_id & 0xFF));



    cmd_ptr->data_1 = (UINT8)(data & 0x000000FF);
    cmd_ptr->data_2 = (UINT8)((data & 0x0000FF00) >> 8);
    cmd_ptr->data_3 = (UINT8)((data & 0x00FF0000) >> 16);
    cmd_ptr->data_4 = (UINT8)((data & 0xFF000000) >> 24);

    crc_16 = calc_servo_crc16(0, (UINT8*)cmd_ptr + 2, 5);
    cmd_ptr->crc_16_1 = (UINT8)((crc_16 & 0xFF00) >> 8);
    cmd_ptr->crc_16_2 = (UINT8)(crc_16 & 0x00FF);

    
}
