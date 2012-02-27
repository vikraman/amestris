#include "mesg.h"
#include "message.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_2 200
#define  CANVAS_1 200

main()
{ 
  unsigned c=0;
  char Buffer[40];
  int count =1 ;
  int backcolor;
  struct smt gm;
  pid_t hproc=GUILogIn();
  sleep(2);
  printf("\n sender2: Sending request %u",c++);
  Create_Window(WINDOW_2,0,hproc,0,0,350,100);  
  Create_Canvas(CANVAS_1,WINDOW_2,hproc,5,15,340,80);
  Show_Window(WINDOW_2,hproc);
  Draw_Text(CANVAS_1,WINDOW_2,hproc,10,1,"Created Window 2");
  sleep(2); 
  while(count){
      int backcolor=rand()%255;
      Draw_Bar(CANVAS_1,WINDOW_2,hproc,0,0,340,80,backcolor);
      sprintf(Buffer,"Running  %02d sec   ",count++);
      Set_Text_Colors(CANVAS_1,WINDOW_2,hproc,count%255,backcolor,rand()%MAX_FONT_NO);
      Draw_Text(CANVAS_1,WINDOW_2,hproc,10,30,Buffer);
	  /*if(count % 5==0){
		Hide_Window(WINDOW_2,hproc);
		sleep(2);
		Show_Window(WINDOW_2,hproc);
	  }
	  */
      sleep(1);
      if(GUIOutput(GMChannel,hproc,&gm)){
         printf(" \n sender2 %d %d %d %d %d ",gm.type,gm.A,gm.B ,gm.C,gm.D);
	 if(gm.C==SS_DESTROYWINDOW)
	  break;
	 } 
				      
      }
  Delete_Window(WINDOW_2,hproc);
  puts("\n sender2: Done \n\n");
  return 0;
 }
  
  
  
