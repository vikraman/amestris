#include "vgasetup.h"

vga_modeinfo *CurrentModeInfo;
GraphicsContext *GuiScreenContext ;
unsigned  CurrentModeNo;

int InitGraphicsSys(unsigned modeno)
{
vga_init();
 vga_runinbackground(1);
 vga_lockvc(); 
 if(!vga_hasmode(modeno))
  return -1;
 if(vga_setmode(modeno))
  return -2;
 CurrentModeNo=modeno;
 CurrentModeInfo=vga_getmodeinfo(CurrentModeNo); 
 gl_setcontextvga(CurrentModeNo);
 gl_enableclipping();
 gl_setwritemode(FONT_COMPRESSED);
 gl_setfont(8,8,gl_font8x8);
 gl_setfontcolors(0,34);
 GuiScreenContext=gl_allocatecontext() ;
 gl_getcontext(GuiScreenContext);
 //?gl_setcontext(GuiScreenContext);
 vga_unlockvc();

return 0;
}

void ShutDownGraphicsSys(void)
{
 vga_lockvc();
 vga_setmode(TEXT);
 CurrentModeNo=TEXT;
 CurrentModeInfo=NULL;
 vga_unlockvc();
 
 return ;
}
