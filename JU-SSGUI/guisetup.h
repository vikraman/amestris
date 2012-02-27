#ifndef GUISETUP_H
#define GUISETUP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include"constants.h"
#include"message.h"
#include"event.h"
#include"vgasetup.h"
#include "processinput.h"
#include "keyboardproc.h"
#define GUI_SCREEN_MODE 10
extern SystemInput GuiRequest;
extern unsigned ProgRun,FreezeWindow;
extern GraphicsContext *GuiScreenContext ;
extern unsigned GE ;
extern int InitGuiSys(unsigned modeno);
extern void ShutDownGuiSys(void);
extern void GetMessage(Message *pM);
#ifdef __cplusplus
}
#endif
#endif

