#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 14 
#define  CANVAS_1 213
#define  BUTTON_1 313
#define  BUTTON_2 314
#define  EDITBOX_1 714
#define  CHECKBOX_1 315
#define  LABEL_1 316
main()
{ 
   int prev=0 ;
   unsigned c=0;
  char Buffer[256];
  short cnt,i;
  struct smt gm;
  int x=0,y=0;
  pid_t hproc=GUILogIn();
  int count =200 ;
  bool CheckBoxState=true;
  bool ButtonState=true;
  Create_Window(WINDOW_1,0,hproc,210,100,350,300);  
  Create_Canvas(CANVAS_1,WINDOW_1,hproc,215,115,240,180);
  Create_PasswordBox(EDITBOX_1,WINDOW_1,hproc,220,300,240,30);
  Set_Text_EditBox(EDITBOX_1,WINDOW_1,hproc,"Initial  Text ");
  Create_Button(BUTTON_1,WINDOW_1,hproc,222,350,120,25,"State");
  Create_Button(BUTTON_2,WINDOW_1,hproc,400,350,120,25,"State");
  Create_CheckBox(CHECKBOX_1,WINDOW_1,hproc,460,225,90,30,"CheckBox");
  Create_Label(LABEL_1,WINDOW_1,hproc,460,125,50,50,"Label");
  Set_CheckBox(CHECKBOX_1,WINDOW_1,hproc,TRUE);
  Show_Window(WINDOW_1,hproc);
  Set_Text_Colors(CANVAS_1,WINDOW_1,hproc,255,30,35);
  while(1){
if(GUIOutput(GMChannel,hproc,&gm)){
        printf(" \n sender4 %d %d %d %d %d ",gm.type,gm.A,gm.B ,gm.C,gm.D);
        if(gm.C==SS_DESTROYWINDOW)
          break;
	else if(gm.C==SS_COMPONENT_USED&&gm.B==CHECKBOX_1){
	      if(ButtonState)
	        ButtonState=false;
	      else
	        ButtonState=true;
	      Set_Component_State(BUTTON_1,WINDOW_1,hproc,ButtonState);
	    }
  	    if(gm.type==SS_INPUT){
			  	sprintf(Buffer,"%c",gm.D);
				Draw_Text(CANVAS_1,WINDOW_1,hproc,x,5,Buffer) ;
				x+=8;
				Draw_Line(CANVAS_1,WINDOW_1,hproc,x+1,6,x+1,10,12);
		}
		if(gm.type==FOCUS_EVENT && gm.B==CANVAS_1  && gm.C == SS_GAINFOCUS){
				Draw_Line(CANVAS_1,WINDOW_1,hproc,x+1,6,x+1,10,12);
 			   Draw_Text(CANVAS_1,WINDOW_1,hproc,10,70,"focus event") ;      
		}
								
  //Get_CheckBox_Status(CHECKBOX_1,WINDOW_1,hproc,&cnt);
  //prev=(int)cnt;
  //Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,50,280,100,30);
  //sprintf(Buffer," CheckBox State is  = %3u",(int)cnt);
  // end if gm.C 
	else if(gm.C==SS_COMPONENT_USED&&gm.B==EDITBOX_1){
  		i= Get_Text(EDITBOX_1,WINDOW_1,hproc,Buffer);
           	Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,50,200,100,30); 
           	Draw_Text(CANVAS_1,WINDOW_1,hproc,10,90,Buffer);
	  }// end if gm.C
	else if(gm.C==SS_COMPONENT_USED&&gm.B==BUTTON_1){
	      Delete_Window_Component(BUTTON_1,WINDOW_1,hproc);
	      sleep(2);
	      Show_Window(WINDOW_1,hproc);
	      } // end if button
         } // end if GUI
        else{
           usleep(1000);/*
						   
           Set_Button_Colors(BUTTON_1,
		WINDOW_1,
		hproc,	
		rand()%25,rand()%25,rand()%25,rand()%25);
            Set_Button_Colors(BUTTON_2,
		WINDOW_1,
		hproc,	
		rand()%25,rand()%25,rand()%25,rand()%25);*/
             }
      }
  Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,50,280,100,30);
  sprintf(Buffer," CheckBox State is  = %3u",prev);
  Draw_Text(CANVAS_1,WINDOW_1,hproc,10,70,Buffer) ; 
  Get_CheckBox_Status(CHECKBOX_1,WINDOW_1,hproc,&cnt);
  printf("\n  CheckBox is %d ",cnt);
  i= Get_Text(EDITBOX_1,WINDOW_1,hproc,Buffer);
  printf("\n Get_Text returns %d ",i) ;
  if(i)
    printf("\n  Text is  %s \n",Buffer);
  
  Delete_Window(WINDOW_1,hproc);
  puts("\n sender4: Done \n\n");
  return 0;
 }
  
