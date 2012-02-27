#include "debug.h"
#include "guiprocess.h"
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
int pr;
FILE *pd;

main(int argc,char *argv[])
{
 pid_t pid;
 
/* 
 struct sched_param sched_parms;
 sched_parms.sched_priority = 3;
 sched_setscheduler(getpid(),SCHED_FIFO,&sched_parms);
 sleep(2);
*/ 
  
 Canvas *pC=NULL;
DEBUG_START("ss.rpt");

pd=fopen("graph.prn","wb+");
fclose(pd);
	
//pr=open("/dev/lp0",O_WRONLY);
	
InitGuiSys(10);
InitSystemSettings();

/******************* Init Screen ******************************************/
int i=0;
short int ret;
ret=CreateWindow(1,0,0,200,639,65,"ASL@ AGNI CHECKOUT Startup < Alpha Release 2.8.3 >",1,1);
ret=CreateCanvas(1,1,5,220,630,35,1,173,0);
pC=(Canvas *)SS_WindowStructure.GetWindowObject(1,1);
ShowWindow(1);
for(i=0;i<630;i+=12){
 DrawBar(1,1,i,5,10,10,1,1<<16|1);
 DrawBar(1,1,i,20,10,3,12,1<<16|1);
 usleep(2000);
}
/**************************************************************************/
/******************** Fork the processes ***************************/
if(argc>1){
 DrawBar(1,1,0,0,630,35,173,1);
 struct stat buf;
 mode_t mode;
 pC->SetTextColors(12,173,14);
 SetWindowTitle(1,"[ Gui2.8.3 ] Starting Processes.....");  for(i=1;i<argc;i++){
    if(!stat(argv[i],&buf)){
	   mode=buf.st_mode;
       mode&=(S_IXOTH|S_IXUSR|S_IXGRP);
       if(!mode){
         pC->ShiftUpBy(30,173);
         pC->SetTextColors(12,173,14);
         TextWrite(1,1,100,10,argv[i],1<<16|1);
		 sleep(3);
	   }
       else{
          pC->ShiftUpBy(35,173);
          pC->SetTextColors(10,173,14);
          TextWrite(1,1,100,10,argv[i],1<<16|1);
          pid=fork();
          if(!pid)
              execv(argv[i],NULL);
          sleep(1);
          } 
    }
	else{  
         pC->ShiftUpBy(30,173);
	     pC->SetTextColors(30,173,14);
         TextWrite(1,1,100,10,argv[i],1<<16|1);
		 sleep(1);
	}
   }
}        
sleep(1);
DeleteWindow(1);
ShowAllWindows();
Run_Window_System();
ShutDownGuiSys();
DEBUG_END(" System ShutDown ");
return 0;
}
     

