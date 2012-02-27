#include<stdio.h>
#include<stdlib.h>

#include "request.h"
#define WINDOW_1 100
#define CANVAS_1 200
#define CANVAS_2 201
void Button_Display(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i);
  pid_t hproc1=GUILogIn();
  
main()
{
	int i,j,k;
	char str[40]={'\0'};
	
	Create_Window(WINDOW_1,0,hproc1,0,0,640,479,1);
	Create_Canvas(CANVAS_1,WINDOW_1,hproc1,5,15,320,410,29);
	Show_Window(WINDOW_1,hproc1);
	while(1)
	{	
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,20,107,60,"hello",10);
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,90,107,60,"hello1",10);
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,160,107,60,"hello2",10);
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,230,107,60,"hello3",10);
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,300,107,60,"hello4",10);
        Button_Display(CANVAS_1,WINDOW_1,hproc1,20,370,107,60,"hello5",10);
	}
}




void Button_Display(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i)
{
	Set_Text_Colors(canvas,window,hproc,255,i,21);
		Draw_Bar(canvas,window,hproc,x,y,w-1,h-1,i);
		Draw_Text(canvas,window,hproc,x+5,y+5,text);
		usleep(50000);
		Draw_Bar(canvas,window,hproc,x+5,y+5,w-6,20,i);
		Draw_Bar(canvas,window,hproc,x+5,y+5,w-6,20,i);
		usleep(5000);
		Draw_Bar(canvas,window,hproc,x+5,y+5,w-6,20,i);
}

