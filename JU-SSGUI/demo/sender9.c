#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25
#define CANVAS_1 399
#define CANVAS_2 400
#define  MENU_1 499

main ()
{
  unsigned c = 0;
  unsigned d = 0;
  char Buffer[40];
  struct smt gm;
  pid_t hproc = GUILogIn ();
  int count = 200;
  int i, j, x, y, y1 = 30;
  char *s[]={ "1 : PARAM1",
	   "2 : PARAM2",
	   "3 : PARAM3",
	   "4 : PARAM4",
	   "5 : PARAM5",
	   "6 : PARAM6",
	   "7 : PARAM7",
	   "8 : PARAM8",
	   "9 : PARAM9",
	   "10: PARAM10",
	  };	
  Create_Window (WINDOW_1, 0, hproc, 0, 0, 639, 470);
  Create_Canvas (CANVAS_1, WINDOW_1, hproc, 5, 45, 498, 460);
  Create_Canvas (CANVAS_2, WINDOW_1, hproc, 503, 45, 135, 460);
  Draw_Text(CANVAS_2,WINDOW_1,hproc,10,10,"sender9");
  for(j = 0; j <10 ; j ++)
  {
  Draw_Text (CANVAS_2, WINDOW_1, hproc, 10, y1, s[j]);
  y1 = y1 + 30;
  } 	
  Create_Menu (MENU_1, WINDOW_1, hproc, 5, 15, 630, 20, "sample3.md");


  Show_Window (WINDOW_1, hproc);

  y = 35;
  while (1)
    {
      x = 30;
      for (i = 0; i < 10; i++)
      {
      if (GUIOutput (GMChannel, hproc, &gm))
	{
	  if (gm.C == SS_DESTROYWINDOW&&gm.A==WINDOW_1&&gm.type==SYSTEM_EVENT)
	  {
		            Delete_Window(WINDOW_1,hproc) ;
			    Shutdown_GUI() ;
			    return 0 ;
	  };

	   if (gm.B == MENU_1&&gm.A==WINDOW_1&&gm.type==SYSTEM_EVENT)
	     {
	      sprintf (Buffer, " Menu returns value %d ", gm.C);
	      Draw_Bar(CANVAS_1,WINDOW_1,hproc,10,350,200,30,30) ;
	      Draw_Text (CANVAS_1, WINDOW_1, hproc, 10, 350, Buffer);
	      if(gm.C==17&&gm.A==WINDOW_1&&gm.type==SYSTEM_EVENT){
		            Delete_Window(WINDOW_1,hproc) ;
			    Shutdown_GUI() ;
			    return 0 ;
	      }  
	     } 
	  }
      else
	  c = 1 + (int) ((10.0 * rand ()) / (RAND_MAX + 1.0));
	  d = 20 * c;
          if(d< 170){
	  Draw_Bar (CANVAS_1, WINDOW_1, hproc, x, y, 20, 200, i + 1);
	  Draw_Bar (CANVAS_1, WINDOW_1, hproc, x, y, 20, d, 30);
	  }
	  sprintf (Buffer, "%1d", i);
	  Draw_Text (CANVAS_1, WINDOW_1, hproc, x + 5, 245, Buffer);
	  x = x + 40;
	  
	}
   }
Delete_Window (WINDOW_1, hproc);
puts ("\n sender9: Done \n\n");
return 0;
}
