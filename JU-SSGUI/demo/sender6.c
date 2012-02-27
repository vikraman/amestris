#include "mesg.h"
#include "message.h"
#include "request.h"

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_1 700
#define  CANVAS_1 200
#define  BUTTON_1 701
main()
{ 
  unsigned c=0;
  char Buffer[40];
  pid_t hproc=GUILogIn();
  struct smt gm;
  int count =0 ;
  sleep(1);
 // printf("\n sender6: Sending request %u",c++);
  Create_Window(WINDOW_1,0,hproc,110,100,250,250);  
  Create_Table_Editor(CANVAS_1,WINDOW_1,hproc,120,130,"samp.rc");
  Create_Button(BUTTON_1,WINDOW_1,hproc,200,300,50,30,"Save");
  //   
  Show_Window(WINDOW_1,hproc);
  //Draw_Text(CANVAS_1,WINDOW_1,hproc,10,30,"Created Window");
  sleep(1); 
      do{
      //count++;
      //sprintf(Buffer,"Run %4d sec   ",count);
      //Draw_Text(CANVAS_1,WINDOW_1,hproc,10,30,Buffer);
      if(GUIOutput(GMChannel,hproc,&gm)){
          printf(" \n sender6 %d %d %d %d %d ",gm.type,gm.A,gm.B ,gm.C,gm.D);
	  if(gm.C==SS_DESTROYWINDOW)
	    break;
	  if(gm.C==SS_COMPONENT_USED&&gm.B==BUTTON_1)
	    {
	      Save_Table_Editor(CANVAS_1,WINDOW_1,hproc,"New.dat");
	      Shutdown_GUI() ;
              return 0 ; 
             }  
	  }  
      } while(1);
  Delete_Window(WINDOW_1,hproc);
  puts("\n sender6: Done \n\n");
  return 0;
 }
  
  
  
