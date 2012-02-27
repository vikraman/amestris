/*  This file contains definitions of the various classes and structures that
    have been used to implement the windowing system.
    These have been used in conjuction with the class WinArea defined in 
    winnarea.h to provide the necessary clipping for the window system.*/


#ifndef WINOBJECTS_H
#define WINOBJECTS_H

#include "guisetup.h"
#include "box.h"
#include "wpr.h"
#include "debug.h"
#include<stdlib.h>
#include<string.h>
#include<map>
#define ALLOCATION_ERROR 100
#define NO_ALLOCATION_ERROR 50
#define BACKGROUND_COLOR SS_SystemSettings.BackgroundColor


#define OBJ_WINDOW_ID   1
#define OBJ_CHECKBOX_ID 2
#define OBJ_LABEL_ID    3
#define OBJ_EDITBOX_ID  4
#define OBJ_CANVAS_ID   5
#define OBJ_SCANVAS_ID  6
#define OBJ_BUTTON_ID   7
#define OBJ_TABLE_ID    8

/* The following structure stores the valus for the different colors in the 
   Windowing system */
typedef struct
{
  struct
  {
    int BorderColor, BaseColor, TitleBarColor, TitleFontColor, DeactiveColor;
    unsigned char *TitleFont;
    int TitleFontWidth, TitleFontHeight;
    int TitleBarHeight;
  }
  Window;

  struct
  {
    int ActiveColor, DeactiveColor, InFocusColor, BorderColor, BaseColor;
    unsigned char *LabelFont;
    int LabelFontWidth, LabelFontHeight;
  }
  Button;

  struct
  {
    int CrossColor;
  }
  SystemButton;

  struct
  {
    int TextColor, TextBackgroundColor, CanvasColor, CanvasBorderColor;
    unsigned char *CanvasFont;
    int CanvasFontWidth, CanvasFontHeight;
  }
  Canvas;

  struct
  {
    int TextColor,
      Color,
      HighlightColor, HighlightTextColor, Separator, FontHeight, FontWidth;
    unsigned char *Font;
  }
  Menu;

  unsigned char *DefaultFont;
  int DefaultFontHeight, DefaultFontWidth;
  int InFocusColor;
  int WindowMoveFlag;
  int MenuActiveFlag;
  int RedrawFlag;
  unsigned BackgroundColor;
}
SystemSettings;

extern SystemSettings SS_SystemSettings;



// Most fundamental class
/* this class implements the minimum required concepts to implement a window */
class Rectangle
{
  public:int Left, Right, Top, Bottom, Width, Height;
    public:short IsInside (int x, int y);
    Rectangle ();
    Rectangle (int l, int t, int w, int h);
  virtual void MoveTo (int x, int y);
  virtual void MoveBy (int dx, int dy);
  virtual void SetDimensions (int w, int h);
  virtual void GetDimensions (int *w, int *h);
  void GetOrigin (int *x, int *y);
};				// eoc

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/* defines the base class for the subsequent specialised visual components
   like the  Label,CheckBox,Menu etc */
class Win_Objects:public Rectangle
{
  friend class SuperStructure;
public:
  int BaseColor, BorderColor;
  unsigned type;
  unsigned ID, PID;
  short active, Infocus, display;
  Rectangle ClipArea;
public:
  
    
    Win_Objects (int x, int y, int w, int h, unsigned id, unsigned pid,
		 int bc = 0, int bdc = 0);
  virtual void MoveTo (int x, int y);
  virtual void MoveBy (int dx, int dy);
  virtual void SetDimensions (int w, int h);
  virtual void process (Message * Win_Message)
  {
  };
  virtual void show (void);
  virtual void howto (void)
  {
  };
  unsigned GetParentId (void)
  {
    return PID;
  }
  unsigned GetId (void)
  {
    return ID;
  }
  unsigned GetType (void)
  {
    return type;
  }
  virtual void SetActive (void)
  {
    active = TRUE;
    show ();
  }
  virtual void SetInactive (void)
  {
    active = FALSE;
    show ();
  }
  virtual short Isactive (void)
  {
    return active;
  }
  virtual short LoseFocus (void);
  virtual short GainFocus (void);
  virtual void EnableDisplay (void)
  {
    display = TRUE;
  }
  virtual void DisableDisplay (void)
  {
    display = FALSE;
  }
  virtual ~ Win_Objects ();
};				// eoc


///////////////////////////////////////////////////////////////////////
/* defines the button implementation */
class Button:public Win_Objects
{
protected:
  char Label[80];
  int activefontcolor;
  int FocusColor, aBaseColor;
  int inactivefontcolor;
  int blsx, blsy;
  virtual void animate (void);
    public:Button (unsigned id, unsigned pid, int x, int y, int w, int h,
		   char *text, int bc = 0, int bdc = 0, int ac = 0, int ic =
		   0);
  virtual void SetInactive (void);
  virtual void process (Message * Button_Message);
  virtual short GainFocus (void);
  virtual short LoseFocus (void);
  virtual void SetColors (int fc = 0, int bcolor = 0, int afcolor =
			  0, int iafcolor = 0);
  void SetText (char *text);
  virtual void howto ();
};
///////////////////////////////////////////////////////////////////////
/* defines the cross button implementation --a specialised button */
# define WINDOW_BUTTON_CROSS  777
class SystemButton:public Button
{
  int CrossColor;
    public:SystemButton (unsigned pid, int x, int y);
  void process (Message * Button_Message);
  virtual void howto ();
private:
  void animate (void);
};				// End of class definition

///////////////////////////////////////////////////////////////////////
/* defines Canvas a region for writing,and drawing */
using namespace std;
class Canvas;
typedef std::map<int,Canvas*> CanvasMap;
extern CanvasMap cm;
class Canvas:public Win_Objects
{
protected:
  int TextColor, TextBackgroundColor,FontWidth,FontHeight;
  GraphicsContext *vgc, *cgc;
  int processid;
  unsigned char *Font;
  char *VBuffer;
  int fontno;
  bool keyflow;
  Rectangle DrawArea;
    public:Canvas (unsigned id, unsigned pid, int x, int y, int w, int h,int procid, int scrnc, int scrbdc);
    virtual ~ Canvas ()
  {
    gl_freecontext (vgc);
    gl_freecontext (cgc);
	cm.erase((processid<<16)|ID);
  }
  virtual void howto ();
  virtual void UpdateArea (int l, int t, int r, int b);
  virtual void SetTextColors (int tc = 0, int tbc = 0, int fn = 0);
  virtual void Write (int x, int y,int fc ,int bc ,int fno ,char *text);
  virtual void Write2 (int x, int y, char *text);
  virtual void Line (unsigned x1, unsigned y1, unsigned x2, unsigned y2,
		     unsigned color);
  virtual void Bar (unsigned x1, unsigned y1, unsigned x2, unsigned y2,
		    unsigned color);
  virtual void Circle (unsigned x, unsigned y, unsigned r, unsigned color);
  virtual void Pixel (unsigned x, unsigned y, unsigned color);

  virtual void ShiftUpBy (unsigned dy, unsigned color);
  virtual void ShiftDownBy (unsigned dy, unsigned color);
  virtual void ShiftRightBy (unsigned dx, unsigned color);
  virtual void ShiftLeftBy (unsigned dx, unsigned color);
  virtual void MoveTo (int x, int y);
  virtual void MoveBy (int dx, int dy);
  virtual void process (Message * WinM);
  virtual short GainFocus (void)
  {
  };
  virtual short LoseFocus (void)
  {
  };
  virtual void SetKeyState(bool state){ keyflow=state;}
};				// End of Class

////////////////////////////////////////////////////////////////////
/* Enhanced Canvas but wihtout different Font support */
class ScrollableCanvas;
typedef std::map<int ,ScrollableCanvas*> ScrollableCanvasMap;
extern ScrollableCanvasMap scm;
class ScrollableCanvas:public Win_Objects
{
protected:
  int TextColor, TextBackgroundColor;
  int VirtualTop, VirtualLeft;
  int VirtualWidth, VirtualHeight;
  GraphicsContext *CanvasContext;
  int processid; 
   bool keyflow;
  void *CanvasScreen;
   public:ScrollableCanvas (unsigned id, unsigned pid, int x, int y, int w,
			     int h, int vw, int vh, int procid,int scrnc,
			     int scrbdc):Win_Objects (x, y, w, h, id, pid,
						      scrnc, scrbdc)
  {
    keyflow=false;
	type = OBJ_SCANVAS_ID;
    VirtualTop = VirtualLeft = 0;
    VirtualWidth = (w > vw ? w : vw);
    VirtualHeight = (h > vh ? h : vh);
    CanvasScreen = malloc (VirtualWidth * VirtualHeight);
    // Assuming the virtual screen bpp as 8 bpp
    if (!CanvasScreen)
      GE = ALLOCATION_ERROR;

    else
      {
	GE = NO_ALLOCATION_ERROR;
	gl_setcontextvirtual (VirtualWidth, VirtualHeight, 1, 8,
			      CanvasScreen);
	CanvasContext = gl_allocatecontext ();
	gl_getcontext (CanvasContext);
	gl_clearscreen (scrnc);
	gl_setclippingwindow(0,0,vw-1,vh-1);
	SetTextColors ();
	gl_setcontext (GuiScreenContext);
      }
     // Set the maps 
     processid=procid; 
     scm[procid<<16|ID];
      
  }
  virtual void Line (unsigned x1, unsigned y1, unsigned x2, unsigned y2,
		     unsigned color);
  virtual void Bar (unsigned x1, unsigned y1, unsigned x2, unsigned y2,
		    unsigned color);
  virtual void Circle (unsigned x, unsigned y, unsigned r, unsigned color);
  virtual void SetTextColors (int tc = 0, int tbc = 0);
  virtual void Write(int x,int y,int fc,int bc,int fno,char *text);
  virtual void howto (void);
  virtual void process (Message * WinM);
  virtual void Pixel (unsigned x, unsigned y, unsigned color);
  virtual ~ScrollableCanvas(){
     scm.erase(processid<<16|ID);
  }
  virtual void SetKeyState(bool state){ keyflow=state;}
};				// End of Class


////////////////////////////////////////////////////////////////////
/* defines the basic text box implementation */

#define EDIT_BOX_MAX_LENGTH 256
class EditBox:public Win_Objects
{
  protected:short Length, Position, dl, dr, nc, dp, dy;
  char Text[EDIT_BOX_MAX_LENGTH];
public:
    EditBox (unsigned id, unsigned pid, int x, int y, int w, int h, char *text);
  virtual void process (Message * wm);
  virtual void howto (void);
  virtual void GetText (char *t);
  virtual void SetText (char *t, short update = 1);
  virtual short GainFocus (void);
  virtual short LoseFocus (void);
};				//eoc    

/* Implements a Password Box that echos '*' */
class PasswordBox:public EditBox
{
public:
  PasswordBox (unsigned id, unsigned pid, int x, int y, int w, int h,
	       char *text):EditBox (id, pid, x, y, w, h, text)
  {
  };
  virtual void howto (void);
};				//eoc    

////////////////Definition of Label Class//////////////////
/* Implements a label */
class Label:public Win_Objects
{
  protected:char Text[80];
  int FontColor;
  unsigned char *FontName;
    public:Label (unsigned id, unsigned pid, int x, int y, int w, int h,
		  char *text, int bc = 0, int fc = 0, unsigned char *fname =
		  NULL);
  void SetLabelText (char *text);
  void GetLabelText (char *text);
  void virtual process (Message * wm);
  virtual void howto (void);
  virtual short GainFocus (void)
  {
  };
  virtual short LoseFocus (void)
  {
  };

};				//eoc

////////////////////Definition of class CheckBox/////////////////////////////
/* Implements the checkbox object */
class CheckBox:public Win_Objects
{
  protected:short Checked;
  char Text[80];
  int FontColor;
  unsigned char *FontName;
  unsigned btop, bleft, bright, bbottom;
    public:CheckBox (unsigned id, unsigned pid, int x, int y, int w, int h,
		     char *text, int bc = 0, int fc =
		     0, unsigned char *fname = NULL, short yesno = FALSE);
  void process (Message * wm);
  virtual void howto (void);
  //virtual short GainFocus(void){};
  virtual short LoseFocus (void);
  virtual void MoveBy (int dx, int dy);
  short GetStatus (void);
  short SetStatus (short status)
  {
    if (active)
      {
	Checked = (status) ? TRUE : FALSE;
	show ();
	return TRUE;
      }
    return FALSE;
  }
};				//eoc


/////////////////////////////////////////////////////////////////////////////
/////////////////////////Window Definition /////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class Window:public Win_Objects
{
  protected:char Title[80];
  unsigned procid;
  int TitleBarColor, TitleTextColor;
  int ScreenX, ScreenY, ScreenWidth, ScreenHeight;
  short display_flag, move_flag, update_report;
  short title_flag;
  unsigned childid;
  unsigned parentid;
    public:Window (unsigned id, unsigned pid, int x, int y, int w, int h,
		   char *text, unsigned processid, int bc = 0, int bdc =
		   0, int tbc = 0, int ttc = 0);
  void set_title (char *text);
  short InTitleBar (int x, int y);
  void showTitleBar (short stat);
  void show (void);
  void howto (void);
  void process (Message * Win_Message);
  void SetActive (void);
  void SetInactive (void);
  void Hide (void);
  void EnableDisplay (void);
  unsigned ChildId(void) {  return childid; }
  unsigned ParentId(void) {  return parentid; }
  void SetChildId(unsigned cid){ 
		  //DEBUG_WRITE("Setting child id of %x to %x ",ID,cid);
		  childid = cid;
  }
  short IsDisplayable (void)
  {
    return display_flag;
  }
  unsigned GetProcessId (void)
  {
    return procid;
  }
  void MoveBy (int dx, int dy);
  void SetTitleFlag (short flag)
  {
    title_flag = flag;
  }
   ~Window ();
  WRect wr;
};

//////////////////////////////////////////////////////////////////////////
////////// Super structure definition  /////////////////////////////////
//////////////////////////////////////////////////////////////////////
/* 
 *  the following structures are defined to manipulate the windows in 
    the system and their own different components like Button, Window etc
    */

/* 
 * This structure holds the information about the various component in a 
 * window in a double linked list 
 * */
typedef struct tag_componentlist
{
  Win_Objects *info;
  struct tag_componentlist *Next, *Prev;
}
ComponentList;

/* This structure hols the different windows on a double-linked list */
typedef struct tag_WinList
{
  Window *pWindow;
  struct tag_WinList *Next, *Prev;
  ComponentList *pComponents, *pCompEnd, *pCompCurrent;
  unsigned Level, Id, LastChildId;
}
WinList;

/* this class defines the methods necessary to mani pulate the various 
   windows on the screen */
class SuperStructure
{
  public:WinList * Start, *End;
  WinList *CurrentWindow, *Anchor;

  unsigned LastId;

    SuperStructure ()
  {
    Start = End = NULL;
    CurrentWindow = NULL;
    Anchor= NULL;
    LastId = 0;
  }
   ~SuperStructure ();
  // Window manipulation functions
  short InsertWindow (Window * pW);	// done
  short IsWindowCurrent (unsigned Id);	// done
  void HideWindow (unsigned Id);	// done
  void EnableWindowDisplay (unsigned Id);	//done
  short DoesWindowExist (unsigned Id);	// done
  short DeleteWindow (unsigned Id, short show = TRUE,bool pn=true);	// done
  short ShowWindow (unsigned Id);	// done
  void SetCurrentWindow (WinList * pWL);	//done
  void SetCurrentWindow (unsigned Id);	//done
  void SetNextWindowActive (void);	// done
  WinList * GetFirstProcessWindow(unsigned procid);
  WinList * GetNextProcessWindow(unsigned procid,WinList *pwl);

  // Component & Windowz manipulation functions
  void ShowAllWindows (void);
  void UpdateScreen (void);
  short InsertWindowObject (Win_Objects * pO);	// inserts a newly requested 
  // component    
  void ShowChildObjects (unsigned Id);	// asks the components in a
  // window to show themselves 
  void SetChildObjectActive (unsigned Id, unsigned Pid, bool state);
  unsigned GetFocussedId (unsigned Pid);
  short SetFocusTo (unsigned Id, unsigned Pid);
  short DeleteWindowObject (unsigned Id, unsigned Pid);
  short DoesComponentExist (unsigned Id, unsigned Pid);	// checks existence of component

  short DeleteWindowComponentList (unsigned Id);	//deletes all the components
  void DeleteComponentList (WinList * pWL);
  void MoveChildObjectsBy (unsigned Id, int dx, int dy);
  void GetDimensions (unsigned Id, Rectangle & R);
  Win_Objects *GetWindowObject (unsigned Id, unsigned Pid);
  short GetWindowObjectFrameInfo (unsigned Id, unsigned Pid,
				  int *px, int *py, int *pw, int *ph);

  void DeactivateCurrentWindow (void);
  void ActivateCurrentWindow (void);
};				// End class definition

extern SuperStructure SS_WindowStructure;
extern void SetClipArea (Rectangle R);	//sets the cliparea to the confines of R

#endif
