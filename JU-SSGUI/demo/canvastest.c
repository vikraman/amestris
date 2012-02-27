#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25
#define CANVAS_1 400 
#define CANVAS_2 401
#define CANVAS_3 402 
#define CANVAS_4 403
#define  MENU_1 499
#define C_WIDTH 310
#define C_HEIGHT 205
main ()
{
  unsigned c = 0;
  unsigned d = 0;
  char Buffer[40];
  struct smt gm;
  unsigned count,flag;
  count=flag=0;
pid_t hproc = GUILogIn ();
Create_Window(WINDOW_1,0,hproc,0,0,638,478);
Show_Window(WINDOW_1,hproc);
sleep(1);
Create_Canvas(CANVAS_1,WINDOW_1,hproc,5,15,C_WIDTH,C_HEIGHT);
sleep(1);
Create_Canvas(CANVAS_2,WINDOW_1,hproc,320,15,C_WIDTH,C_HEIGHT);
sleep(1);
Create_Canvas(CANVAS_3,WINDOW_1,hproc,5,235,C_WIDTH,C_HEIGHT);
sleep(1);
Create_ScrollableCanvas(CANVAS_4,WINDOW_1,hproc,320,235,C_WIDTH,C_HEIGHT,2*C_WIDTH,2*C_HEIGHT);
sleep(1);
sleep(1);
sleep(1);
while(1){
    count ++;
	Draw_Circle(CANVAS_1,WINDOW_1,hproc,
			rand()%C_WIDTH,rand()%C_HEIGHT,1,rand()%256);
	Draw_Line(CANVAS_2,WINDOW_1,hproc,rand()%C_WIDTH,rand()%C_HEIGHT,
			rand()%C_WIDTH,rand()%C_HEIGHT,rand()%256);
	Draw_Circle(CANVAS_3,WINDOW_1,hproc,rand()%C_WIDTH,rand()%C_HEIGHT,
			rand()%C_HEIGHT,rand()%256);
	Draw_BarSc(CANVAS_4,WINDOW_1,hproc,50,50,
			rand()%C_WIDTH,rand()%C_HEIGHT,rand()%256);
	if(GUIOutput(GMChannel,hproc,&gm)&&gm.C==SS_DESTROYWINDOW)
		break;
	/*
        if(count==250){
		count=1;
		if(flag){
			flag=0;
			Hide_Window(WINDOW_1,hproc);
		}
		else{
			flag=1;
			Show_Window(WINDOW_1,hproc);
		}
	}
           */
}
Delete_Window (WINDOW_1, hproc);
puts ("\n canvastest  Done \n\n");
return 0;
}
