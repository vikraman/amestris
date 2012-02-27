#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <unistd.h>
#include <stdlib.h>
#include "message.h"
#include "requestlist.h"
#define OBJ_WINDOW_ID   1
#define OBJ_CHECKBOX_ID 2
#define OBJ_LABEL_ID    3
#define OBJ_EDITBOX_ID  4
#define OBJ_CANVAS_ID   5
#define OBJ_SCANVAS_ID  6
#define OBJ_BUTTON_ID   7 
#define OBJ_TABLE_ID    8 


 typedef struct {
    unsigned char Request_Id ;
    unsigned short Process_Id ;
    unsigned int Parameters[10] ;
    unsigned int Otherparams[25][8];
    char labels[800];
    }SystemInput ;

 struct smt{
          unsigned type,A,B,C,D;
          };
extern int RequestChannel;
extern int GMChannel;
extern bool CreateRequestQ(int key);
extern bool CreateGOQ(int key);
extern short GUILogIn(void);
//extern smt Guim;

extern void SendGUIRequest(int Channel, SystemInput sr);
extern bool GUIOutput(int Channel,unsigned procid,struct smt *g);
extern bool GetGUIOutput(int Channel,unsigned procid,struct smt *g);


extern void SendText(int channel ,unsigned procid,char *buffer,int length);


/* Window functions */
extern int Create_Window(unsigned id,unsigned pid ,unsigned short procid,
		       int x,int y,int w,int h,
		       int Title=1);
extern void Show_Window(unsigned id, unsigned short procid);
extern void Hide_Window(unsigned id, unsigned short procid);
extern void Set_Window_Title(unsigned id,unsigned pid ,unsigned short procid,char *text);
extern int Delete_Window(unsigned id , unsigned short procid);

 /* Canvas functions */
extern int Create_Canvas(unsigned Id,unsigned Pid,unsigned short procid,
			 int x,int y,int w,int h,
			 int scrnc =0,int scrbdc =0);
extern void Draw_Line(unsigned id,unsigned pic ,unsigned short procid,
			 int x1,int y1,int x2,int y2,
			 int color);
extern void Draw_Bar(unsigned id,unsigned pic ,unsigned short procid,
			 int x,int y,int width,int height,
			 int color);
extern void Draw_Pixel(unsigned id , unsigned pid ,unsigned procid,
			int x,int y,int color);
extern void Draw_Circle(unsigned id , unsigned pid ,unsigned procid,
                        int x,int y,int radius ,int color);
extern void Draw_Text(unsigned id,unsigned pid ,unsigned short procid,
			int x,int y ,char *text);  
extern void Set_Text_Colors(unsigned id,unsigned pid,unsigned short procid ,
     unsigned fc,unsigned bc,unsigned fno=0);
extern void Shift_Up_By(unsigned id,unsigned pid,unsigned short procid,unsigned dy,unsigned color);
extern void Shift_Down_By(unsigned id,unsigned pid,unsigned short procid,unsigned dy,unsigned color);
extern void Shift_Right_By(unsigned id,unsigned pid,unsigned short procid,unsigned dx,unsigned color);
extern void Shift_Left_By(unsigned id,unsigned pid,unsigned short procid,unsigned dx,unsigned color);
extern void Set_Canvas_Key_State(unsigned id,unsigned pid,unsigned short procid,short kstate);

/* Scrollable Canvas Function */

extern int Create_ScrollableCanvas(unsigned Id,unsigned Pid,unsigned short procid,int x,int y,int w,int h, int vw, int vh,int scrnc =0,int scrbdc =0);

extern void Draw_LineSc(unsigned id,unsigned pic ,unsigned short procid,
                         int x1,int y1,int x2,int y2,
                         int color);
extern void Draw_BarSc(unsigned id,unsigned pic ,unsigned short procid,
                         int x,int y,int width,int height,
                         int color);
extern void Draw_PixelSc(unsigned id , unsigned pid ,unsigned procid,
                        int x,int y,int color);
extern void Draw_CircleSc(unsigned id , unsigned pid ,unsigned procid,
                        int x,int y,int radius ,int color);

extern void Draw_TextSc(unsigned id,unsigned pid ,unsigned short procid,
                        int x,int y ,char *text);
extern void Set_Text_Colors_Sc(unsigned id,unsigned pid,unsigned short procid ,
     unsigned fc,unsigned bc);
extern void Set_Canvas_Key_State_Sc(unsigned id,unsigned pid,unsigned short procid,short kstate);
/*TableEditor Function */

extern short Create_Table_Editor(unsigned id , unsigned pid,unsigned short procid,int x, int y, char *rcfilename);
extern short Save_Table_Editor(unsigned id, unsigned pid,unsigned short procid, char *filename);

/* Edit Box functions */
extern int Create_EditBox(unsigned id,unsigned pid,unsigned short procid,
			int x,int y,int w,int h);
extern int Set_Text_EditBox(unsigned id,unsigned pid,unsigned short procid,
		    char * text);
extern int Get_Text(unsigned id,unsigned pid,unsigned short procid,
		    char * Buffer);
extern int Create_PasswordBox(unsigned id,unsigned pid,unsigned short procid,
			int x,int y,int w,int h);

extern int Set_Text_PasswordBox(unsigned id,unsigned pid,unsigned short procid,
		    char * text);
/* Button functions */
extern short Create_Button(unsigned id,unsigned pid,unsigned short procid ,
                           int x,int y , int w, int h,char *text);
extern int Set_Text_Button(unsigned id,unsigned pid,unsigned short procid,
                    char * text);

/* Label functions */
extern short Create_Label(unsigned id,unsigned pid,unsigned short procid , 
                        int x,int y , int w, int h,char *text);
extern int Set_Text_Label(unsigned id,unsigned pid,unsigned short procid,
                    char * text);
extern void Set_Button_Color(int color) ;
/* Check Box functions */
		    
extern short  Create_CheckBox(unsigned id,unsigned pid,unsigned short procid ,
                        int x,int y , int w, int h,char *text);
extern short Get_CheckBox_Status(unsigned id,unsigned pid,unsigned short procid,
			short *pStatus);
extern void Set_CheckBox(unsigned id,unsigned pid,unsigned short procid,
			short Status);
/* Menu Functions */
extern short Create_Menu(unsigned id,unsigned pid,unsigned short procid ,
                        int x,int y , int w, int h,char *filename);
extern void Set_Menu_Item(unsigned id,unsigned pid,unsigned short procid,
			unsigned Value,short State);
extern void Set_Menu_Page(unsigned id,unsigned pid,unsigned short procid,
			unsigned Value,short State);
extern void Set_Button_Colors(unsigned id,unsigned pid,unsigned short procid,
			unsigned FocusColor=0,
			unsigned ButtonColor=0,
                        unsigned ActiveFontColor=0,
			unsigned InactiveFontColor=0);
extern void Set_Component_State(unsigned id,unsigned pid,unsigned short procid,
      short State);
extern short Get_Component_State(unsigned id,unsigned pid,unsigned short procid);
extern short Set_Focus_To(unsigned id,unsigned pid,unsigned short procid);
extern short Delete_Window_Component(unsigned id,unsigned pid,unsigned short procid);
extern unsigned Get_Focussed_Component(unsigned pid,unsigned short procid);
extern short Get_Next_Component_Info(unsigned id,unsigned pid,
		unsigned short procid,unsigned *nextid,unsigned *nexttype);
extern void Shutdown_GUI(void) ;
//extern void Freeze_Window(unsigned short procid) ;
extern void DeFreeze_Window(unsigned short procid) ;
extern void Print_Window(unsigned short procid,int mode ) ;
extern void Write_Text(unsigned id,unsigned pid ,unsigned short procid,
			int x,int y,int fc,int bc,int fno,char *text);  
  
#endif
