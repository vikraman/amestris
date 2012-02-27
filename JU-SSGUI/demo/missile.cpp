#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25
#define CANVAS_1 399

void winproc();
pid_t hproc;

main ()
{
	hproc = GUILogIn ();
  
  	Create_Window (WINDOW_1, 0, hproc, 0, 0, 400, 200,1);
  	Create_Canvas (CANVAS_1, WINDOW_1, hproc,0,20,80,80);
  	Show_Window (WINDOW_1, hproc);

	winproc();
	return 0;
}

void winproc()
{
	struct smt gm;
	
	while(1)
	{
		if(GUIOutput(GMChannel,hproc,&gm))
			printf("%d %d %d %d\n",gm.A,gm.B,gm.C,gm.D);
	}
}

