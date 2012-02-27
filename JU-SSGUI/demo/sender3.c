#include "mesg.h"
#include "message.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_2  200
#define WINDOW_21 300
#define CANVAS_2  300
#define CANVAS_1  200

main()
{ 
  char Buffer[40];
  int count =1 ;
  int backcolor;
  struct smt gm;
  pid_t hproc=GUILogIn();
  Create_Window(WINDOW_2,0,hproc,250,50,350,100);  
  Create_Window(WINDOW_21,WINDOW_2,hproc,250,350,350,100);  
  Create_Canvas(CANVAS_1,WINDOW_2,hproc,255,65,340,80);
  Create_Canvas(CANVAS_2,WINDOW_21,hproc,255,365,340,80);
  Show_Window(WINDOW_2,hproc);
  Show_Window(WINDOW_21,hproc);
  sleep(1);
  Draw_Text(CANVAS_1,WINDOW_2,hproc,10,1,"Created Window 2");
  Draw_Text(CANVAS_2,WINDOW_21,hproc,10,1,"Created Window 2.1");
  sleep(2); 
  backcolor=rand()%255;
  Draw_Bar(CANVAS_1,WINDOW_2,hproc,0,0,340,80,backcolor);
  Draw_Bar(CANVAS_2,WINDOW_21,hproc,0,0,340,80,backcolor);
  Set_Text_Colors(CANVAS_1,WINDOW_2,hproc,count%255,backcolor,35);
  Set_Text_Colors(CANVAS_2,WINDOW_21,hproc,count%255,backcolor,35);
  while(count){
      sprintf(Buffer,"Running  %04d  ",count++);
      Draw_Text(CANVAS_1,WINDOW_2,hproc,10,60,Buffer);
      Draw_Text(CANVAS_2,WINDOW_21,hproc,10,60,Buffer);
      //usleep(80000);
	  Shift_Up_By(CANVAS_1,WINDOW_2,hproc,15,backcolor);
	  Shift_Up_By(CANVAS_2,WINDOW_21,hproc,15,backcolor);
      if(GUIOutput(GMChannel,hproc,&gm)){
	     if(gm.C==SS_DESTROYWINDOW)
	    	break;
	 }
	 usleep(10000); 
				      
 }
 Delete_Window(WINDOW_2,hproc);
 Delete_Window(WINDOW_21,hproc);
 puts("\n sender3: Done \n\n");
 return 0;
 }
  
  
  
