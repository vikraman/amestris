#include<stdio.h>
#include<sched.h>
#include<termios.h>
#include<term.h>
#include<unistd.h>
#include<stdlib.h>
#include"mesg.h"
#include"keymap.h"
#include"processinput.h"
#include"requestlist.h"
static struct termios is,ns;
static int peek_ch=-1;
void init_keyboard();
void close_keyboard();

void init_keyboard(){
    tcgetattr(0,&is);
    ns=is;
    ns.c_lflag &= ~ICANON;
    ns.c_lflag &= ~ECHO;
    ns.c_lflag &= ~ISIG;
    
    ns.c_cc[VMIN]=1;
    ns.c_cc[VTIME]=0;
    tcsetattr(0,TCSANOW,&ns);
    }

void close_keyboard(){
   tcsetattr(0,TCSANOW,&is);
   }

main()
{
 int rq_id;
 int i=1,count=0;
 int prev=10;
 
 /* 
 struct sched_param sched_parms;
 sched_parms.sched_priority = 3;
 sched_setscheduler(getpid(),SCHED_FIFO,&sched_parms);
 sleep(2);
 
 added 07/07/05 */
 
 Mesg my_msg;
 init_keyboard();
 rq_id =create_mq(REQUEST_QUEUE_ID);
 if(rq_id==-1) {
  puts("Request Queue could not be created");
  exit(1);
  }
 /* 
  mq_id =create_mq(INPUT_QUEUE_ID);
  if(mq_id==-1) {
   puts("Message Queue could not be created");
   exit(1);
  }
  */
 while(i!='\0'){
        scanf("%c",&i);
	if(i==0)
	  break;
	my_msg.mesg_text[0]=(char)(i);
   	my_msg.mesg_len=1;
  	my_msg.mesg_type=0xFFF0001; 
	mesg_send(rq_id,&my_msg);
	if(errno==EIDRM|| errno==EINVAL)
	 break;
        
   }
    printf("\n Sending Shutdown signal\n\n");
 	while(errno!=EIDRM&&errno!=EINVAL){ 
      my_msg.mesg_len=1;
      my_msg.mesg_type=1;
      my_msg.mesg_text[0]= RQ_GUI_SHUTDOWN;
      mesg_send(rq_id,&my_msg);
	}
close_keyboard();
return 0;
}
