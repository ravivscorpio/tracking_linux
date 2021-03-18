#include "term.h"




MSG* TermRxMsg;
BYTE TermRxIdx;
//BYTE val;

RC TermDrv_Init (void)
{

   TermRxMsg = NULL;
   TermRxIdx = 0;

   return(OK);
}



RC TermDrvRx (MSG **msg)
{
    RC rc;
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
        rc=UartDrv_Rx (PORT_TERM, &(TermRxMsg->Data[TermRxIdx]), &len);
        
        if (rc)
            return(rc);

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
  			rc=UartDrv_Rx (PORT_TERM, &(TermRxMsg->Data[TermRxIdx]), &len);
  			if (rc)
     			return(rc);
  			TermRxIdx += len;
	}
	if (TermRxIdx >=  Max_RxTermMsg)
    {
      TermRxIdx = 0;
      TermRxMsg->Header.OpCode = 0x11;
      *msg = TermRxMsg;
      if (rc)
         return(rc);
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
   rc=UartDrv_Tx (PORT_TERM, (BYTE*)tmsg, sizeof(*tmsg));
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

    //crc_16 = calc_servo_crc16(0, (UINT8*)cmd_ptr + 2, 7);
    //cmd_ptr->crc_16_1 = (UINT8)((crc_16 & 0xFF00) >> 8);
    //cmd_ptr->crc_16_2 = (UINT8)(crc_16 & 0x00FF);

    
}
