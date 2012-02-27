#include "winobject.h"
#include "processinput.h"
#include "guiout.h"
#include "winarea.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "font.h"
#include "requestlist.h"
#include "debug.h"

#define  GCLIP gl_setclippingwindow(0,0,640,480);
#define DEACTIVE_BACKGROUND SS_SystemSettings.Window.BaseColor
SystemSettings SS_SystemSettings;
SuperStructure SS_WindowStructure;

static boolean Update[MAX_WINDOWS];
static int wBuffer[4*MAX_WINDOWS];
static int iRC;
static boolean UpdateFlag;
static unsigned parwin=0;
boolean ScreenUpdate;
unsigned curr_pwin=0;
extern unsigned char  font12[2816],font10[2048],font7[4096],font8[4096],font9[2048],font11[11264],font1[2048],font2[2048],font4[3072],font5[4096],font13[8192];


unsigned GE ;
void SetClipArea(Rectangle R){
	gl_setclippingwindow(R.Left,R.Top,R.Right,R.Bottom);
	}

void Reset_iRC(void){
   for(int i=0;i<iRC;i++)
     Update[i]=false;
    }
	 
CanvasMap cm;
ScrollableCanvasMap scm;


/////////////////////////////////////////////////////////////////////////
short Rectangle::IsInside(int x, int y )
	{
	return ((x>=Left && x<=Right)&&(y>=Top && y<=Bottom) ? 1:0);
	}

Rectangle::Rectangle()
	{
	Left=Right=Top=Bottom=0;
	Width= Height=1;
	 }
Rectangle::Rectangle(int l,int t, int w, int h )
        {
 	  Left=l;
	  Top=t, Width=w, Height=h;
	  Right=l+w-1;
	  Bottom=t+h-1;
	 }

void Rectangle::MoveTo(int x, int y)
	       {
		Left=x;
		Top=y;
		Right=Left+Width-1;
		Bottom=Top+Height-1;
	        }

void Rectangle::MoveBy(int dx,int dy)
		{
	  	 Left+=dx;Right+=dx;
		 Top+=dy;Bottom+=dy;
		}

void Rectangle::SetDimensions(int w,int h)
		{
		 Width=w;Height=h;
		 Right=Left+Width-1;
		 Bottom=Top+Height-1;
		 }


void Rectangle::GetDimensions(int *w,int *h)
		 {
		 *w=Width;*h=Height;
		 }

void Rectangle::GetOrigin(int *x,int *y)
		 {
		 *x=Left;*y=Top;
		  }

////////////////////////////////////////////////////////////////////////////////
Win_Objects::Win_Objects(int x,int y,int w, int h,unsigned id, unsigned pid,int bc,int bdc):Rectangle(x,y,w,h)
	{
	 type=0;
	 BaseColor=bc; BorderColor=bdc ;
		  ID=id;
		  PID=pid ;
		  active=TRUE;
	  if(PID!=0){
		 SS_WindowStructure.GetDimensions(pid,ClipArea);
		if(ClipArea.Left<Left)
			ClipArea.Left=Left;
		else
			ClipArea.Left+=2;
		if(ClipArea.Right>Right)
			ClipArea.Right=Right;
		else
			ClipArea.Right-=2;
		if(ClipArea.Top<Top)
			ClipArea.Top=Top;
		else
		  ClipArea.Top+=2;
		if(ClipArea.Bottom>Bottom)
		  ClipArea.Bottom=Bottom;
		else
		  ClipArea.Bottom-=2;
		}
	  else
		 {
		  ClipArea.Left=Left;
		  ClipArea.Right=Right;
		  ClipArea.Top=Top;
		  ClipArea.Bottom=Bottom;

		  }
		 ClipArea.Width=ClipArea.Right-ClipArea.Left+1 ;
		 ClipArea.Height=ClipArea.Bottom-ClipArea.Top+1 ;
	 display=Infocus=FALSE;	 
	 }

 void Win_Objects::MoveTo(int x,int y){
		Rectangle::MoveTo(x,y);
		ClipArea.MoveTo(x,y);
		}
 void Win_Objects::MoveBy(int dx,int dy){
		Rectangle::MoveBy(dx,dy);
		ClipArea.MoveBy(dx,dy);
		}

 void Win_Objects::SetDimensions(int w,int h){
		Rectangle::SetDimensions(w,h);
		ClipArea.SetDimensions(w,h);
		}

short Win_Objects::LoseFocus(void)
  {
	Infocus=FALSE;
	show();
	return TRUE;
  }

short Win_Objects::GainFocus(void)
 {
	Infocus=TRUE;
	show();
	if(active)
	 drawbox(Left+1,Top+1,Width-2,Height-2,SS_SystemSettings.InFocusColor);
	return TRUE;
  }

 static unsigned objshowBuffer[4*MAX_WINDOWS];
 static unsigned objshowregions;

 void Win_Objects::show(void){
  DEBUG_WRITE(" Win_OBjects::show %x of %x ",ID,PID) ;
  if(!display){
      DEBUG_WRITE(" ... display disabled"); 
	  return;
  }
  if(SS_WindowStructure.IsWindowCurrent(PID)
	&&!SS_SystemSettings.MenuActiveFlag){
	  SetClipArea(ClipArea);
	  howto();
	  }
	else{
         DEBUG_WRITE(" Win_OBjects::show %x of %x ELSE",ID,PID) ;
		 Rectangle R;
		  int l,t,r,b,i,j;
		  if(parwin!=PID||SS_SystemSettings.MenuActiveFlag){
		  SS_WindowStructure.GetDimensions(PID,R);
		  Win_Objects W(R.Left,R.Top,R.Width,R.Height,PID,0);
		  objshowregions=SS_WindowArea.GetBoundaries(W,objshowBuffer);
		  parwin=PID;
		  }
		  for(i=j=0;i<objshowregions;i++){
				l=objshowBuffer[j++];
				t=objshowBuffer[j++];
				r=objshowBuffer[j++];
				b=objshowBuffer[j++];
				if(l<ClipArea.Left)
				    l=ClipArea.Left;
				if(r>ClipArea.Right)
				    r=ClipArea.Right;
				if(t<ClipArea.Top)
				    t=ClipArea.Top;
				 if(b>ClipArea.Bottom)
				      b=ClipArea.Bottom;
                 gl_setclippingwindow(l,t,r-1,b-1);
				 //gl_fillbox(l,t,r-l,b-t,14);
				 //sleep(1);
				 howto();
				 }
		  }
 
  DEBUG_WRITE(" Win_OBjects::show %x of %x RETURNING",ID,PID) ;
}


Win_Objects::~Win_Objects(){
    DEBUG_WRITE("Destroying %x of %x ",ID,PID);
    }
///////////////////////////////////////////////////////////////////////////////



Button:: Button(unsigned id,unsigned pid,int x,int y,int w, int h,char *text,int bc,int bdc, int ac , int ic) : Win_Objects(x,y,w,h,id,pid,bc,bdc)
 {
  type= OBJ_BUTTON_ID ;
  strcpy(Label,text);
  activefontcolor=ac ;
  inactivefontcolor=ic ;
  active=1;
  blsx=(w-strlen(Label)*SS_SystemSettings.Button.LabelFontWidth)/2;
  blsy=(h-SS_SystemSettings.Button.LabelFontHeight)/2;
  FocusColor=SS_SystemSettings.Button.InFocusColor;
  aBaseColor=SS_SystemSettings.Button.BaseColor;
DEBUG_WRITE("Button Constructed %x of %x ",ID,PID);
}

void Button::SetColors(int fc,int bc,int afcolor,int icfcolor)
{
   
   FocusColor=(fc==0? FocusColor: fc) ;
   activefontcolor=(afcolor==0?activefontcolor:afcolor) ;
   inactivefontcolor=(icfcolor==0? inactivefontcolor:icfcolor) ;
   BaseColor=(bc==0? BaseColor:bc) ;
   aBaseColor=(bc==0? aBaseColor:bc) ;
   show();
   DEBUG_WRITE("Setting Colors of Button %x of %x ",ID,PID);
   return ;
}
         
void Button::SetText(char *text)
 {
  strcpy(Label,text) ;
  blsx=(Width-strlen(Label)*SS_SystemSettings.Button.LabelFontWidth)/2;
  blsy=(Height-SS_SystemSettings.Button.LabelFontHeight)/2;
  this->show() ;
  return ;
}

void Button :: animate(void)
{
	// Animates the button pressing
				SetClipArea(ClipArea);
		 gl_fillbox(Left,Top,Width, Height,BaseColor) ;
		 gl_setfontcolors(BaseColor,inactivefontcolor) ;
					  gl_setfont(SS_SystemSettings.Button.LabelFontWidth,SS_SystemSettings.Button.LabelFontHeight,SS_SystemSettings.Button.LabelFont);
		 gl_write(Left+blsx+6,Top+blsy+6,Label);
					 usleep(10000);
		 gl_fillbox(Left,Top,Width, Height,BaseColor) ;
		 gl_setfontcolors(BaseColor,activefontcolor) ;
		 gl_write(Left+blsx,Top+blsy,Label);
		 draw3doutbox(Left,Top,Width, Height) ;
		 return ;
}


void Button::process(Message *Button_Message)
 {
	if(active
	 &&Button_Message->Type==KEYBOARD_EVENT)
	{
	 if(Button_Message->Event.Keyboard.Value==KEY_ENTER)
		 {
		 animate() ;
		 Button_Message->Type=SYSTEM_EVENT ;
		 Button_Message->Event.System.Source_Id=ID ;
		 Button_Message->Event.System.Destination_Id=PID ;
		 Button_Message->Event.System.Message=SS_COMPONENT_USED ;
		 }
	 else if(
	     (Button_Message->Event.Keyboard.Value==KEY_ARROWUP)||
	     (Button_Message->Event.Keyboard.Value==KEY_ARROWDOWN)||
	     (Button_Message->Event.Keyboard.Value==KEY_ARROWLEFT)||
	     (Button_Message->Event.Keyboard.Value==KEY_ARROWRIGHT)||
	     (Button_Message->Event.Keyboard.Value==KEY_ESCAPE))
		 Button_Message->Type=KEYBOARD_EVENT ;
	     else
	         Button_Message->Type=NULL_EVENT  ;
	}
	else   
	 Button_Message->Type=NULL_EVENT  ;
  return ;
}

short Button::GainFocus(void){
 Infocus=TRUE;
 if(active)
//	BaseColor=SS_SystemSettings.Button.InFocusColor;
	BaseColor=FocusColor;
 else
//	BaseColor=SS_SystemSettings.Button.BaseColor;
	BaseColor=aBaseColor;
 show();
 return TRUE;
 }

short Button::LoseFocus(void){
 Infocus=FALSE;
	//BaseColor=SS_SystemSettings.Button.BaseColor;
	BaseColor=aBaseColor;
	show();
 return TRUE;
 }

void Button::howto(void)
 {
	if(active)
	  gl_setfontcolors(BaseColor,activefontcolor) ;
 	else
	  gl_setfontcolors(BaseColor,inactivefontcolor) ;
	  gl_setfont(SS_SystemSettings.Button.LabelFontWidth,
	    SS_SystemSettings.Button.LabelFontHeight,
	    SS_SystemSettings.Button.LabelFont);
	gl_fillbox(Left,Top,Width,Height,BaseColor) ;
	draw3doutbox(Left,Top,Width, Height);
	gl_write(Left+blsx,Top+blsy,Label);

	return ;
}

void Button::SetInactive(void){
    active=FALSE; 
    BaseColor=SS_SystemSettings.Button.BaseColor;
    show();
}
	    
////////////////////////////////////////////////////////////////////////////
SystemButton::SystemButton(unsigned pid,int x,int y):
       Button(WINDOW_BUTTON_CROSS,pid,x,y,
	SS_SystemSettings.Window.TitleBarHeight, SS_SystemSettings.Window.TitleBarHeight,"")
  {
   BaseColor=SS_SystemSettings.Button.BaseColor;
   CrossColor=SS_SystemSettings.SystemButton.CrossColor;
   BorderColor=SS_SystemSettings.Button.BorderColor;
   };
	
void SystemButton::process(Message *Button_Message)
      {
	if(!active)
	 return;
	if(Button_Message->Type==KEYBOARD_EVENT
		  && Button_Message->Event.Keyboard.Value==SS_ENTER)
	  {
		 animate() ;
		 Button_Message->Type=SYSTEM_EVENT ;
		 Button_Message->Event.System.Source_Id=ID ;
		 Button_Message->Event.System.Destination_Id=PID ;
		 Button_Message->Event.System.Message=SS_DESTROYWINDOW ;
	  }
	else
	 Button_Message->Type=NULL_EVENT  ;
  return ;
}

void SystemButton::howto(void)
 {
	if(active)
           CrossColor=SS_SystemSettings.SystemButton.CrossColor;
        else 
           CrossColor=SS_SystemSettings.Button.DeactiveColor;
	gl_fillbox(Left,Top,Width, Height,BaseColor) ;
	draw3doutbox(Left+1,Top+1,Width-2, Height-2);
	gl_line(Left+3,Top+3,Right-3,Bottom-3,CrossColor);
	gl_line(Left+3,Bottom-3,Right-3,Top+3,CrossColor);
	return ;
 }

void SystemButton::animate(void)
{
	SetClipArea(ClipArea);
	 drawbox(Left+1,Top+1,Width-2, Height-2, BorderColor);
	 gl_fillbox(Left+1,Top+1,Width-2, Height-2,BaseColor) ;
	 gl_line(Left+5,Top+5,Right-4,Bottom-4,CrossColor);
	 gl_line(Left+5,Bottom-5,Right-4,Top+4,CrossColor);
 	 usleep(10000);
        howto();
	return ;
}


////////////////////////////////////////////////////////////////////////////////
Canvas::Canvas(unsigned id,unsigned pid,int x,int y,int w,int h,int procid,int scrnc,int scrbdc): Win_Objects(x,y,w,h,id,pid,scrnc,scrbdc)
 {
	 keyflow=false;	 
	 type= OBJ_CANVAS_ID; 
	 VBuffer = (char*)malloc((w)*(h));
	 cgc=gl_allocatecontext();
	 vgc=gl_allocatecontext();
	 gl_getcontext(cgc);
	 gl_setcontextvgavirtual(10);
	 gl_setcontextvirtual(w,h,1,8,VBuffer);
	 gl_enableclipping();
	 gl_setclippingwindow(2,2,w,h-1);
	 gl_fillbox(0,0,w,h,scrnc);
	 //draw3dinbox(0,0,w,h);
	 gl_getcontext(vgc);
	 gl_setcontext(cgc);
	 DrawArea=ClipArea;
	 DrawArea.Left=ClipArea.Left+2;DrawArea.Top=ClipArea.Top+2;
	 DrawArea.Width-=4;DrawArea.Height-=4;
	 DrawArea.Right=ClipArea.Right-2;
	 DrawArea.Bottom=ClipArea.Bottom-2;
	 fontno=0;
	 SetTextColors();
	 processid=procid;
	 cm[procid<<16|ID]=this;
     keyflow=false;	 
  DEBUG_WRITE("Canvas constructed  %x of %x   with buffer at %p",ID,PID,VBuffer);
  }

void Canvas::MoveTo(int x,int y){
   Win_Objects::MoveTo(x,y);
   DrawArea.MoveTo(x,y);
}

void Canvas::MoveBy(int dx,int dy){
  Win_Objects::MoveBy(dx,dy);
  DrawArea.MoveBy(dx,dy);
}



void Canvas::howto(void)
{
 gl_copyboxfromcontext(vgc,2,2,Width-4,Height-4,Left+2,Top+2);
 draw3dinbox(Left,Top,Width,Height);
}

void Canvas::UpdateArea(int left,int top,int right,int bottom){
 Rectangle R;
  int l,l1,t,t1,r,r1,b,b1,i,j,f;
  if(parwin!=PID||SS_SystemSettings.MenuActiveFlag){
  SS_WindowStructure.GetDimensions(PID,R);
  Win_Objects W(R.Left,R.Top,R.Width,R.Height,PID,0);
  objshowregions=SS_WindowArea.GetBoundaries(W,objshowBuffer);
  parwin=PID;
  }
  for(i=j=0,f=0;i<objshowregions;i++){
	  l1=left,r1=right,t1=top,b1=bottom;	
	  l=objshowBuffer[j++];
		t=objshowBuffer[j++];
		r=objshowBuffer[j++]-1;
		b=objshowBuffer[j++]-1;
		if(l<DrawArea.Left)
			l=DrawArea.Left;
		if(r>DrawArea.Right)
			r=DrawArea.Right;
		if(t<DrawArea.Top)
			t=DrawArea.Top;
		if(b>DrawArea.Bottom)
        	        b=DrawArea.Bottom;
		 // ok clipping to window done
		 // now clip to required area
		 if(l<left+Left)
			l=left+Left,f++;
		 if(t<top+Top)
			t=top+Top,f++;
		 if(r>right+Left)
			r=right+Left,f++;
		 if(b>bottom+Top)
			b=bottom+Top,f++;
		 if(l>r|| t >b )
		        continue;
		 gl_setclippingwindow(l,t,r,b);
		 //gl_fillbox(l,t,r-l,b-t,164+rand()%40); 
		 gl_copyboxfromcontext(vgc,l-Left,t-Top,
		 		 r-l+2,b-t+2,l,t);	 
		 if(f==4)
			 break;
			 
		 //usleep(80000);
          }	 
}

void Canvas::SetTextColors(int tc,int tbc,int fn)
{
 if(!tc)
  TextColor=SS_SystemSettings.Canvas.TextColor;
 else
  TextColor=tc;
 if(!tbc)
  TextBackgroundColor=SS_SystemSettings.Canvas.TextBackgroundColor;
 else
  TextBackgroundColor=tbc;
 fontno=fn;
}

void Canvas::process(Message *WinM){
 if(!keyflow)
   WinM->Type=NULL_EVENT;		 
}// End Proc

/*Added On 7th Jan 2002 for addition of new fonts*/

void Canvas::Write(int x,int y,int fc,int bc,int fno,char *text)
{	
 if(!fc)
  TextColor=SS_SystemSettings.Canvas.TextColor;
 else
  TextColor=fc;
 if(!bc)
  TextBackgroundColor=SS_SystemSettings.Canvas.TextBackgroundColor;
 else
  TextBackgroundColor=bc;
 if(!fno)
 {
   Font=SS_SystemSettings.Canvas.CanvasFont;
   FontWidth=SS_SystemSettings.Canvas.CanvasFontWidth;
   FontHeight=SS_SystemSettings.Canvas.CanvasFontHeight;
 }
 else
 {
	 
	if(fno==7)
	{
		Font=font7;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==8)
	{
		Font=font8;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==9)
	{
		Font=font9;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==10)
	{
		Font=font10;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==11)
	{
		Font=font11;
		FontWidth=12;
		FontHeight=22;
	}	
	if(fno==1)
	{
		Font=font1;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==2)
	{
		Font=font1;
		FontWidth=8;
		FontHeight=8;
	}	
	if(fno==4)
	{
		Font=font4;
		FontWidth=8;
		FontHeight=12;
	}	
	if(fno==5)
	{
		Font=font5;
		FontWidth=8;
		FontHeight=16;
	}	
	if(fno==12)
	{
		Font=font12;
		FontWidth=6;
		FontHeight=11;
	}	
	if(fno==13)
	{
		Font=font13;
		FontWidth=8;
		FontHeight=16;
	}	
		
 }	       	
	 
  	
  gl_setfontcolors(TextBackgroundColor,TextColor);
  gl_setfont(FontWidth,FontHeight,Font);
  gl_setcontext(vgc);
  gl_write(x+2,y+2,text);
  gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
	          &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(DrawArea);
		 gl_write(Left+x+2,Top+y+2,text); 
 }
 else
  UpdateArea(x,y,x+SS_SystemSettings.Canvas.CanvasFontWidth*strlen(text)
		  ,y+SS_SystemSettings.Canvas.CanvasFontHeight);//show();
}// End Proc

void Canvas::Write2(int x,int y,char *text)
{
 int tw,th;
 if(fontno>MAX_FONT_NO){
		 Write(x,y,20,30,1,text);
		 return;
 		}
 set_font(fontno) ;
 set_fontcolor(TextColor);
 gl_setcontext(vgc);
 draw_text(x+2,y+2,text,1,TextBackgroundColor,&tw,&th);
 gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(DrawArea); 
 		draw_text(Left+x+2,Top+y+2,text,1,TextBackgroundColor,&tw,&th);
 }
 else
 UpdateArea(x+2,y+2,x+tw+2,y+th+4);
} 

void Canvas::ShiftUpBy(unsigned dy,unsigned color)
{
if(!dy)
	return;
if(dy>Height)
 dy=Height;
int d=Width*dy,i,j;
i=Width*Height-d;
memmove(VBuffer,VBuffer+d,i);
memset(VBuffer+i,color,d);
show();
}	

void Canvas::ShiftDownBy(unsigned dy,unsigned color)
{
if(!dy)
	return;
if(dy>Height)
	 dy=Height;
int d=Width*dy,i,j;
i=Width*Height-d;
memmove(VBuffer+d,VBuffer,i);
memset(VBuffer,color,d);
show();
}

void Canvas::ShiftRightBy(unsigned dx,unsigned color)
{
	int i,j;
	if(!dx)
		return;
	if(dx>Width)
		dx=Width;
	int d=Width-dx;	
	for(j=i=0;i<Height;i++,j+=Width){
		memmove(&VBuffer[j]+dx,&VBuffer[j],d);
		memset(&VBuffer[j],color,dx);
	}
	show();
}

void Canvas::ShiftLeftBy(unsigned dx,unsigned color)
{
    int i,j;
	if(!dx)
       return;
    if(dx>Width)
	     dx=Width;
	int d=Width-dx;
	for(j=i=0;i<Height;i++,j+=Width){
	     memmove(&VBuffer[j],&VBuffer[j]+dx,d);
	     memset(&VBuffer[j]+d,color,dx);
   }
   show();
}


void Canvas::Line(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned color)

{
  gl_setcontext(vgc);
  gl_line(x1+2,y1+2,x2+2,y2+2,color);
  gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(DrawArea); 
		  gl_line(Left+x1+2,Top+y1+2,Left+x2+2,Top+y2+2,color);
 }
 else
UpdateArea(x1+2,y1+2,x2+2,y2+2);//  show();
}// End Proc

void Canvas::Circle(unsigned x, unsigned y , unsigned r , unsigned color)
{
  gl_setcontext(vgc);
  gl_circle(x+2,y+2,r,color);
  gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(DrawArea); 
  		  gl_circle(Left+x+2,Top+y+2,r,color);
 }
 else
  UpdateArea(x-r+2,y-r+2,x+r+2,y+r+2);//show();
}
void Canvas::Bar(unsigned x1,unsigned y1,unsigned w,unsigned h,unsigned color)
{
  gl_setcontext(vgc);
  gl_fillbox(x1+2,y1+2,w,h,color);
  gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	     SetClipArea(DrawArea); 
  	     gl_fillbox(Left+x1+2,Top+y1+2,w,h,color);
 }
 else
  UpdateArea(x1+2,y1+2,x1+w+2,y1+h+2);//show();
 }

void Canvas::Pixel(unsigned x,unsigned y,unsigned color)
{
  gl_setcontext(vgc);
  gl_setpixel(x+2,y+2,color);
  gl_setcontext(cgc);
 if(!display)
	 return;
 if(SS_WindowStructure.IsWindowCurrent(PID)
	        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(DrawArea); 
  		  gl_setpixel(Left+x+2,Top+y+2,color);
 }
 else
  UpdateArea(x+2,y+2,x+2,y+2);//show();
 }
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ScrollableCanvas::howto(void)
{
 gl_copyboxfromcontext(CanvasContext,VirtualLeft,VirtualTop,Width-4,Height-4,Left+2,Top+2);
 drawbox(Left,Top,Width,Height,BorderColor);
 drawbox(Left+1,Top+1,Width+14,Height+14,BorderColor);
 drawbox(Left,Top,Width,Height,27);
 if(Infocus)
   drawbox(Left,Top,Width,Height,SS_SystemSettings.InFocusColor);
}

void ScrollableCanvas::SetTextColors(int tc,int tbc)
{
 if(!tc)
  TextColor=SS_SystemSettings.Canvas.TextColor;
 else
  TextColor=tc;
 if(!tbc)
  TextBackgroundColor=SS_SystemSettings.Canvas.TextBackgroundColor;
 else
  TextColor=tbc;
}
void ScrollableCanvas::Write(int x,int y,int fc,int bc,int fno,char *text)
{
  if(x>VirtualWidth || y > VirtualHeight)
     return ;
  gl_setcontext(CanvasContext) ;
  gl_setfontcolors(TextBackgroundColor,TextColor);
  gl_setfont(SS_SystemSettings.Canvas.CanvasFontWidth,
             SS_SystemSettings.Canvas.CanvasFontHeight,
             SS_SystemSettings.Canvas.CanvasFont);
  gl_write(x,y,text);// Arrange for clipping here
  gl_setcontext(GuiScreenContext) ;
  if(!display)
	  return;/*
 if(SS_WindowStructure.IsWindowCurrent(PID)
	          &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(ClipArea);
		 gl_write(Left+x+2-VirtualLeft,Top+y+2-VirtualTop,text); 
 }
 else*/
    show();  	 
 }
void ScrollableCanvas::Line(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned color)
{
  gl_setcontext(CanvasContext);
  gl_line(x1,y1,x2,y2,color);
  gl_setcontext(GuiScreenContext);
  if(!display)
	  return;/*
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(ClipArea); 
		  gl_line(Left+x1+2-VirtualLeft,Top+y1+2-VirtualTop,
						  Left+x2+2-VirtualLeft,Top+y2+2-VirtualTop,color);
 }*/
 else
  show();

}
 void ScrollableCanvas::Bar(unsigned x1,unsigned y1,unsigned w,unsigned h,unsigned color){
  
  gl_setcontext(CanvasContext);
  gl_fillbox(x1,y1,w,h,color);
  gl_setcontext(GuiScreenContext);
  if(!display)
	  return;/*
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	     SetClipArea(ClipArea); 
  	     gl_fillbox(Left+x1+2-VirtualLeft,Top+y1+2-VirtualTop
	      					 ,w,h,color);
 }*/
 else
  show();
  
}
void ScrollableCanvas::Pixel(unsigned x,unsigned y,unsigned color)
{
  gl_setcontext(CanvasContext);
  gl_setpixel(x,y,color);
  gl_setcontext(GuiScreenContext);
  if(!display)
	  return;/*
 if(SS_WindowStructure.IsWindowCurrent(PID)
	        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(ClipArea); 
  		  gl_setpixel(Left+x+2-VirtualLeft,Top+y+2-VirtualTop,color);
 }
 else*/
  show();
 }

void ScrollableCanvas::Circle(unsigned x, unsigned y , unsigned r , unsigned color)
{
  gl_setcontext(CanvasContext);
  gl_circle(x,y,r,color);
  gl_setcontext(GuiScreenContext);
  if(!display)
	  return;/*
 if(SS_WindowStructure.IsWindowCurrent(PID)
		        &&!SS_SystemSettings.MenuActiveFlag){
	          SetClipArea(ClipArea); 
  		  gl_circle(Left+x+2-VirtualLeft,Top+y+2-VirtualTop,r,color);
 }
 else*/
  show();
}


 void ScrollableCanvas::process(Message *WinM)
 {
   int dx , dy ;
  if(WinM->Type==KEYBOARD_EVENT && WinM->Event.Keyboard.Category==SS_CURSORBLOCK)
  {
     switch(WinM->Event.Keyboard.Value)
     {
        case KEY_ARROWLEFT:   dx=VirtualLeft-5 ;
                              VirtualLeft=(dx<0 ? 0 :dx) ;
                              break ;

        case KEY_ARROWRIGHT:   dx=VirtualLeft+5 ;
   VirtualLeft=(VirtualWidth-(dx+Width)<0 ? VirtualLeft :dx) ;
                              break ;

        case KEY_ARROWUP:   dy=VirtualTop-5 ;
                    VirtualTop=(dy<0 ? 0 :dy) ;
                     break ;


        case KEY_ARROWDOWN:   dy=VirtualTop+5 ;
                              VirtualTop=(VirtualHeight-(dy+Height)<0 ? VirtualTop :dy) ;                              break ;
       }// End of switch ;

    WinM->Type=NULL_EVENT ;
   this->show() ;
  }// End of if
 if(!keyflow)
   WinM->Type=NULL_EVENT;		 
return ;
}

///////////////////////////////////////////////////////////////////////////////
EditBox::EditBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text)
 :Win_Objects(x,y,w,h,id,pid)
{
  type= OBJ_EDITBOX_ID;
 Position=0;
 strncpy(Text,text,EDIT_BOX_MAX_LENGTH-1);
 Text[EDIT_BOX_MAX_LENGTH-1]='\0';
 Length=strlen(Text);
 dr=SS_SystemSettings.Canvas.CanvasFontWidth;
 nc=(Right-Left-4)/SS_SystemSettings.Canvas.CanvasFontWidth;
 dl=dp=0;
 dy=(Height-SS_SystemSettings.Canvas.CanvasFontHeight)/2;
 DEBUG_WRITE("Edit Box constructed %x of %x ",ID,PID);
}


void EditBox::GetText(char *t)
{
  strcpy(t,Text);
}


void EditBox::SetText(char *t,short display)
{
 Position=0;
 strncpy(Text,t,EDIT_BOX_MAX_LENGTH-1);
 Text[EDIT_BOX_MAX_LENGTH-1]='\0';
 Length=strlen(Text);
 dp=dl=0;
 if(display)
  show();
 }

void EditBox::howto(void)
{
	int c=(active)?SS_SystemSettings.Canvas.CanvasColor:DEACTIVE_BACKGROUND;
	 gl_fillbox(Left+2,Top+2,Width-4,Height-4,c);
	  if(dp<0)
	  {
	   dl=(Position>0)?(dl-1):0;
	   dp=0;
	  }
	 else
	  if(dp>nc)
	      { dl++;dp=nc;}
	 gl_setfontcolors(c,SS_SystemSettings.Canvas.TextColor);
	 gl_setfont(SS_SystemSettings.Canvas.CanvasFontWidth,
			SS_SystemSettings.Canvas.CanvasFontHeight,
			SS_SystemSettings.Canvas.CanvasFont);
	 gl_write(2+Left,dy+Top,&Text[dl]);
	 if(Infocus&&active){
	   //gl_line(2+Left+dp*dr,Top+3,2+Left+dp*dr,Bottom-3,12);
		 gl_hline(2+Left+dp*dr,Bottom-dy+2,2+Left+dp*dr+8,12); 
	   draw3dinbox(Left,Top,Width,Height);
	 }
	  else
	drawbox(Left,Top,Right,Bottom,/*SS_SystemSettings.Canvas.CanvasBorderColor*/7);
}// end howto


void EditBox::process(Message *WinM)
{
 int i;
 short terminate=0;
 if(!active){
	WinM->Type=NULL_EVENT;
	return;
	}

 switch(WinM->Type)
 {
  case KEYBOARD_EVENT:
		 switch(WinM->Event.Keyboard.Category)
		 {
		  case SS_NORMAL:
			if(WinM->Event.Keyboard.Value==10)//enter key
			   {
			     terminate=TRUE;
			      break;
			   }

			if(WinM->Event.Keyboard.Value==127)//backspace
			 {
			   if(Position)
			    {
			     for(dp--,i=--Position;Text[i]!='\0';i++)
			          Text[i]=Text[i+1];
		                   Length--;
			    }
			 }
			else
			 if(Length<EDIT_BOX_MAX_LENGTH-1)
		         {
			   for(i=Length++;i!=Position;i--)
			      Text[i]=Text[i-1];
			   Text[Position++]=WinM->Event.Keyboard.Value;
			   dp++;
			 }

			show();
	 break;

		  case SS_CURSORBLOCK:
			switch(WinM->Event.Keyboard.Value)
			{
			 case KEY_ARROWRIGHT:
			    if(Position<Length)
			       {
				 Position++;
			         dp++;
				 }
			     break;
			 case KEY_ARROWLEFT:
			    if(Position)
				 Position--;
			    dp--;
			    break;
			 case KEY_HOME:
			     Position=0;
			     dp=0;dl=0;
			     break;
			 case KEY_END:
			      if(Position<Length)
			      {
				 Position=Length;
				 if(Length>=nc)
				  {
			 dl=Length-nc;
			 dp=nc;
			}
			 else
			  dp=Length;

			  }
		  break;
	case KEY_DELETE:
			  if(Position<Length)
			  {
				for(i=Position;i<Length;i++)
				  Text[i]=Text[i+1];
				  Length--;
			  }
			 default:break;
		  }
	show();
	break;

	default :break;
	}// switch on Category
 break;
 default :break;
 }// switch on Type
if(terminate)
 {
	WinM->Type=SYSTEM_EVENT;
	WinM->Event.System.Source_Id=ID;
	WinM->Event.System.Destination_Id=PID;
	WinM->Event.System.Message=SS_EDITBOX_DONE;
 }
 else
  WinM->Type=NULL_EVENT;

}// end Edit Box process

short EditBox::GainFocus(void){
	Infocus=TRUE;
	show();
	return 1;
     }

short EditBox::LoseFocus(void){
	Infocus=FALSE;
	show();
	return 0;
	}


void PasswordBox::howto(void)
{
	int c=(active)?SS_SystemSettings.Canvas.CanvasColor:DEACTIVE_BACKGROUND;
	 gl_fillbox(Left+2,Top+2,Width-4,Height-4,c);
	  if(dp<0)
	  {
	   dl=(Position>0)?(dl-1):0;
	   dp=0;
	  }
	 else
	  if(dp>nc)
	      { dl++;dp=nc;}
	 gl_setfontcolors(c,SS_SystemSettings.Canvas.TextColor);
	 gl_setfont(SS_SystemSettings.Canvas.CanvasFontWidth,
			SS_SystemSettings.Canvas.CanvasFontHeight,
			SS_SystemSettings.Canvas.CanvasFont);
	 int pwd,x;
	 for(x=2+Left,pwd=0;
	     pwd<nc && Text[dl+pwd]!='\0';
	     x+=SS_SystemSettings.Canvas.CanvasFontWidth,pwd++)
	     gl_write(x,dy+Top,"*");
	 //gl_write(2+Left,dy+Top,&Text[dl]);
	 if(Infocus&&active){
	    //gl_line(2+Left+dp*dr,Top+3,2+Left+dp*dr,Bottom-3,12);
  	    gl_hline(2+Left+dp*dr,Bottom-dy+2,2+Left+dp*dr+8,12); 
	 draw3dinbox(Left,Top,Width,Height);
	 }
	  else
	drawbox(Left,Top,Right,Bottom,SS_SystemSettings.Canvas.CanvasBorderColor);
}// end howto
////////////////////////////////////////////////////////////////////////////

Label::Label(unsigned id,unsigned pid, int x,int y,int w,int h,char *text,int bc,int fc,unsigned char *fname):Win_Objects(x,y,w,h,id,pid,bc,bc)
 {
	type= OBJ_LABEL_ID;
	strcpy(Text,text);
	if(fname)
		  FontName=fname;
	 else
		  FontName=SS_SystemSettings.Button.LabelFont;
       DEBUG_WRITE("Label constructor %x of %x",ID,PID);  
       return ;
     		
  }

  void Label::SetLabelText(char *text)
	{
		strcpy(Text,text) ;
		return ;
	 }

  void Label::howto(void)
	{
	  
	  gl_fillbox(Left,Top,Width,Height,SS_SystemSettings.Window.BaseColor);		
   	  FontColor=SS_SystemSettings.Button.ActiveColor;
      gl_setfontcolors(BaseColor,FontColor);
	  gl_setfont(SS_SystemSettings.Button.LabelFontWidth,
				SS_SystemSettings.Button.LabelFontHeight,FontName);
		gl_write(Left+2,Top+2,Text);
		return ;
  }

void Label::GetLabelText(char *text)
  {
		strcpy(text,Text) ;
		return ;
  }

void Label::process(Message *wm)
 {

	wm->Type=NULL_EVENT  ;
	return ;
 }
//////////////////////////////////////////////////////////////////////////////

CheckBox::CheckBox(unsigned id,unsigned pid, int x,int y,int w,int h,char *text,int bc,int fc,unsigned char *fname,short yesno):Win_Objects(x,y,w,h,id,pid,(bc)?bc:SS_SystemSettings.Window.BaseColor,(fc)?fc:SS_SystemSettings.Window.BorderColor)
 {
      type= OBJ_CHECKBOX_ID;
	if(yesno)
	  Checked=TRUE ;
	else
	 Checked=FALSE ;
	 strcpy(Text,text);
	if(fc)
	 FontColor=fc;
	else
	 FontColor=0 ;
	if(fname)
	  FontName=fname;
	else
	  FontName=SS_SystemSettings.Button.LabelFont;
	bleft=Left+4;
	bright=bleft+15;
	btop=Top+(Height-17)/2;
	bbottom=btop+15;
	DEBUG_WRITE("CheckBox are %x of %x ",ID,PID); 
	return ;
  }

void CheckBox::MoveBy(int dx,int dy){
  Win_Objects::MoveBy(dx,dy);
  bleft+=dx,
  bright+=dx,
  btop+=dy,
  bbottom+=dy;
  return;
  }

short CheckBox::LoseFocus(void)
  {

	Infocus=FALSE;
	show();
	return TRUE;
  }

void CheckBox::howto(void)
  {
	  gl_setfontcolors(BaseColor,FontColor) ;
	  gl_setfont(SS_SystemSettings.Button.LabelFontWidth,
					  SS_SystemSettings.Button.LabelFontHeight,FontName);
	  gl_fillbox(Left,Top,Width,Height,BaseColor) ;
	  gl_fillbox(bleft,btop,15,15,(active)?
				SS_SystemSettings.Canvas.CanvasColor
			:DEACTIVE_BACKGROUND);
	  if(active&&Infocus){
		 draw3dinbox(bleft,btop,15,15);
		 drawdashedbox(Left+1,Top+1,Width-2,Height-2,
						SS_SystemSettings.InFocusColor);
		 }
	  else
		 drawbox(bleft,btop,15,15,BorderColor);
	  gl_write(bright+4,Top+(Height-SS_SystemSettings.Button.LabelFontHeight)/2
										,Text);
		 if(Checked)
			 {
				gl_line(bleft+2,btop+2,bright-2,bbottom-2,FontColor) ;
				gl_line(bleft+2,bbottom-2,bright-2,btop+2,FontColor);
			  }

			 return ;
	 }

void CheckBox::process(Message *wm)
  {
  if(!active){
	 wm->Type=NULL_EVENT;
	 return;
	 }

  if(wm->Type==KEYBOARD_EVENT && (wm->Event.Keyboard.Value==KEY_ENTER ||
										  wm->Event.Keyboard.Value==32))
	  {
			if(Checked)
			 Checked=FALSE;
			else
			 Checked=TRUE;
			show() ;
			wm->Type=SYSTEM_EVENT ;
			wm->Event.System.Source_Id=ID ;
			wm->Event.System.Destination_Id=PID ;
			wm->Event.System.Message=SS_COMPONENT_USED ;
		}
  else
			wm->Type=NULL_EVENT  ;
	 return ;

 }

short CheckBox::GetStatus(void)
 {
	 return (Checked)? TRUE:FALSE ;
 }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Window::Window(unsigned id,unsigned pid,int x,int y,int w, int h, char *text,unsigned processid,int bc, int bdc, int tbc,int ttc): Win_Objects(x,y,w,h,id,0,bc,bdc),wr(x,y,x+w-1,y+h-1)
{
	type= OBJ_WINDOW_ID;
	procid=processid;
	strcpy(Title,text);
			TitleBarColor=tbc , TitleTextColor=ttc ;
	ScreenX=Left+2;ScreenY=Top+SS_SystemSettings.Window.TitleBarHeight+1;
	ScreenWidth=Right-ScreenX-1;ScreenHeight=Bottom-ScreenY-1;
        display_flag=FALSE;
	SS_WindowArea.Link(*this);
	active=FALSE;
	move_flag=FALSE;
	update_report=FALSE;
	title_flag=FALSE;
	childid=0;
	DEBUG_WRITE("Setting parent id of %x to %x ",id,pid);
	parentid=pid;
    DEBUG_WRITE("wr %d %d %d %d",wr.left,wr.top,wr.right,wr.bottom);	
	DEBUG_WRITE("Window constructed %x of %x ",ID,PID);

}


short Window::InTitleBar(int x,int y)
{
 if(!title_flag)
	return FALSE; 
 return (x>=Left && x<=Right && y>=Top && y<=ScreenY);
}

void Window::showTitleBar(short tf)
{
 
 if(tf){
	 
 int tw=Width-SS_SystemSettings.Window.TitleBarHeight;
 gl_fillbox(Left+1,Top+1,tw-2,SS_SystemSettings.Window.TitleBarHeight
				,(active)?TitleBarColor:
		 SS_SystemSettings.Window.DeactiveColor);
gl_setfontcolors((active)?TitleBarColor:
		SS_SystemSettings.Window.DeactiveColor,TitleTextColor);
 gl_setfont(SS_SystemSettings.Window.TitleFontWidth,SS_SystemSettings.Window.TitleFontHeight,SS_SystemSettings.Window.TitleFont);
 gl_write(Left+4,Top+4,Title);
 gl_hline(Left,Top+SS_SystemSettings.Window.TitleBarHeight+1,Right-1,BorderColor);
 }
 draw3doutbox(Left,Top,Width-1,Height-1);
}



void Window::SetActive(void){
 /*if(!active)*/
	 Message um; 
	 um.Type=SS_SYSTEM_EVENT;
	 um.Event.System.Source_Id=ID&0x0000FFFF;
	 um.Event.System.Destination_Id=ID&0x0000FFFF;
	 um.Event.System.Message=SS_WINDOWACTIVATE;
	 process(&um);
	 active=TRUE; 
	 
  }

void Window::SetInactive(void){
 /*if(active)*/
	 Message um;
	 um.Type=SS_SYSTEM_EVENT;
	 um.Event.System.Source_Id=ID&0x0000FFFF;
	 um.Event.System.Destination_Id=ID&0x0000FFFF;
	 um.Event.System.Message=SS_WINDOWDEACTIVATE;
         process(&um);
	 active=FALSE;
	 
  }

void Window::show(void){
  Message um;
 if(!display_flag||move_flag)
    return;
  DEBUG_WRITE(" In function Window::show() of %x ",ID);
  um.Type=SYSTEM_EVENT;
  um.Event.System.Source_Id=ID&0x0000FFFF;
  um.Event.System.Destination_Id=ID&0x0000FFFF;
  update_report=FALSE;
     if(UpdateFlag/*&&!move_flag*/){
          SS_WindowArea.UpdateValid(*this);
          um.Event.System.Message=SS_WINDOWUPDATE;
          }
     else if(active&&!SS_SystemSettings.MenuActiveFlag){
          SS_WindowArea.SetAreaValid(*this);
          if(1/*!move_flag*/){
           	showTitleBar(title_flag);
            um.Event.System.Message=SS_WINDOWSHOW;
            update_report=TRUE;
            }
          }
    else if(SS_SystemSettings.MenuActiveFlag)
         SS_WindowArea.UpdateValid(*this);
 DEBUG_WRITE("Wnd::show  calling SS::ShowChildObjects");
  parwin=0;
  howto();
  SS_WindowStructure.ShowChildObjects(ID);
//  if(update_report)
//        process(&um);
  DEBUG_WRITE(" Returning from function Window::show() of %x",ID);
  return;
 }


void Window::howto()
{
   if(UpdateFlag/*&&!SS_SystemSettings.MenuActiveFlag*/){
       int i,r;
       for(i=0,r=0;i<iRC;i++,r+=4){
         if(Update[i]==false &&
            wBuffer[r]>=Left && wBuffer[r+1] >=Top &&
            wBuffer[r+2]<=Right && wBuffer[r+3] <=Bottom ){
              Update[i]=true;
	     update_report=TRUE;
              gl_setclippingwindow(wBuffer[r],wBuffer[r+1],
	      		wBuffer[r+2],wBuffer[r+3]);
	          wr.Color(BaseColor) ;
			  /*gl_fillbox(wBuffer[r],wBuffer[r+1],
                         wBuffer[r+2]-wBuffer[r],
                         wBuffer[r+3]-wBuffer[r+1],BaseColor);*/
	         showTitleBar(title_flag);
			 //sleep(2);
             }
          }
     }
   else if(active&&!SS_SystemSettings.MenuActiveFlag){
     SetClipArea(ClipArea);
     wr.Color(BaseColor) ;
     showTitleBar(title_flag);
     }
   else {
         SetClipArea(ClipArea);
         wr.Color(BaseColor) ;
	     showTitleBar(title_flag);
	}
 
 DEBUG_WRITE(" Returning from `function Window::howto() of %x ",ID);
 return;
}


void Window::Hide(void)
{
  display_flag=FALSE;  // set display_flag to null
  return;
}

void Window::EnableDisplay(void) {
	display_flag=TRUE;
}

void Window::set_title(char *text)
{
 if(!title_flag)
   return;	 
 strcpy(Title,text);
 int regions=SS_WindowArea.GetBoundaries(*this,(unsigned int *)wBuffer);
 int l,t,r,b,i,j;
 for(i=0,j=0;i<regions;i++){
   l=wBuffer[j++];
   t=wBuffer[j++];
   r=wBuffer[j++];
   b=wBuffer[j++];
   gl_setclippingwindow(l,t,r,b);
   showTitleBar(title_flag);
   }
 return;
}


void Window::MoveBy(int dx,int dy)
{
 SS_WindowArea.SetAreaInvalid(*this);
 DEBUG_WRITE(" WW::MB Calling SS::GAI");
 int l,t,r,b,ll,tt,rr,bb;
 ll=l=ClipArea.Left;
 tt=t=ClipArea.Top;
 rr=r=ClipArea.Right;
 bb=b=ClipArea.Bottom;
 Win_Objects::MoveBy(dx,dy);
 wr.MoveBy(dx,dy);
 ScreenX+=dx;ScreenY+=dy;
 SS_WindowArea.Link(*this);
 //SS_WindowArea.SetAreaValid(*this);
 SS_WindowStructure.MoveChildObjectsBy(ID,dx,dy);
 /*
if(dy==0){
 l=(dx>0)?l:(r+dx);
 r=(dx>0)?(l+dx):r;
}
if(dx==0){
 t=(dy>0)?t:(b+dy);
 b=(dy>0)?(t+dy):b;
}*/
if(dy==0){
 l=(dx>0)?l:(l+dx);
 r=(dx>0)?(r+dx):r;
}
if(dx==0){
 t=(dy>0)?t:(t+dy);
 b=(dy>0)?(b+dy):b;
}
 
 iRC=SS_WindowArea.GetAreasIn(
        (unsigned)l,(unsigned)t,
        (unsigned)r,(unsigned)b,
        (int *)wBuffer);
 DEBUG_WRITE(" WW::MB Calling SS::US");
 Reset_iRC();
 UpdateFlag=true; 
 show();
 UpdateFlag=false; 
 //move_flag=TRUE;
 SS_WindowStructure.UpdateScreen();
 SS_WindowArea.Unlink(ll,tt,rr,bb,ID);
 //move_flag=FALSE;
 DEBUG_WRITE(" WW::MB returning");
}



void Window::process(Message *GuiM)
{
 int Idout=ID&0x0000FFFF;
  unsigned D;
  if(Idout==GuiM->Event.System.Source_Id&&GuiM->Type==SYSTEM_EVENT)
    D=(Left<<16)|Top;
  else
    D=0;
  if(GuiM->Type==SYSTEM_EVENT
        #ifndef SENDFOCUS
		  && !(GuiM->Event.System.Message ==SS_WINDOWDEACTIVATE 
		      || GuiM->Event.System.Message ==SS_WINDOWDEACTIVATE )
             #endif		  
		  ){
	 SendGUIOutput(SS_SYSTEM_EVENT,procid,
		 Idout,
		 GuiM->Event.System.Source_Id,
		 GuiM->Event.System.Message,
		 D);
        }  
  
   if(GuiM->Type==KEYBOARD_EVENT)
   {
	 SendGUIOutput(SS_INPUT,procid,Idout,
		GuiM->Event.Keyboard.Category,
		GuiM->Event.Keyboard.Control,
		GuiM->Event.Keyboard.Value);
   }
}

 

Window::~Window(){
  DEBUG_WRITE("Destroying WINDOW %x",ID);
  SS_WindowArea.UpdateInvalid(*this);  
  SS_WindowArea.Unlink(*this);
  }


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
SuperStructure::~SuperStructure()
{
 while(Start!=NULL)
	DeleteWindow(Start->Id,0);
}
//--------------------------------------------------------------------------
void SuperStructure::ShowChildObjects(unsigned Id)
{
	WinList *temp ;
	ComponentList *cp ;
	temp=Start ;
	while(temp){
	  //DEBUG_WRITE("SCO Checking %x with  %x ",Id,temp->Id);
	  if(temp->Id==Id){
		  DEBUG_WRITE("Showing components of %x",Id);
		  for(cp=temp->pComponents ; cp!=NULL ;cp=cp->Next )
			 cp->info->show() ;// Showing all the components
		  break;
	  }
	  temp=temp->Next ;
	}
  return ;
} // End  function definition
//----------------------------------------------------------------------------
void SuperStructure::MoveChildObjectsBy(unsigned Id,int dx,int dy)
{
	WinList *temp ;
	ComponentList *cp ;
	temp=Start ;
	while(temp){
	 if(temp->Id==Id){
	  DEBUG_WRITE("Moving components of %x by (%x,%x)",Id,dx,dy);
   	   for(cp=temp->pComponents ; cp!=NULL ;cp=cp->Next ){
	     cp->info->MoveBy(dx,dy) ;
	     }
	   break;
	   }
	 temp=temp->Next ;
	 }
  return ;
} // End  function definition
//---------------------------------------------------------------------------

short SuperStructure::InsertWindowObject(Win_Objects *pO)
  {
	unsigned pid=pO->PID,l,t,r,b ;
	WinList *temp ;
	ComponentList *tempcp ;
	temp=Start ;
        DEBUG_WRITE("In function SS::InsWinObj of %u",pid);
	while(temp){
	if(temp->Id==pid){ // Parent found search for the the child
		 tempcp=new ComponentList ;
		 if(tempcp){
		    DEBUG_WRITE("Adding object %x to %x ",pO->ID,pO->PID);
		    tempcp->info=pO ;
		    tempcp->Prev=NULL;
		    if(temp->pCompEnd==NULL)
			  temp->pCompEnd=tempcp;
		     else
		         temp->pComponents->Prev=tempcp;
		     tempcp->Next=temp->pComponents;
		     temp->pComponents=tempcp ;
		  // add display status
		  if(temp->pWindow->IsDisplayable()){
			  pO->EnableDisplay();
			  pO->show();
		  }
		  else
			  pO->DisableDisplay();
		  // goodie
		   l=tempcp->info->ClipArea.Left;
		   t=tempcp->info->ClipArea.Top;
		   r=tempcp->info->ClipArea.Right;
		   b=tempcp->info->ClipArea.Bottom;
		   Rect R(l,t,r,b);
     	   temp->pWindow->wr.BreakUp(R);  
		  return TRUE ;
		  }  // Added
		}
	 temp=temp->Next ;
         }	
 return 0 ;
} // End function definition
//---------------------------------------------------------------------------

short SuperStructure::DeleteWindowObject(unsigned Id, unsigned Pid)
{
 ComponentList *pC;
  unsigned regions,i,j,l,t,r,b;
 Rectangle ClipArea;
 if(!DoesWindowExist(Pid))
	return FALSE;
 // window found
 for(pC=Anchor->pComponents;pC!=NULL;pC=pC->Next)
  if(pC->info->GetId()==Id){ // object Id found
	DEBUG_WRITE("Deleting WindowOject %x of %x",pC->info->ID,pC->info->PID);
   // goodie
		 l=pC->info->ClipArea.Left;
		 t=pC->info->ClipArea.Top;
		 r=pC->info->ClipArea.Right;
		 b=pC->info->ClipArea.Bottom;
     	 Rect R(l,t,r,b);
		 Anchor->pWindow->wr.Merge(R);  
		 ClipArea=pC->info->ClipArea; 
        // delete window object
		  delete pC->info;
	 // cut links
		if(Anchor->pCompCurrent==pC)
		   Anchor->pCompCurrent=NULL;

		 if(pC==Anchor->pComponents)
			Anchor->pComponents=pC->Next;
		 else
			pC->Prev->Next=pC->Next;

		 if(pC==Anchor->pCompEnd)
			Anchor->pCompEnd=pC->Prev;
		 else
			pC->Next->Prev=pC->Prev;
	  // remove component
		  delete pC;
  regions=SS_WindowArea.GetBoundaries(*(Anchor->pWindow),
					(unsigned int *)wBuffer);
  DEBUG_WRITE(" NO OF REGIONS = %x",regions);
  for(i=j=0;i<regions;i++){
	l=wBuffer[j++];
	t=wBuffer[j++];
	r=wBuffer[j++];
	b=wBuffer[j++];
	if(l<ClipArea.Left)
	    l=ClipArea.Left;
	if(r>ClipArea.Right)
	    r=ClipArea.Right;
	if(t<ClipArea.Top)
	    t=ClipArea.Top;
	 if(b>ClipArea.Bottom)
	      b=ClipArea.Bottom;
	  gl_setclippingwindow(l,t,r,b);
	  gl_fillbox(l,t,r-l+1,b-t+1,
		SS_SystemSettings.Window.BaseColor);
	 }

 return TRUE;
  }// End of if
return FALSE;
}// end function definition


//----------------------------------------------------------------------

short SuperStructure::DeleteWindowComponentList(unsigned Id)
{
 ComponentList *pC,*ppC;
      if(!DoesWindowExist(Id))
	 return FALSE;
	pC=Anchor->pComponents;
	DEBUG_WRITE(" SS::Deleting Components of %x ",Id);
	while(pC){
	   ppC=pC;
	   pC=pC->Next;
	   delete ppC->info;
	   delete ppC;
	 }
  Anchor->pComponents=Anchor->pCompEnd=NULL;
  return TRUE;
}// end function definition

void SuperStructure::DeleteComponentList(WinList *pWL){
   ComponentList *pC,*ppC;
   if(!pWL)
     return;
   pC=pWL->pComponents;
   DEBUG_WRITE(" SS::Deleting Components of WinList %x ",pWL->Id);
   while(pC){
      ppC=pC;
      pC=pC->Next;
      DEBUG_WRITE("SS::DCL Deleting component %x of %x",ppC->info->ID,pWL->Id);
      delete ppC->info;
      delete ppC;
      }
   pWL->pComponents=pWL->pCompEnd=pWL->pCompCurrent=NULL;
   return;
   }
//---------------------------------------------------------------------------
void SuperStructure::GetDimensions(unsigned Id,Rectangle &R){
  if(!DoesWindowExist(Id))
	  return;
  else{
	 R=Anchor->pWindow->ClipArea;
  }
 }// end function definition
//---------------------------------------------------------------------------

short SuperStructure::GetWindowObjectFrameInfo(unsigned Id,unsigned Pid,int *px,int *py,int *pw,int *ph)
{
 ComponentList *pCL;
 if(!DoesWindowExist(Pid))
  return FALSE;
 for(pCL=Anchor->pComponents;pCL;pCL=pCL->Next)
	if(pCL->info->GetId()==Id)
		break;
 if(!pCL)
	return FALSE;
 pCL->info->GetOrigin(px,py);
 pCL->info->GetDimensions(pw,ph);
 return TRUE;
}// end function definition
//---------------------------------------------------------------------------

Win_Objects * SuperStructure:: GetWindowObject(unsigned Id, unsigned Pid)
 {
	 ComponentList *pC;
	 if(!DoesWindowExist(Pid))
		 return NULL;
	// window found
	  for(pC=Anchor->pComponents;pC!=NULL;pC=pC->Next)
			 if(pC->info->GetId()==Id) // object Id found
				return pC->info ;
	  return NULL ; // if object id not found return null
 }// end function definition
//----------------------------------------------------------------------------

void SuperStructure::SetChildObjectActive(unsigned Id,unsigned Pid,bool state)
{
 ComponentList *pC;
 if(!DoesWindowExist(Pid))
	return ;
// window found
  for(pC=Anchor->pComponents;pC!=NULL;pC=pC->Next)
	 if(pC->info->GetId()==Id) // object Id found
		(state)?pC->info->SetActive():pC->info->SetInactive();
 return ;
}// end function definition
//-----------------------------------------------------------------------------
unsigned SuperStructure::GetFocussedId(unsigned Pid){
  if(DoesWindowExist(Pid)&&Anchor->pCompCurrent)
     return Anchor->pCompCurrent->info->GetId();
   else
     return FALSE;
   }  
    
//------------------------------------------------------------------------------
short SuperStructure::SetFocusTo(unsigned Id,unsigned Pid){
  ComponentList *pC;
  if(!DoesWindowExist(Pid))
   return FALSE;
  DEBUG_WRITE(" SetFocus To %u of %u ",Id,Pid);
  for(pC=Anchor->pComponents;pC!=NULL;pC=pC->Next)
    if(pC->info->GetId()==Id){
     DEBUG_WRITE(".... Found setting focus to %u",Id);
     if(Anchor->pCompCurrent)
         Anchor->pCompCurrent->info->LoseFocus();
      Anchor->pCompCurrent=pC;
      Anchor->pCompCurrent->info->GainFocus();
     return TRUE;
     }
  return FALSE;
  }
//----------------------------------------------------------------------------  
/*-----------------------------------------------------------------------------
------------------- Window List Manipulation Routines--------------------------
-------------------------------------------------------------------------------
*/

/* The windows are maintained in a circular list with a current pointer
	CurrentWindow set to the active window */

short SuperStructure::DoesWindowExist(unsigned Id)
 {
  WinList *pWL;
  DEBUG_WRITE("Checking Existence of %x",Id);
  for(pWL=Start;pWL!=NULL&&pWL->Id!=Id;pWL=pWL->Next);
  if(pWL){
	 Anchor=pWL;
	 DEBUG_WRITE("Window found..... ");
	 return TRUE;
	 }
 DEBUG_WRITE("Window not found..... ");
 return FALSE;
 }// end function
//--------------------------------------------------------------------------

short SuperStructure:: IsWindowCurrent(unsigned Id){
	DEBUG_WRITE("Checking if %x is Current Window ",Id);
	if(!CurrentWindow||CurrentWindow->pWindow->GetId()!=Id)
	 return FALSE;
	return TRUE;
  }// end function definition
//----------------------------------------------------------------------------

void SuperStructure::HideWindow(unsigned Id){
  unsigned l,t,r,b;
  WinList *pWL;
  DEBUG_WRITE("\n In .... Hiding Window of %x",Id);	 
  if(DoesWindowExist(Id)){
	 ComponentList *pC;
	 pWL=Anchor;
	 Anchor->pWindow->Hide();
      DEBUG_WRITE("\n In if of Hiding Window of %x",pWL->pWindow->GetId());	 
	  if(Anchor->pWindow->ChildId()){
			 HideWindow(Anchor->pWindow->ChildId());
	  }
	 DEBUG_WRITE("\n Hiding Window %x of %x ",pWL->pWindow->GetId(),
					 pWL->pWindow->ParentId());
	 for(pC=pWL->pComponents;pC!=NULL;pC=pC->Next)
		 pC->info->DisableDisplay();
	 if(CurrentWindow == pWL )
		DeactivateCurrentWindow();	 
	 pWL->pWindow->Hide();
	 l=pWL->pWindow->ClipArea.Left;
	 t=pWL->pWindow->ClipArea.Top;
	 r=pWL->pWindow->ClipArea.Right;
	 b=pWL->pWindow->ClipArea.Bottom;
	 iRC=SS_WindowArea.GetAreasIn((unsigned)l,(unsigned)t,(unsigned)r,
	 	(unsigned)b,(int *)wBuffer);
	   Reset_iRC();
	   UpdateScreen();
	 }
  return;
 } // end function definition
//-----------------------------------------------------------------------------

void SuperStructure::EnableWindowDisplay(unsigned Id){
  if(DoesWindowExist(Id))
	  Anchor->pWindow->EnableDisplay();
  return ;
  }// end function definition
//-----------------------------------------------------------------------------

short SuperStructure::InsertWindow(Window *pW){
	 // We start scanning from the rear to find a window whose processid is =
  	 // that of the new window
     // If found then we write at the end of it
	 // Else we add at the start of the list
	 // We insert the new window at the end of the already existing stack.
	 // No updating on the screen is done at this time and the
	 // Current Window pointer is not disturbed
	 WinList *npWL,*pWL;
	 bool pfound=false;
	 npWL= new WinList;
	 if(!npWL)
		 return FALSE;
	 else
		npWL->pComponents=npWL->pCompEnd=npWL->pCompCurrent=NULL;
	 

	 if(Start==NULL){
		 Start=End=npWL;
		 npWL->Next=NULL;
		 npWL->Prev=NULL;
		 DEBUG_WRITE(" Added first winlist");
		 }
	 else{
		 for(pWL=End;pWL!=NULL;pWL=pWL->Prev)
		    if(pWL->pWindow->GetProcessId()==pW->GetProcessId()){
			  // insert the window after this
				npWL->Next=pWL->Next;
				npWL->Prev=pWL;
				if(pWL==End)
						End=npWL;
				else
						pWL->Next->Prev=npWL;
				pWL->Next=npWL;
				pfound=true;
				break;
			  }
		 
		 if(!pfound){
		   npWL->Next=Start;
		   Start->Prev=npWL;
		   Start=npWL;
	       npWL->Prev=NULL;
           }
        }		 
	 npWL->Id=pW->GetId();
	 npWL->pWindow=pW;
	 DEBUG_WRITE("Inserting Window %x ",npWL->Id);
	/*if(Start== NULL)
	{
		Start = new WinList;
		End = new WinList;
		memcpy(Start,npWL,sizeof(WinList));
		memcpy(End,npWL,sizeof(WinList));
	}*/
	 return TRUE;
	}// End function Insert Window

//-------------------------------------------------------------------------
short SuperStructure::DeleteWindow(unsigned Id,short show,bool notify){
  int l,t,r,b;
  DEBUG_WRITE(" In function DeleteWindow %x",Id);
  if(!DoesWindowExist(Id))
	 return FALSE;
  if(CurrentWindow&&CurrentWindow->Id==Id)
	 DeactivateCurrentWindow();
  l=Anchor->pWindow->ClipArea.Left;
  t=Anchor->pWindow->ClipArea.Top;
  r=Anchor->pWindow->ClipArea.Right;
  b=Anchor->pWindow->ClipArea.Bottom;
  WinList *pWL=Anchor;
  if(notify)
    if(pWL->pWindow->ParentId()&&DoesWindowExist(pWL->pWindow->ParentId()))
		  Anchor->pWindow->SetChildId(0);

  // cut links
	 if(Start==pWL)
		Start=pWL->Next;
	 else
		pWL->Prev->Next=pWL->Next;
	 if(End==pWL)
		End=pWL->Prev;
	 else
		pWL->Next->Prev=pWL->Prev;
  DeleteComponentList(pWL);
  if(pWL->pWindow->ChildId())
		  DeleteWindow(pWL->pWindow->ChildId(),TRUE,false);
  delete pWL->pWindow;
  delete pWL;
  if(show){
   DEBUG_WRITE("SS::DeleteWindows calling SS::UpdateScreen");
   iRC=SS_WindowArea.GetAreasIn((unsigned)l,(unsigned)t,(unsigned)r,(unsigned)b,(int *)wBuffer);
   int i,j;
     for(i=j=0;i<iRC;i++,j+=4){
		       if(wBuffer[j]<l)
			          wBuffer[j]=l;
	           if(wBuffer[j+1]<t)
			          wBuffer[j+1]=t;
			    if(wBuffer[j+2]>r)
			           wBuffer[j+2]=r;
			    if(wBuffer[j+3]>b)
			           wBuffer[j+3]=b;
			   }
	 
   Reset_iRC();UpdateScreen();
   }
   return TRUE;
    
  DEBUG_WRITE(" Returning from function DeleteWindow %x",Id);
  }// End of DeleteWindow function


//-----------------------------------------------------------------------
 short SuperStructure::ShowWindow(unsigned Id){
	  unsigned hproc;
      bool pf=false;	  
	  DEBUG_WRITE("In function Show Window %x",Id);
	    if(CurrentWindow && CurrentWindow->pWindow->GetId()==Id){
	  	   if(SS_SystemSettings.MenuActiveFlag){
	          DEBUG_WRITE("In function Show Window curr match %x",Id);
	          CurrentWindow->pWindow->show();
	          return TRUE;
	         }
	    }
	  if(!DoesWindowExist(Id)){
	    DEBUG_WRITE("ShowWindow win doesnot exist %x",Id);
	    return FALSE;
	    }
	  if(!Anchor->pWindow->ParentId())
			  curr_pwin=Anchor->pWindow->GetId();
	  WinList *pW = Anchor;
	  DEBUG_WRITE(" ShowWindow win %x found ",Id);
	  if((CurrentWindow!=0)&&(CurrentWindow->Id!=Id))
	  {
		 WinList *pW1=CurrentWindow->Prev;
	     if(CurrentWindow!=Start){
				 //cut
		    if(CurrentWindow!=End)
			{
		        CurrentWindow->Next->Prev=CurrentWindow->Prev;
			} 
			else
			{
			    End=CurrentWindow->Prev;
			} 
			 CurrentWindow->Prev->Next=CurrentWindow->Next;
			    //paste
             hproc=CurrentWindow->pWindow->GetProcessId();
	         // insert this pWL in the partial list ,
			 // after already shown windows of 	the current processid
			 for(WinList * pt=Start;!pt && pt!=pW1;pt=pt->Next)
				if(pt->pWindow->GetProcessId()==hproc){
					CurrentWindow->Next=pt->Next;
					CurrentWindow->Prev=pt;
					pt->Next->Prev=CurrentWindow;
					pt->Next=CurrentWindow;
					pf=true;
					break;
				}
	         if(!pf){		 
			    CurrentWindow->Next=Start;
                Start->Prev=CurrentWindow;
                CurrentWindow->Prev=NULL;
                Start=CurrentWindow;
			    }
		     }
		  DEBUG_WRITE("SS::ShowWindow deac CW");
	    DeactivateCurrentWindow();
	    }
	    CurrentWindow=pW;//Anchor;
	    DEBUG_WRITE("SS::ShowWindow ac CW");
	    ActivateCurrentWindow(); 
	    DEBUG_WRITE("SW :: Searching for ChildId %x",pW->pWindow->ChildId());
	    if(pW->pWindow->ChildId()){
			 ShowWindow(pW->pWindow->ChildId());
	    }
				
	 

	    DEBUG_WRITE("Returning from function Show Window %x",Id);
	    return TRUE;
	 }// end of ShowWindow function
//-------------------------------------------------------------------------

void SuperStructure::SetCurrentWindow(unsigned Id){
	DEBUG_WRITE("In function SetCurrent Window %x",Id);
	if(!DoesWindowExist(Id))
	  return;
	if(Anchor->pWindow->IsDisplayable()){
	   DeactivateCurrentWindow();
	   CurrentWindow=Anchor;
	   ActivateCurrentWindow();
	   }
	DEBUG_WRITE(" Returning from function Set Current Window %x",Id);   
  
  } // end of SetCurrentWindow

void SuperStructure::SetCurrentWindow(WinList *pWL){
		 SetCurrentWindow(pWL->Id);
  }

//-------------------------------------------------------------------------

void SuperStructure::SetNextWindowActive(void){
	  WinList *pWL,*pWL1;
	  DEBUG_WRITE("In function SetNextWindowActive");
	  if(!CurrentWindow){
	    for(pWL=Start;pWL!=NULL;pWL=pWL->Next)
	      if(pWL->pWindow->IsDisplayable() && !pWL->pWindow->ParentId())
		 	break;
		if(!pWL)
	    	return;
	    }
	  else{
		 pWL=CurrentWindow;
		do{	  
	      for(pWL=pWL->Next;pWL!=CurrentWindow;pWL=pWL->Next){
		      if(pWL==NULL)
		         pWL=Start;
			  
	          if(!pWL->pWindow->ParentId()&&pWL->pWindow->IsDisplayable()
							  &&pWL->pWindow->GetId()!=curr_pwin)
		        break;
		   }
	      if(pWL==CurrentWindow)
		    return;
	      else{
		    if(CurrentWindow!=Start){
		        //cut
				if(CurrentWindow!=End)
		           CurrentWindow->Next->Prev=CurrentWindow->Prev;
			    else
			       End=CurrentWindow->Prev;
			    CurrentWindow->Prev->Next=CurrentWindow->Next;
               //paste at start
			    
				CurrentWindow->Next=Start;
		        Start->Prev=CurrentWindow;
			    CurrentWindow->Prev=NULL;
			    Start=CurrentWindow;		 
			   }
	        }
		  }while(!pWL->pWindow->IsDisplayable()); 
		 DeactivateCurrentWindow();
		}   
	 DEBUG_WRITE("Setting current window to %x ",pWL->Id);
	 CurrentWindow=pWL;
	 ActivateCurrentWindow();
	 if(pWL->pWindow->ChildId()){
		 ShowWindow(pWL->pWindow->ChildId());
		 //sleep(3);
	 }
	 
	 return;
	}// SetNextWindowActive
//------------------------------------------------------------------------------
void SuperStructure::DeactivateCurrentWindow(void){
     if(!CurrentWindow)
        return;
    DEBUG_WRITE("Deactivating CW %x",CurrentWindow->Id);
    if(CurrentWindow->pCompCurrent){
    	DEBUG_WRITE("Deactivating CW %x CurrentComp",CurrentWindow->Id);
        CurrentWindow->pCompCurrent->info->LoseFocus();
	CurrentWindow->pWindow->SetInactive();
	CurrentWindow->pWindow->show();
	}
     CurrentWindow=NULL;
     
     DEBUG_WRITE("Returning from Deactivating CW ");	
     }// End DeactivateCurrentWindow
//------------------------------------------------------------------------------
void SuperStructure::ActivateCurrentWindow(void){
     if(!CurrentWindow)
          return;
     DEBUG_WRITE("SS::AcCurrWin calling CW->pW->SetActive %x",CurrentWindow->pWindow->GetId());
     CurrentWindow->pWindow->SetActive();
     SS_WindowArea.SetAreaValid(*CurrentWindow->pWindow);
     DEBUG_WRITE("SS::AcCurrWin calling CW->pW->EnableDisplay");
     CurrentWindow->pWindow->EnableDisplay();
     DEBUG_WRITE("SS::AcCurrWin calling CW->pW->show");
     //CurrentWindow->pCompCurrent=NULL;
	 
	 if(!CurrentWindow->pWindow->ParentId())
		 curr_pwin=CurrentWindow->pWindow->GetId();
     //GCLIP;
     //gl_printf(0,0,"\n Setting current 0 level window = %x",curr_pwin);
	 //sleep(5);	 
	 if(CurrentWindow){
	     ComponentList *pC;
	     for(pC=CurrentWindow->pComponents;pC!=NULL;pC=pC->Next)
		     pC->info->EnableDisplay();
	     }
     CurrentWindow->pWindow->show();
	 //CurrentWindow->pCompCurrent=NULL;

     if(CurrentWindow->pCompCurrent){
          CurrentWindow->pCompCurrent->info->GainFocus();
         }  
     
	 else{
        
		 CurrentWindow->pCompCurrent=CurrentWindow->pComponents;
        while(CurrentWindow->pCompCurrent &&
          CurrentWindow->pCompCurrent->info->GetType()==OBJ_LABEL_ID)
              CurrentWindow->pCompCurrent=CurrentWindow->pCompCurrent->Next;
         if(CurrentWindow->pCompCurrent) 
           CurrentWindow->pCompCurrent->info->GainFocus();
	    }
		
     DEBUG_WRITE("Returning from Activating CW %x",CurrentWindow->Id);
     return ;	      
    }// End ActivateCurrentWindow
//------------------------------------------------------------------------------
short SuperStructure::DoesComponentExist(unsigned Id,unsigned Pid){
  ComponentList *pC;
  if(!DoesWindowExist(Pid))
   return -1;
  for(pC=Anchor->pComponents;pC!=NULL;pC=pC->Next)
   if(pC->info->GetId()==Id)
     return TRUE;
  return FALSE;
  }
//-----------------------------------------------------------------------  
void SuperStructure::ShowAllWindows(void){
   WinList *pWL,*pWL1;
   int i=0;
   Win_Objects W(0,0,640,480,0,0);
   DEBUG_WRITE(" In SS::ShowAllWindows");
   gl_setclippingwindow(0,0,SCREEN_MAXX,SCREEN_MAXY);
   SS_WindowArea.SetAreaValid(W);
    gl_fillbox(0,0,SCREEN_MAXX,SCREEN_MAXY,BACKGROUND_COLOR);
    for(pWL=End;pWL!=NULL;pWL=pWL->Prev){
	
           SS_WindowArea.SetAreaValid(*pWL->pWindow);
           pWL->pWindow->show();
           }
    if(!CurrentWindow)  
      SetNextWindowActive();
    else
      ActivateCurrentWindow();
    DEBUG_WRITE("Returning from SS:: ShowAllWindows");
    return;
    
   }
//-----------------------------------------------------------------------------
  void SuperStructure::UpdateScreen(void){
   WinList *pWL,*pWL1;
   int i,r;
   Win_Objects W(0,0,640,480,0,0);
   SS_WindowArea.SetAreaInvalid(W);
   DEBUG_WRITE(" In function Update Screen ");
   UpdateFlag=true;
   if(Start){
      for(pWL=Start;pWL!=NULL;pWL=pWL->Next){
            if(pWL->pWindow->IsDisplayable()){ 
			  DEBUG_WRITE(" US for %x",pWL->Id);
			  pWL->pWindow->show();
			}
	  }
   }
   
  	 
		
   UpdateFlag=false;
    GCLIP;
      for(i=0,r=0;i<iRC;i++,r+=4)
       if(Update[i]==false){
          gl_fillbox(wBuffer[r],wBuffer[r+1],
		  wBuffer[r+2]-wBuffer[r],wBuffer[r+3]-wBuffer[r+1],
		              BACKGROUND_COLOR);
		  Update[i]=true;
		  }
   SS_WindowArea.UpdateValid(W);
   
   if(!CurrentWindow)
     SetNextWindowActive();
   else   
     ActivateCurrentWindow();
			   
   return;
  }
//-------------------------------------------------------------------------
