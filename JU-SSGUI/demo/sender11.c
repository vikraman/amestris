#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_2 0x200
#define WINDOW_21 0x300
#define WINDOW_22 0x400
#define  CANVAS_1 0x200

main()
{ 
  unsigned c=0;
  char Buffer[]="Distributed Computing Lab";
  char buffer1[80];
  int count =1 ;
  int backcolor;
  int sf=1;
  struct smt gm;
  pid_t hproc=GUILogIn();
  printf("\n Request for window creation ");
  if(!Create_Window(WINDOW_2,0,hproc,50,50,350,200)){
	   printf("\n Couldn't create window ");
	   exit(-1);
  }
  printf("\n Request for canvas creation ");
  if(!Create_Canvas(CANVAS_1,WINDOW_2,hproc,55,65,340,180))
  {
	  printf("\n Couldn't create canvas ");
	  exit(-1);
  }
  printf("\n Request for window show ");
  printf("\n Request for text");
  Draw_Text(CANVAS_1,WINDOW_2,hproc,10,10,"Created Window 1");
  sleep(2); 
  Create_Window(WINDOW_21,WINDOW_2,hproc,150,150,350,200,1);
  Create_Window(WINDOW_22,WINDOW_21,hproc,250,250,350,200,1);
  Create_Canvas(CANVAS_1,WINDOW_21,hproc,155,165,340,180);
  Create_Canvas(CANVAS_1,WINDOW_22,hproc,255,265,340,180);
  Draw_Text(CANVAS_1,WINDOW_21,hproc,10,20,"Created Child Window 1.1");
  Draw_Text(CANVAS_1,WINDOW_22,hproc,10,40,"Created Child Window 1.1.1");
  Set_Window_Title(WINDOW_2,0,hproc,"Sender11");
  Set_Window_Title(WINDOW_21,WINDOW_21,hproc,"Sender11.1");
  Set_Window_Title(WINDOW_22,WINDOW_22,hproc,"Sender11.1.1");
  Show_Window(WINDOW_2,hproc);
  sleep(2);
  while(count){
      if(++count% 200==0){
		if(sf)	  
  	      Hide_Window(WINDOW_2,hproc);
		else
         Show_Window(WINDOW_2,hproc);
       backcolor=rand()%255;
       sprintf(buffer1," bc = %d",backcolor);
       //Set_Window_Title(WINDOW_2,0,hproc,buffer1);
	   sf=!sf;
       }
      Draw_Bar(CANVAS_1,WINDOW_2,hproc,0,0,340,180,backcolor);
      Set_Text_Colors(CANVAS_1,WINDOW_2,hproc,count%255,backcolor,MAX_FONT_NO+1);
      Draw_Bar(CANVAS_1,WINDOW_21,hproc,0,0,340,180,backcolor);
      Draw_Bar(CANVAS_1,WINDOW_22,hproc,0,0,340,180,backcolor);
      Draw_Text(CANVAS_1,WINDOW_2,hproc,rand()%100,rand()%100,Buffer);
      Draw_Text(CANVAS_1,WINDOW_21,hproc,rand()%100,rand()%100,
					  							"Created Child Window 1.1");
      Draw_Text(CANVAS_1,WINDOW_22,hproc,rand()%100,rand()%100,
					  							"Created Child Window 1.1.1");
      if(GUIOutput(GMChannel,hproc,&gm)){
	     if(gm.C==SS_DESTROYWINDOW)
	       break;
	 	 }
      else
      usleep(10000);  
				      
      }
  Delete_Window(WINDOW_2,hproc);
  puts("\n sender1: Done \n\n");
  return 0;
 }
  
  
  
