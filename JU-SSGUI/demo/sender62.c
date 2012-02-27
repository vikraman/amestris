#include "mesg.h"
#include "message.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#define WINDOW_2 200
#define  CANVAS_1 200

main()
{ 
  char Buffer[40];
  int count =0,c,i,x;
  int backcolor,y=0;
  struct smt gm;
  unsigned char sussanna[3000]={'\0'};
  char buff[50]=" ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char str[50];
  FILE *fp;
  pid_t hproc=GUILogIn();
  printf("\n sender2: Sending request %u",c++);
  fp=fopen("cover.jpg","rb");
  Create_Window(WINDOW_2,0,hproc,0,0,639,480);  
  Create_Canvas(CANVAS_1,WINDOW_2,hproc,5,15,635,470,15);
  Set_Canvas_Key_State(CANVAS_1,WINDOW_2,hproc,1);
      i = 0;

 while ((c = fgetc (fp)) != EOF) {
/*to prevent picture aliasing, use random():*/
 if(random() & 1) {
         sussanna[i++] = c & 0xf0;
         sussanna[i++] = (c >> 4) & 0x0f;
   } else {
        sussanna[i++] = (c >> 4) & 0x0f;
         sussanna[i++] = c & 0xf0;
     }
   }
  fclose (fp);
	      
  
  y=10;
for(x=0,y=0;y<=480;x++)
{
	Draw_Pixel(CANVAS_1,WINDOW_2,hproc,x,y,sussanna[x]);
	if(x==640)
	{	
		x=0;
		y++;
	}	
}	
  Show_Window(WINDOW_2,hproc);
 }
