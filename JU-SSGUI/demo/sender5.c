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
  bool copystate=true;
  bool filestat=true;
  sleep(2);
  Create_Window(WINDOW_1,0,hproc,200,100,350,300);  
  Create_Canvas(CANVAS_1,WINDOW_1,hproc,205,140,310,250);
  Create_Menu(MENU_1,WINDOW_1,hproc,202,115,345,20,"sample.md");
  Show_Window(WINDOW_1,hproc);
  
  while(1){
      if(GUIOutput(GMChannel,hproc,&gm)){
        Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,0,300,240,31);  
	sprintf(Buffer,"%d %d %d %d",gm.A,gm.B,gm.C,gm.D);
	Draw_Text(CANVAS_1,WINDOW_1,hproc,10,200,Buffer);
	
	if(gm.C==SS_DESTROYWINDOW)
          break;
	if(gm.B==MENU_1){
	   switch(gm.C){
	      case 401:
	         copystate=!copystate;
	 	 Set_Menu_Item(MENU_1,WINDOW_1,hproc,202,copystate);
	         Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,"Toggling Copy");
		 break;
	      case 201:
	          Set_Menu_Page(MENU_1,WINDOW_1,hproc,1003,false);
	          Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,"Zapping Device");
		  break;
	       case 403:
	         Set_Menu_Page(MENU_1,WINDOW_1,hproc,2001,false);
	         Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,"Nuking Special");
		 break;
	       case 402:
	         Set_Menu_Page(MENU_1,WINDOW_1,hproc,1003,true);
	         Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,"Enabling device");
		 break;
	       case 202:
	          Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,"Enabling Special");
	          Set_Menu_Page(MENU_1,WINDOW_1,hproc,2001,true);
		  break;
	      default:
	         break;
	      	 
	      }// end case 
	   }// end MENU_1
		     
      }//end if
      else{
        sleep(1);
	filestat=!filestat;
	Set_Menu_Page(MENU_1,WINDOW_1,hproc,1001,filestat);
	Draw_Text(CANVAS_1,WINDOW_1,hproc,10,10,(char *)(
	 (filestat)?"File true":"File false"));
       }
    }// end while  
  Delete_Window(WINDOW_1,hproc);
  puts("\n sender5: Done \n\n");
  return 0;
 }
  
  
  
