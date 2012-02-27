#include "request.h"
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_1 100
#define WINDOW_2 200

#define EDITBOX_1 201
#define EDITBOX_2 202

#define BUTTON_1 203
#define BUTTON_2 204

#define CHECKBOX_1 205
#define CHECKBOX_2 206

#define LABEL_1 207

#define CANVAS_1 208
#define CANVAS_2 209
//short stat ;
main()
{ 
  	struct smt gm;
	char c,buffer1[200];
	//int ret_val;
	short stat,s1;
	int i;

  	pid_t hproc=GUILogIn();
  	//pid_t hproc2=GUILogIn();

  	Create_Window(WINDOW_1,0,hproc,0,0,310,479);  
  	Create_Window(WINDOW_2,0,hproc,320,0,310,479);  

	Set_Window_Title(WINDOW_1,0,hproc,"W1");
	Set_Window_Title(WINDOW_2,0,hproc,"W2");

	Create_EditBox(EDITBOX_1,WINDOW_1,hproc,30,300,100,50);
	Set_Text_EditBox(EDITBOX_1,WINDOW_1,hproc,"Hello!!!");
	Get_Text(EDITBOX_1,WINDOW_1,hproc,buffer1);
	Create_EditBox(EDITBOX_2,WINDOW_2,hproc,340,300,100,50);

	//Show_Window(WINDOW_1,hproc);

	Set_Text_EditBox(EDITBOX_2,WINDOW_2,hproc,buffer1);
	Show_Window(WINDOW_1,hproc);
  	Show_Window(WINDOW_2,hproc);

	Create_Button(BUTTON_1,WINDOW_1,hproc,30,250,100,30,"ok");	
	Create_Button(BUTTON_2,WINDOW_2,hproc,350,250,100,30,"ok");	
	Set_Text_Button(BUTTON_2,WINDOW_2,hproc,"CANCEL");

	Create_CheckBox(CHECKBOX_1,WINDOW_1,hproc,20,20,120,20,"CHECK1");
	
        Set_CheckBox(CHECKBOX_1,WINDOW_1,hproc,1);
	Get_CheckBox_Status(CHECKBOX_1,WINDOW_1,hproc,&s1);
        stat=s1 ;
        sprintf(buffer1,"Status %3d",int(s1)) ;    
	printf("\n\n\nSudio  %d ",s1) ;

	Create_CheckBox(CHECKBOX_2,WINDOW_2,hproc,350,20,20,20," ");
	Set_CheckBox(CHECKBOX_2,WINDOW_2,hproc,s1);
	Create_Label(LABEL_1,WINDOW_2,hproc,375,20,100,30,"CHECK2");
	

	Create_Canvas(CANVAS_1,WINDOW_1,hproc,20,50,250,100);
	Create_Canvas(CANVAS_2,WINDOW_2,hproc,330,50,250,100);
        Draw_Text(CANVAS_2,WINDOW_2,hproc,20,20,buffer1) ;

	Set_Text_Colors(CANVAS_1,WINDOW_1,hproc,7,1,5);	
	Draw_Text(CANVAS_1,WINDOW_1,hproc,5,5,"PAVAN");
	Draw_Text(CANVAS_2,WINDOW_2,hproc,5,5,"PAVAN");
	
	for(i=5;i<=120;i+=5)
	{
		Draw_Line(CANVAS_1,WINDOW_1,hproc,i,29,i+5,29+i,39);
		usleep(100000);
	}

	Draw_Bar(CANVAS_1,WINDOW_1,hproc,10,30,40,10,109);	
	Draw_Bar(CANVAS_1,WINDOW_1,hproc,10,50,60,10,109);	
	Draw_Bar(CANVAS_1,WINDOW_1,hproc,10,70,30,10,109);	

	for(i=0;i<=40;i++)
	{
		Draw_Circle(CANVAS_2,WINDOW_2,hproc,125,50,i,9);
		usleep(100000);
	}

	Hide_Window(WINDOW_1,hproc);
        sleep(3); 
	Hide_Window(WINDOW_2,hproc);
        sleep(3); 
	Show_Window(WINDOW_1,hproc);
  	Show_Window(WINDOW_2,hproc);
	sleep(3);
	Delete_Window(WINDOW_1,hproc);
  	Delete_Window(WINDOW_2,hproc);
	printf("Buffer1 %s\n",buffer1);
 }
  
  
  
