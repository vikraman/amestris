#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25 
#define CANVAS_1 399
#define  MENU_1 499
main()
{ 
  unsigned c=0;
  char Buffer[40];
  struct smt gm;
  pid_t hproc=GUILogIn();
  int count =200 ;
  sleep(2);
  Create_Window(WINDOW_1,0,hproc,0,0,639,479);  
  Create_Canvas(CANVAS_1,WINDOW_1,hproc,5,45,630,478);
  Draw_Text(CANVAS_1,WINDOW_1,hproc,10,30,"sender7");
  Create_Menu(MENU_1,WINDOW_1,hproc,5,15,630,20,"/sample3.md");
  Show_Window(WINDOW_1,hproc);
  
  while(1){
       //usleep(10000); 
      if(GUIOutput(GMChannel,hproc,&gm)){
         printf(" \n sender5 %d %d %d %d %d ",gm.type,gm.A,gm.B ,gm.C,gm.D);
        if(gm.C==SS_DESTROYWINDOW)
          break;
	if(gm.B==MENU_1){
	   sprintf(Buffer," Menu returns value %d",gm.C);
           Draw_Bar(CANVAS_1,WINDOW_1,hproc,10,50,200,30,30);
	   Draw_Text(CANVAS_1,WINDOW_1,hproc,10,50,Buffer);
	   } 
	}
		     
      }
  Delete_Window(WINDOW_1,hproc);
  puts("\n sender7: Done \n\n");
  return 0;
 }
  
  
  
