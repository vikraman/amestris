#include "processinput.h"
#include "mesg.h"
#include "keymap.h" 
#include "requestlist.h"
#include <stdio.h>
#include <unistd.h>
#include "debug.h"
/*  variables */
int /*MsgQueue,*/ RequestQueue;
Mesg my_msg;
/*Mesg rq_msg ;*/
/* function definitions */

boolean InitInput(int ikey,int rkey){  
  // sleep(1);
  /*if((MsgQueue=create_mq(ikey))==-1)
   return false;
  // sleep(1);*/
  if((RequestQueue=create_mq(rkey))!=-1)
    return true;
  else
   return false;
  } /* end proc */

  
boolean CloseInput(void){
  // sleep(1);
  /*remove_mq(MsgQueue);*/
  // sleep(1);
  remove_mq(RequestQueue);
  return true;
  }/* end proc */

//boolean GetSystemInput(SystemInput *pSi)
boolean GetCombinedInput(SystemInput *pSi,KeyboardInput *pki,int *picode)
{
   short cnt;
   int  start;
   my_msg.mesg_type=0;
   cnt =mesg_recvb(RequestQueue,&my_msg) ;
   if(cnt<0 && ((errno==ENOMSG)||(errno==EINTR)))
    return false;
   /*if(errno==EINTR)
		return false;	*/
    if(my_msg.mesg_type!=0xFFF0001){   
       pSi->Process_Id=my_msg.mesg_type&0xFFFF;
       pSi->Seq=my_msg.mesg_type>>16; 
       DEBUG_WRITE("\n Request from %d[%d]",pSi->Process_Id,pSi->Seq);
       pSi->Request_Id=my_msg.mesg_text[0];
       //Added for A3 LC
       if(pSi->Request_Id == RQ_DRAW_BARS)
       {

       	memcpy(&pSi->Parameters[0],&my_msg.mesg_text[1],sizeof(int)*3);
       	memcpy(&pSi->Otherparams[0][0],&my_msg.mesg_text[13],
			sizeof(int)*(pSi->Parameters[2])*8);
        start = 13+pSi->Parameters[2]*8*sizeof(int);
       	memcpy(&pSi->labels[0],&my_msg.mesg_text[start],pSi->Parameters[2]*30);
	
       }//
       //Added for A3 LC
       else if(pSi->Request_Id == RQ_MENUBUTTONS)
       {
       	memcpy(&pSi->Parameters[0],&my_msg.mesg_text[1],sizeof(int)*3);
       	memcpy(&pSi->Otherparams[0][0],&my_msg.mesg_text[13],
			sizeof(int)*(pSi->Parameters[2])*8);
        start = 13+pSi->Parameters[2]*8*sizeof(int);
       	memcpy(&pSi->labels[0],&my_msg.mesg_text[start],pSi->Parameters[2]*20);
       }//
       //Added for A3 LC
       else if(pSi->Request_Id == RQ_WRITE_TEXTS)
       {

       	memcpy(&pSi->Parameters[0],&my_msg.mesg_text[1],sizeof(int)*3);
       	memcpy(&pSi->Otherparams[0][0],&my_msg.mesg_text[13],
			sizeof(int)*(pSi->Parameters[2])*8);
        start = 13+pSi->Parameters[2]*8*sizeof(int);
       	memcpy(&pSi->labels[0],&my_msg.mesg_text[start],pSi->Parameters[2]*60);
	
       }//
       else
       memcpy(&pSi->Parameters[0],&my_msg.mesg_text[1],sizeof(int)*9);
       *picode=1;
       return true;
	 
    }
    *picode=2; 
    pki->type=my_msg.mesg_text[0];
    pki->value=my_msg.mesg_text[1];
    //printf("SSGUI:key:%d\n",my_msg.mesg_text[1]);
    return true;
}/* end proc */


