/*written by MSB & Co. dated 17/11/99*/
#include "mesg.h"
/******************************************************************************/

void mesg_send(int mq_id, Mesg *mesgptr)
{
msgsnd(mq_id,(char *) &(mesgptr->mesg_type),mesgptr->mesg_len,0);
}
/******************************************************************************/
int mesg_recvb(int mq_id, Mesg *mesgptr)
{
int n;
n=msgrcv(mq_id,(char *) &(mesgptr->mesg_type),MAXMESGDATA,mesgptr->mesg_type,0);

mesgptr->mesg_len=n;
if(n>=0)
  mesgptr->mesg_text[n]='\0';
return(n);
}
/******************************************************************************/
int mesg_recvi(int mq_id, Mesg *mesgptr)
{
int n;
n=msgrcv(mq_id,(char *) &(mesgptr->mesg_type), MAXMESGDATA,mesgptr->mesg_type,IPC_NOWAIT);

if ((mesgptr->mesg_len=n) <0){
	//printf("\nmesg_recv: msgrcv error");
	return(n);
	}

mesgptr->mesg_text[n]='\0';
return(n);
}
/*****************************************************************************/
int create_mq(key_t mq_key)
{
int mq_id;

if((mq_id=msgget(mq_key, PERMS|IPC_CREAT|IPC_EXCL))==-1)
{
	
	if((mq_id=msgget(mq_key, PERMS|IPC_CREAT))==-1)
	{
		printf("\ncreate_mq: can't open message queue with key %d",mq_key);
		return(-1);
	}
	else
	{
		return(mq_id);
	}
}
return(mq_id);
}

/*********************************************************************/
int remove_mq(int mq_id)
{
int ret_val;
ret_val=msgctl(mq_id, IPC_RMID, 0);
return(ret_val);
}
/**********************************************************************/

int mesg_recv(int mq_id, Mesg *mesgptr)
{
int n;
long seq_no;
int flag=1;

seq_no=mesgptr->mesg_type;


while(flag)
{
mesgptr->mesg_type=0;
n=msgrcv(mq_id,(char *) &(mesgptr->mesg_type), MAXMESGDATA,0,0);

if ((mesgptr->mesg_len=n) <0)
	printf("\nmesg_recv: msgrcv error");
//printf("\nmesg_recv: one message received successfully");
//printf("\nmesg_recv: message type = %ld, message text = %s",mesgptr->mesg_type, mesgptr->mesg_text);
if(mesgptr->mesg_type<seq_no)
	continue;
if(mesgptr->mesg_type>=seq_no)
	break;
}
   return(n);
}

/*****************************************************************************/
