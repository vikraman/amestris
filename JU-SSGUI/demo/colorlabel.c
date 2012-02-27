#include"request.h"
#include "message.h"
#include "stdio.h"
#define WINDOW_1 100
#define CANVAS_1 100

#define IN_COLOR1 25
#define IN_COLOR2 27
#define OUT_COLOR1 245
char *labels[]={ "AAAA","BBBB","CCCC","DDDD","EEEE","FFFF"};

void display_3d_status(unsigned canvas,unsigned window,unsigned hproc){
 int i,j;
 int x=20,y=20,w=70,h=23;
 for(j=0;j<6;j++,x+=w){
	 i=rand()%255; 
	 if(x>300){
		x= 20;y+=h+10;
	 }	
		
	 Draw_Line(canvas,window,hproc,x,y,x+w-1,y,IN_COLOR1); // out top
	 Draw_Line(canvas,window,hproc,x+1,y+1,x+w-3,y+1,IN_COLOR2);// in top
	 Draw_Line(canvas,window,hproc,x,y+h-1,x+w-1,y+h-1,OUT_COLOR1);//out bottom
	 Draw_Line(canvas,window,hproc,x+1,y+h-2,x+w-2,y+h-2,OUT_COLOR1);// in bottom
	 Draw_Line(canvas,window,hproc,x,y,x,y+h-1,IN_COLOR1); //out left
	 Draw_Line(canvas,window,hproc,x+1,y,x+1,y+h-2,IN_COLOR2);// in left
	 Draw_Line(canvas,window,hproc,x+w-1,y,x+w-1,y+h-1,OUT_COLOR1);// out right
	 Draw_Line(canvas,window,hproc,x+w-2,y+1,x+w-2,y+h-2,OUT_COLOR1);// in right
	 Draw_Bar(canvas,window,hproc,x+2,y+2,w-4,h-4,i);
	 Set_Text_Colors(canvas,window,hproc,255-i,i,17);
	 Draw_Text(canvas,window,hproc,x+6,y+6,labels[j]);
      }
}

main(){
  int i;
  pid_t hproc=GUILogIn();
  struct smt gm;
  if((i=Create_Window(WINDOW_1,0,hproc,100,100,400,300))==SS_FAILURE){
	   puts("Could not create widow");
	   return -1;
  }
  if((i=Create_Canvas(CANVAS_1,WINDOW_1,hproc,105,135,390,200))==SS_FAILURE){
	  puts("Could not create canvas");
	  return -2;
  }
	
  Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,0,390,280,173);
  Show_Window(WINDOW_1,hproc);
  while(1){
		 if(GUIOutput(GMChannel,hproc,&gm)){
				if(gm.C==SS_DESTROYWINDOW ){
						break;
				}
		 }
		 sleep(1);
		 display_3d_status(CANVAS_1,WINDOW_1,hproc);
  }
  Delete_Window(WINDOW_1,hproc);
  return 0;
}
          		 

