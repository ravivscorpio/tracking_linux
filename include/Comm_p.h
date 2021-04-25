/************************************************************************
*        Gsatcom LTD., All Rights Reserved
* -----------------------------------------------------------------------
* FILE NAME     : Comm_p.h
* AUTHORS       : Raviv Kiron
* CREATE TIME   : 13/02/2009 09:16:10
************************************************************************/
#ifndef __Comm_p__H__
#define __Comm_p__H__


/***** Include files ***************************************************/

#include "Comm.h"
#include "Utils.h"

/***** Defines  ********************************************************/


#define MAX_MSG_BUFFS 9


MSG Msg1;
MSG Msg2;
MSG Msg3;
MSG Msg4;
MSG Msg5;
MSG Msg6;
MSG Msg7;
MSG Msg8;
MSG Msg9;


MSG *MsgBank[]=
{
   &Msg1,
   &Msg2,
   &Msg3,
   &Msg4,
   &Msg5,
   &Msg6,
   &Msg7,
   &Msg8,
   &Msg9

};
/***** Private Return Code *********************************************/


/***** Functions Declarations ******************************************/


#endif  /* __Comm_p__H__ */
/* Do not add any thing below this line ! */
