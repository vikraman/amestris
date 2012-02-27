#include "box.h"
#define IN_COLOR1 25
#define IN_COLOR2 27 
#define OUT_COLOR1 245 

void drawbox(int x,int y,int w,int h,int c)
{
  gl_hline(x,y,x+w-1,c);
  gl_hline(x,y+h-1,x+w-1,c);
  gl_line(x,y,x,y+h-1,c);
  gl_line(x+w-1,y,x+w-1,y+h-1,c);
  return;
}

void drawNOTbox(int x,int y,int w,int h)
{
 int i,j;
 // draw the vertical part
 for(i=x+w-1,j=h;j>=0;j--)
   {
    gl_setpixel(x,y+j,0xFF&~gl_getpixel(x,y+j));
    gl_setpixel(i,y+j,0xFF&~gl_getpixel(i,y+j));
   }
// draw the horizontal part 
for(i=w,j=y+h-1;i>=0;i--)
  {
   gl_setpixel(x+i,y,~gl_getpixel(x+i,y));
   gl_setpixel(x+i,j,~gl_getpixel(x+i,j));
  }  
return;
}
 
void draw3doutbox(int x,int y,int w,int h)
{
  gl_hline(x,y,x+w-1,IN_COLOR1); // out top
  gl_hline(x+1,y+1,x+w-3,IN_COLOR2);// in top
  gl_hline(x,y+h-1,x+w-1,OUT_COLOR1);//out bottom
  gl_hline(x+1,y+h-2,x+w-2,OUT_COLOR1);// in bottom
  gl_line(x,y,x,y+h-1,IN_COLOR1); //out left
  gl_line(x+1,y,x+1,y+h-2,IN_COLOR2);// in left 
  gl_line(x+w-1,y,x+w-1,y+h-1,OUT_COLOR1);// out right 
  gl_line(x+w-2,y+1,x+w-2,y+h-2,OUT_COLOR1);// in right
  return;
}

void draw3dinbox(int x,int y,int w,int h)
{
  gl_hline(x,y,x+w-1,OUT_COLOR1); // out top
  gl_hline(x+1,y+1,x+w-3,OUT_COLOR1); // in top
  gl_hline(x,y+h-1,x+w-1,IN_COLOR1); // out bottom
  gl_hline(x+1,y+h-2,x+w-2,IN_COLOR2); // in bottom
  gl_line(x,y,x,y+h-1,OUT_COLOR1); // out left
  gl_line(x+1,y,x+1,y+h-2,OUT_COLOR1); // in left
  gl_line(x+w-1,y,x+w-1,y+h-1,IN_COLOR1);// out right
  gl_line(x+w-2,y+1,x+w-2,y+h-2,IN_COLOR2);// in right 
  return;
}

void drawdashedbox(int x,int y,int w, int h,int c){
  int i;
  for(i=x;i+5<x+w-1;i+=8)
    gl_hline(i,y,i+5,c);
  for(i=x+w-1;i-5>x;i-=8)
    gl_hline(i-5,y+h-1,i,c);
  for(i=y;i+5<y+h-1;i+=8)
   gl_line(x+w-1,i,x+w-1,i+5,c);
  for(i=y+h-1;i-5>y;i-=8)
   gl_line(x,i-5,x,i,c);
  gl_hline(x+w-3,y,x+w-1,c);
  gl_hline(x,y+h-1,x+2,c);
  gl_line(x+w-1,y+h-3,x+w-1,y+h-1,c);
  gl_line(x,y,x,y+2,c);
 return;
 }

 void draw3dinline(int x1,int y1,int x2,int y2){
   gl_line(x1,y1,x2,y2,IN_COLOR2);
   gl_line(x1,y1+1,x2,y2+1,IN_COLOR1);
 
 }
