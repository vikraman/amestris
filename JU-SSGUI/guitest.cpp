#include "guiprocess.h"
#include "stdio.h"
#include "string.h"
#include "readresource.cpp"

#define WINDOW_4 14 
#define CANVAS_4 100
#define MENU_4   500

main()
{
MenuHeader *pMH;
MenuItem *pMI;
char *LabelList;
InitGuiSys(10);
InitSystemSettings();
CreateWindow(WINDOW_4,SUPERVISOR_LEVEL,20,20,350,180,"Window IV",4);
if(!ReadMenu("sample.md",&pMH,&pMI,&LabelList))
	if(!CreateMenu(MENU_4,WINDOW_4,23,34,347,18,pMH,pMI))
 		gl_printf(0,400,"Couldnot create Menu");
		
 CreateCanvas(CANVAS_4,WINDOW_4,25,55,335,135);

ShowAllWindows();

Run_Window_System() ;
ShutDownGuiSys();
delete pMH;
delete pMI;
delete LabelList;
return 0;
}
     

