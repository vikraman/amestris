/* Contains the routines that manage requests from the clients,
   and instructs the Windowing System i.e. SS_WindowStructure 
   to act accordingly
   
   Handles the requests  from clients
   and provides the necessary feedback id any 
   */
   
#ifndef GUIPROCESS_H
#define GUIPROCESS_H
#include"winobject.h" 
#include"menu.h"
#include "table.h" 
#define SUPERVISOR_LEVEL 0
#define font0 gl_font8x8

void InitSystemSettings(void);
short AddWindowObject(Win_Objects *pO);
void Run_Window_System(void) ;
short CreateWindow(unsigned id,unsigned pid,int x,int y,int w,int h,char * text,unsigned processid,int tilte,int bc=0, int bdc=0,int tbc=0,int ttc=0);
short ShowWindow(unsigned Id);
short DeleteWindow(unsigned Id);
short DeleteComponent(unsigned Id,unsigned Pid);
short CreateButton(int Id,int Pid,int x,int y,int w,int h,char *text,
                                int bc=0,int bdc=0,int ac=0,int ic=0);

short CreateCanvas(unsigned Id,unsigned Pid,int x,int y,int w,int h,int procid,int scrnc =0,int scrbdc =0);
short CreateScrollableCanvas(unsigned Id,unsigned Pid,int x,int y,int w,int h,int vw,int vh,int procid,int scrnc =0,int scrbdc =0);

short CreateEditBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text);
short CreatePasswordBox(unsigned id,unsigned pid,int x,int y,int w,int h,
char *text);
short CreateLabel(unsigned id,unsigned pid,int x,int y,int w,int h,char *text,int bc=0 , int fc=0 , unsigned char *fname=NULL);
short CreateCheckBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text);
short GetCheckBoxStatus(unsigned id,unsigned pid);
short CreateTableEditor(unsigned Id,unsigned Pid,int x,int y,char *filename );
short SaveTableEditor(unsigned Id,unsigned Pid,char *filename) ;
short CreateMenu(unsigned Id,unsigned Pid,int x,int y,int w,int h,char * menufile);
void DrawBar(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid);
void DrawLine(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid);
void DrawBarSc(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid);
void DrawLineSc(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid);

void ReceiveText(int channel ,unsigned procid , char *buffer, int length) ;
void DrawPixel(unsigned Id,unsigned Pid,int x,int y,unsigned color,unsigned kid);
void DrawPixelSc(unsigned Id,unsigned Pid,int x,int y,unsigned color,unsigned kid);
void DrawCircle(unsigned Id,unsigned Pid,int x,int y,int r ,unsigned color,unsigned kid);
void DrawCircleSc(unsigned Id,unsigned Pid,int x,int y,int r ,unsigned color,unsigned kid);
void SetKeyState(unsigned Id,unsigned Pid,unsigned kstate,unsigned kid);
void SetKeyStateSc(unsigned Id,unsigned Pid,unsigned kstate,unsigned kid);

void TextWrite(unsigned Id,unsigned Pid,int x,int y,char *text,unsigned kid);
void TextWriteSc(unsigned Id,unsigned Pid,int x,int y,char *text,unsigned kid);
void WriteText(unsigned Id,unsigned Pid,int x,int y,int fc,int bc,int fno,char *text,unsigned kid);
short FindWindowIn(int x,int y);
void SendMessage(unsigned Id,Message *WinM);
void SendMessageToCurrentWindow(Message *WinM);
void ProcessKeyboard(Message *WinM);
void ProcessMouse(Message *WinM,int x,int y);
void ShowAllWindows(void);
void SetWindowTitle(unsigned Id,char *text);
void ServeGUIRequest(void);
#endif   
