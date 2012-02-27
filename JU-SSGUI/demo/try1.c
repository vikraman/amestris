#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 200
#define  CANVAS_1 100

main()
{ 
  struct smt gm;
  pid_t hproc=GUILogIn();
  
  if(!Create_Window(WINDOW_1,0,hproc,60,60,350,200)){
	   printf("\n Couldn't create window ");
	   exit(-1);
  }
  printf("\n Request for canvas creation ");
  if(!Create_Canvas(CANVAS_1,WINDOW_1,hproc,65,75,340,180))
  {
	  printf("\n Couldn't create canvas ");
	  exit(-1);
  }
  printf("\n Request for window show ");
  Set_Canvas_Key_State(CANVAS_1,WINDOW_1,hproc,1);
  Show_Window(WINDOW_1,hproc);

  while(1)
  {
  	if(GUIOutput(GMChannel,hproc,&gm))
	{
		printf("\nMessage gm.type %d gm.A %d gm.B %d gm.C %d gm.D %d\n",gm.type,gm.A,gm.B,gm.C,gm.D);
	}
  }
  }
  
