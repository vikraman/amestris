#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25
#define CANVAS_1 399

main ()
{
  unsigned c = 0;
  unsigned d = 0;
  char Buffer[40];
  struct smt gm;
  pid_t hproc = GUILogIn ();
  Create_Window (WINDOW_1, 0, hproc, 0, 0, 640, 480,0);
  Create_Canvas (CANVAS_1, WINDOW_1, hproc, 0, 0, 640, 480,255);
  Draw_Line(CANVAS_1,WINDOW_1,hproc,320,153,320,400,49);
  Draw_Line(CANVAS_1,WINDOW_1,hproc,0,400,640,400,49);
  Draw_Circle(CANVAS_1,WINDOW_1,hproc,320,480,327,12);
  Draw_Bar(CANVAS_1,WINDOW_1,hproc,0,401,604,78,255);
  Show_Window (WINDOW_1, hproc);
return 0;
}
