#ifndef FONT_H
#define FONT_H
#ifdef __cplusplus
 extern "C" {
#endif
  extern void draw_text(int x,int y,char *str,short back,int backcolor,int *w,int *h);
  extern void set_font(int fontno);
  extern  void set_fontcolor(unsigned color);

#ifdef __cplusplus
 } 
#endif

#endif
