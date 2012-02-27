#include"request.h"
#include<string.h>
#define WINDOW_1 100
#define CANVAS_1 200
#define WINDOW_WD 600
#define WINDOW_HT 400
#define CANVAS_WD WINDOW_WD-10
#define CANVAS_HT WINDOW_HT-60
#define MAXLINES 16

main(){
  int i;
  int count;
  char buffer[256];
  struct smt gm; 
  pid_t hproc=GUILogIn();
  if(!Create_Window(WINDOW_1,0,hproc,0,0,WINDOW_WD,WINDOW_HT)){
	  printf("\n Couldnot create window");
	  return -1;
  }
  if(!Create_Canvas(CANVAS_1,WINDOW_1,hproc,5,21,CANVAS_WD,CANVAS_HT)){
	  printf("\n Couldnot create canvas");
	  Delete_Window(WINDOW_1,hproc);
	  return -1;
  }
  Set_Text_Colors(CANVAS_1,WINDOW_1,hproc,1,30,17);
  count=0;
  Show_Window(WINDOW_1,hproc);
  while(1){
         
       sprintf(buffer,"Count is %d",++count);
       if(count>=MAXLINES){
	       Shift_Up_By(CANVAS_1,WINDOW_1,hproc,20,30);
	       Draw_Text(CANVAS_1,WINDOW_1,hproc,10,MAXLINES*20-20,buffer);
       }
       else
	       Draw_Text(CANVAS_1,WINDOW_1,hproc,10,count*20,buffer);
	       
       if(GUIOutput(GMChannel,hproc,&gm)){
		       if(gm.C==SS_DESTROYWINDOW)
			       break;
       }
  }// end while
  Delete_Window(WINDOW_1,hproc);
  return 0;
}
  
		  

