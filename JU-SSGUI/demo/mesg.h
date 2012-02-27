/*written by MSB & Co. dated 17/11/99*/
#ifndef MESG_H
#define MESG_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include <termio.h>
#include <string.h>
#include<stdio.h>
#define PERMS 0666

#define MAXMESGDATA 1024 
#define MESGHDRSIZE (sizeof(Mesg) - MAXMESGDATA)

typedef struct
{
	int mesg_len;
	long mesg_type;
	char mesg_text[MAXMESGDATA];
} Mesg;

void mesg_send(int mq_id, Mesg *mesgptr);
int mesg_recvb(int mq_id, Mesg *mesgptr);
int mesg_recvi(int mq_id, Mesg *mesgptr);
int create_mq(key_t mq_key);
int remove_mq(int mq_id);
/*****************************************************************************/
#endif
