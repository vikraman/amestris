#ifndef BOX_H
#define BOX_H

#include"vga.h"
#include"vgagl.h"

#ifdef __cplusplus 
 extern "C"
{
#endif

void drawbox(int x,int y,int w,int h,int c);
void drawNOTbox(int x,int y,int w,int h);
void draw3doutbox(int x,int y,int w,int h);
void draw3dinbox(int x,int y,int w,int h);
void drawdashedbox(int x,int y,int w, int h,int c);
void draw3dinline(int x1,int y1,int x2,int y2);
#ifdef __cplusplus
}
#endif

#endif
