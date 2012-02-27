#include <stdio.h>
#include <string.h>
#include "vga.h"
#include "vgagl.h"
#include "font.h"
#include "font_bdf.c"

#define TABSIZE  64   /* size of a tab, in pixels */
unsigned CurrentFontColor;
struct fontinfo_tag *pCurrentFont=&fontcour12;

void set_fontcolor(unsigned color){
   CurrentFontColor=color;
   return;
   }

void set_font(int fontno){
  switch(fontno){
   	       case 0: pCurrentFont=&fonttimesR10;break;
               case 1: pCurrentFont=&fonttimesR12;break;
               case 2: pCurrentFont=&fonttimesR14;break;
               case 3: pCurrentFont=&fonttimesR16;break;
               case 4: pCurrentFont=&fonttimesR20;break;
               case 5: pCurrentFont=&fonttimesR24;break;

               case 6: pCurrentFont=&fonttimesI10;break;
               case 7: pCurrentFont=&fonttimesI12;break;
               case 8: pCurrentFont=&fonttimesI14;break;
               case 9: pCurrentFont=&fonttimesI16;break;
               case 10: pCurrentFont=&fonttimesI20;break;
               case 11: pCurrentFont=&fonttimesI24;break;

               case 12: pCurrentFont=&fontcour10;break;
               case 13: pCurrentFont=&fontcour12;break;
               case 14: pCurrentFont=&fontcour16;break;
               case 15: pCurrentFont=&fontcour20;break;
               case 16: pCurrentFont=&fontcour24;break;

               case 17: pCurrentFont=&fontariali10;break;
               case 18: pCurrentFont=&fontariali14;break;
               case 19: pCurrentFont=&fontariali16;break;
               case 20: pCurrentFont=&fontariali24;break;
 
               case 21: pCurrentFont=&fontimpact12;break;
               case 22: pCurrentFont=&fontimpact16;break;
               case 23: pCurrentFont=&fontimpact20;break;
               case 24: pCurrentFont=&fontimpact24;break;

               case 25: pCurrentFont=&fontArialn12;break;
               case 26: pCurrentFont=&fontArialn14;break;
               case 27: pCurrentFont=&fontArialn16;break;
               case 28: pCurrentFont=&fontArialn20;break;
               case 29: pCurrentFont=&fontArialn24;break;
	       default:break;	
               }
    return;
}

void draw_text(int xpos,int ypos,char *str,short back,int backcolor,
															int *pw,int *ph)
{
int f,c,x,y;
int initxpos=xpos;
signed char *ptr,*fontdat;
int ox,oy,w,h,dw;
int yst;
int xrunst=-1;
int *fonttbl;
int fontyofs,fontfh,fontoy;
int widthadd=0;
int origxpos=xpos;	
int constcalc;
*pw=0;*ph=0;
fontdat=pCurrentFont->data;
fonttbl=pCurrentFont->table;
fontyofs=pCurrentFont->yofs;
fontfh=pCurrentFont->fh;
fontoy=pCurrentFont->oy;
constcalc=fontfh+fontoy-fontyofs;
*pw=xpos;
for(f=0;f<strlen(str);f++){
  c=str[f];
  if(c=='\t'){
    xpos=((xpos/TABSIZE)+1)*TABSIZE;
    continue;
    }
  if(c<32 || c>126) c='_';
  ptr=&fontdat[fonttbl[c-32]];
  ox=*ptr++; oy=*ptr++;
  w =*ptr++; h =*ptr++;
  dw=*ptr++;
  
 yst=constcalc-oy-h;
 if(*ph<h+yst)
	 *ph=h+yst;
  if(back)	// on opaque text
   gl_fillbox(xpos,ypos,w+ox,h+yst,backcolor); 
  if(c!='\t')
    for(y=ypos+yst;y<ypos+yst+h;y++){            //scan entire height
     for(x=xpos+ox;x<xpos+ox+w;x++)
          if(*ptr++){
	     if(xrunst==-1) 
	        xrunst=x;
	      }
          else
            if(xrunst!=-1){
              if(xrunst==x-1)
                gl_setpixel(xrunst,y,CurrentFontColor);
              else
                gl_hline(xrunst,y,x-1,CurrentFontColor);
              xrunst=-1;
              }
        
        if(xrunst!=-1){
          if(xrunst==x-1)
            gl_setpixel(xrunst,y,CurrentFontColor);
          else
            gl_hline(xrunst,y,x-1,CurrentFontColor);
          }
        xrunst=-1;
        }
  	
  xpos+=dw+widthadd;
  }
*pw=xpos-*pw;
return;
}

