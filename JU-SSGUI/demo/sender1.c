#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_2 200
#define  CANVAS_1 200

main()
{ 
  unsigned c=0;
  char Buffer[]="Distributed Computing Lab";
  char buffer1[80];
  int count =1 ;
  int backcolor;
  struct smt gm;
  pid_t hproc=GUILogIn();
  printf("\n Request for window creation ");
  if(!Create_Window(WINDOW_2,0,hproc,60,60,350,200)){
	   printf("\n Couldn't create window ");
	   exit(-1);
  }
  printf("\n Request for canvas creation ");
  if(!Create_Canvas(CANVAS_1,WINDOW_2,hproc,65,75,340,180))
  {
	  printf("\n Couldn't create canvas ");
	  exit(-1);
  }
  printf("\n Request for window show ");
  Show_Window(WINDOW_2,hproc);
  printf("\n Request for text");
  Draw_Text(CANVAS_1,WINDOW_2,hproc,10,1,"Created Window 1");
  sleep(2); 
  while(count){
    if(++count% 1000==0){
			printf("\n Request for hide");
			Hide_Window(WINDOW_2,hproc);
			printf("...done");
			printf("\n Request for window show ");
			sleep(10);
			Show_Window(WINDOW_2,hproc);
			printf("...done");
			backcolor=rand()%255;
			sprintf(buffer1," bc = %d",backcolor);
			printf("\n Request for title text");
			Set_Window_Title(WINDOW_2,0,hproc,buffer1);
		    printf("...done");
       }
      printf("\n Request for bar");	   
      Draw_Bar(CANVAS_1,WINDOW_2,hproc,0,0,340,180,backcolor);
      printf("...done");
      printf("\n Request for colors");
      Set_Text_Colors(CANVAS_1,WINDOW_2,hproc,count%255,backcolor,rand()%MAX_FONT_NO);
      printf("...done");
      printf("\n Request for text");
      Draw_Text(CANVAS_1,WINDOW_2,hproc,rand()%100,rand()%100,Buffer);
      printf("...done");
      if(GUIOutput(GMChannel,hproc,&gm)){
	     if(gm.C==SS_DESTROYWINDOW)
	       break;
	 	 }
      	//else
        	//usleep(4000);  
				      
      //sleep(1);  
      }
  Delete_Window(WINDOW_2,hproc);
  puts("\n sender1: Done \n\n");
  return 0;
 }
  
  
  
