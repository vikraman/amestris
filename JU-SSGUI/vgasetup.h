#ifndef VGASETUP_H
#define VGASETUP_H

#include "vga.h"
#include "vgagl.h"
#include "stdio.h"

#define SCREEN_MAXX CurrentModeInfo->width
#define SCREEN_MAXY CurrentModeInfo->height
#define SCREEN_MAXCOLORS CurrentModeInfo->colors
#define SCREEN_BYTESPERPIXEL CurrentModeInfo->bytesperpixel
#define SCREEN_VIDEOMEMORY CurrentModeInfo->memory

extern vga_modeinfo *CurrentModeInfo;
extern unsigned CurrentModeNo;
extern GraphicsContext *GuiScreenContext ;
extern int InitGraphicsSys(unsigned modeno);
extern void ShutDownGraphicsSys(void);

#endif  

