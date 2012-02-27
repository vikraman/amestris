#include "message.h"
#include "mesg.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_1 25
#define CANVAS_1 399
#define CANVAS_2 400
//#define BUTTON   300
#define  MENU_1 499
#define  RED_COLOR 4
#define  CANVAS_COLOR 30 
  pid_t hproc = GUILogIn ();
  int count = 200;
  int i, j, x, k, y;
void write_text(char );
main ()
{
  unsigned c = 0;
  struct smt gm;

  Create_Window (WINDOW_1, 0, hproc, 0, 0, 639, 479);
  Create_ScrollableCanvas (CANVAS_2, WINDOW_1, hproc, 12, 398, 610, 80, 610,
			   478);
  Create_Canvas (CANVAS_1, WINDOW_1, hproc, 12, 45, 610, 350);
  Set_Canvas_Key_State_Sc(CANVAS_2, WINDOW_1, hproc,1);
  Set_Canvas_Key_State(CANVAS_1, WINDOW_1, hproc,1);
  Draw_Text (CANVAS_1, WINDOW_1, hproc, 10, 30, "sender8-LC");

  		Show_Window (WINDOW_1, hproc);
  write_text("hello");

    }

void write_text(char buff[])
{
	
  char Buffer[40],m[]="abcd";
  x = 100;
  y = 100;
  Set_Text_Colors_Sc(CANVAS_2,WINDOW_1,hproc,3,1);
  sprintf (Buffer, " %s",buff[20]);
  Draw_TextSc (CANVAS_2, WINDOW_1, hproc, x + 1, y + 1, Buffer);
}	
