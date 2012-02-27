#include <string.h>
#include <ctype.h>
#include"debug.h"
#include "menu.h"
#include "winarea.h"
#include "processinput.h"
#define GCLIP gl_setclippingwindow(0,0,SCREEN_MAXX,SCREEN_MAXY)

static void DrawTriangle(int x,int y,int h,int w,bool state,bool color=false){
 int c=(state)?
 	((color)?SS_SystemSettings.Menu.HighlightTextColor
		:SS_SystemSettings.Menu.TextColor)
	:SS_SystemSettings.Menu.Separator;
 gl_line(x,y,x,y+h,c);
 gl_line(x,y,x+w,y+h/2,c);
 gl_line(x+w,y+h/2,x,y+h,c);
 return;
}


static void DrawBar(int x,int y,int w,int h,char *text,
				bool color,int keyoff,bool state=true){
  int kx,ky;
  int fc=(state)?
              ((color)?SS_SystemSettings.Menu.HighlightTextColor
	                      :SS_SystemSettings.Menu.TextColor)
	       :SS_SystemSettings.Menu.Separator;
					   
  ky=y+SS_SystemSettings.Menu.FontHeight+1;
  gl_setfont(SS_SystemSettings.Menu.FontWidth,
  	    SS_SystemSettings.Menu.FontHeight,
            SS_SystemSettings.Menu.Font);
  gl_setfontcolors((color)?SS_SystemSettings.Menu.HighlightColor:
  		    SS_SystemSettings.Menu.Color,fc);
  gl_fillbox(x+1,y-1,w,h,(color)?SS_SystemSettings.Menu.HighlightColor:
  				SS_SystemSettings.Menu.Color);
  gl_write(x+4,y+1,text);
  kx=keyoff*SS_SystemSettings.Menu.FontWidth+x+6;
  gl_hline(kx,ky,kx+4,fc);
  
  return;
  }

///////////////////////////////////////////////////////////////////////	
//////    Popup class definition //////////////////////////////////////

Popup::Popup(unsigned id,unsigned pid,
      MenuHeader *pmh,MenuItem *pmi,
      bool *ps,unsigned short *pp,unsigned short *px,unsigned short *py, 
      unsigned x,unsigned y):Win_Objects(x,y,0,0,id,pid)
      {
      int i,j;
      DEBUG_WRITE(" Constructing popup %d,%d",id,pid);
      pMH=pmh;
      pMI=&pmi[pmh->Index-1];
      pS=ps;pP=pp; pX=px; pY=py;
      CurrentPosition=0,vPosition=0;
      spacing=2*SS_SystemSettings.Menu.FontHeight;
      for(i=0,textwidth=5;i<pMH->Number;i++)
        if(pMI[i].Value&&(j=strlen(pMI[i].Label))>textwidth)
	   textwidth=j;
      DEBUG_WRITE(" Constructing popup 2%d,%d",id,pid);
      int w=(textwidth+4)*(SS_SystemSettings.Menu.FontWidth);
      int h=spacing*(pMH->Number+1);
      Rectangle r;
      SS_WindowStructure.GetDimensions(PID,r);
      if(w>r.Width)
       w=r.Width;
      if(r.Bottom < Top+h)
        h=r.Bottom-Top;
      Win_Objects::SetDimensions(w,h);
      if(r.Right<Left+w)
         Win_Objects::MoveBy(r.Right-Left-w+1,0);
      vNumber=h/spacing-1;
      if(vNumber< pMH->Number)
        vNumber++;
      SS_WindowArea.Link(*this);
      Next=NULL;
      DEBUG_WRITE(" Constructing Popup with %d number of items ",pMH->Number);
      }// end contructor POPUP
      
Popup::~Popup(){
      SS_WindowArea.SetAreaInvalid(*this);
      SS_WindowArea.Unlink(*this);
     // gl_setclippingwindow(Left,Top,Right,Bottom);
     // gl_fillbox(Left,Top,Width-1,Height-1,SS_SystemSettings.Window.BaseColor);
      DEBUG_WRITE(" Destroying popup %d,%d",ID,PID);
      }// end destructor POPUP

void Popup::show(void){
 int i,y,kx,ky,fc;
     // draw box
     DEBUG_WRITE(" Showing popup %d,%d",ID,PID);
     SS_WindowArea.SetAreaValid(*this);
     gl_setclippingwindow(ClipArea.Left,ClipArea.Top,
     			ClipArea.Right,ClipArea.Bottom);
     gl_fillbox(Left,Top,Width-1,Height-1,SS_SystemSettings.Menu.Color);
     draw3doutbox(Left,Top,Width-1,Height-1);
     // draw the items
    gl_setfont(SS_SystemSettings.Menu.FontWidth,
     		SS_SystemSettings.Menu.FontHeight,
                SS_SystemSettings.Menu.Font);
				      
     for(i=vPosition,y=Top+SS_SystemSettings.Menu.FontHeight;
         			i<vPosition+vNumber;i++,y+=spacing){
       if(pMI[i].Value){
	  fc=(pMI[i].State)?SS_SystemSettings.Menu.TextColor
	                 :SS_SystemSettings.Menu.Separator;
	  gl_setfontcolors(SS_SystemSettings.Menu.Color,fc);
	  gl_write(Left+7,y,pMI[i].Label);
	   if(pMI[i].Value<0)
	     DrawTriangle(Right-12,y+2,8,8,pMI[i].State);
       	  kx=pMI[i].Key*SS_SystemSettings.Menu.FontWidth+Left+9;
          ky=y+SS_SystemSettings.Menu.FontHeight;
          gl_hline(kx,ky,kx+4,fc);
	  }  
       else
          draw3dinline(Left+4,y+spacing/2,Right-4,y+spacing/2);
       }
	SetBar(true) ;
      if(vPosition)
        gl_hline(Left+5,Top+3,Right-5,SS_SystemSettings.InFocusColor);
      if(vPosition+vNumber<pMH->Number)
        gl_hline(Left+5,Bottom-3,Right-5,SS_SystemSettings.InFocusColor);
 }// end function how to

void Popup::process(Message *WinM){
    SetClipArea(ClipArea);
    switch(WinM->Type){
      case KEYBOARD_EVENT:
      		switch(WinM->Event.Keyboard.Category){
		   case SS_CURSOR:
		      switch(WinM->Event.Keyboard.Value){
		        case KEY_ARROWUP:
		           SetPositionUp();
		           ReportPosition(false);
			   WinM->Read=TRUE;
			   break;
			case KEY_ARROWDOWN:
			   SetPositionDown();
		           ReportPosition(false);
			   WinM->Read=TRUE;
			   break;
			default:
		           ReportPosition(false);
			   WinM->Read=FALSE;
		      }
		      break;
		
		  case SS_NORMAL:
		   if(WinM->Event.Keyboard.Value==KEY_ESCAPE){
                      ReportPosition(false);
                      WinM->Read=FALSE;
                      }
		    else if(WinM->Event.Keyboard.Value==KEY_ENTER){
		      ReportPosition(true);
		      WinM->Read=TRUE;
		      }
		    else{
		      SetPositionTo(WinM->Event.Keyboard.Value);
		      ReportPosition(false);
		      WinM->Read=TRUE;
		      }
		    break;
		
		 default:
		   ReportPosition(false);
		   WinM->Read=FALSE;
		}
		break;
      
      default :WinM->Read=FALSE;
      }
}// end process

void Popup::ReportPosition(bool state){
    *pS=state;
    *pP=CurrentPosition;
    return;
   }// end func 

void Popup::SetPositionUp(void){
   int i; 
   DEBUG_WRITE("Setting Position Up %d,%d",ID,PID);
   for(i=CurrentPosition-1;i>=0;i--){
      if(pMI[i].Value!=0){
        SetBar(false);// erase old bar
	CurrentPosition=i ;// new position is set
	// recalculate the display if necessary
	if(CurrentPosition>=vPosition)
	  SetBar(true);
	else{
	   vPosition=CurrentPosition;
	   show();
	   }
	return;
	}
    }
  return;
 }// end func

void Popup::SetPositionDown(void){
   int i; 
   DEBUG_WRITE("Setting Position Down %d,%d",ID,PID);
   for(i=CurrentPosition+1;i<pMH->Number;i++){
      if(pMI[i].Value!=0){
        SetBar(false);// erase old bar
	CurrentPosition=i ;// new position is set
	// recalculate the display if necessary
	 if(CurrentPosition < vPosition + vNumber)
		   SetBar(true);
		else{
		  vPosition=CurrentPosition-vNumber+1;
		  show();
		  }
	return;
	}
    }
  return;
 }// end func

void Popup::SetPositionTo(char k){
 int i;
  k=toupper(k); 
  for(i=(CurrentPosition+1)%pMH->Number;i!=CurrentPosition;i=(i+1)%pMH->Number){
       if(pMI[i].Value!=0&&toupper(pMI[i].Label[pMI[i].Key])==k){
        SetBar(false);// erase old bar
        CurrentPosition=i ;// new position is set
        // recalculate the display if necessary
	   if(CurrentPosition>= vPosition&&
		 CurrentPosition < vPosition + vNumber)
		   SetBar(true);
		else{
		  if(CurrentPosition >= vPosition + vNumber)
		   vPosition=CurrentPosition-vNumber+1;
		  else
		    vPosition=CurrentPosition;
		  show();
		  }

        return;
        }
    }
  return;
 }// end func
 


void Popup::SetBar(bool state){
     unsigned short bPosition=CurrentPosition-vPosition;
     DrawBar(Left+3,
	     Top+SS_SystemSettings.Menu.FontHeight+
	          spacing*bPosition-1,
	     Width-8,spacing-2,
	    pMI[CurrentPosition].Label,
	    state, pMI[CurrentPosition].Key,pMI[CurrentPosition].State); 
    if(pMI[CurrentPosition].Value<0)
         DrawTriangle(Right-12,
	 Top+SS_SystemSettings.Menu.FontHeight+spacing*bPosition,
	 8,8,pMI[CurrentPosition].State,state);
    *pX=Left;
    *pY=Top+SS_SystemSettings.Menu.FontHeight+spacing*bPosition;
    return;		    
  } //end 


//////////////////////////////////////////////////////////////////////////
/////// MenuBar Class Definition /////////////////////////////////////////
MenuBar::MenuBar(int id,int pid,int x,int y,int w,int h,MenuHeader *pmh,
        MenuItem *pmi,char *pml,unsigned tpd,unsigned tmi)
	:Win_Objects(x,y,w,h,id,pid)
{
  int i,j;
  DEBUG_WRITE("Constructing MenuBar %d %d start",ID,PID);
  type=OBJ_MENUBAR_ID;
  pMH=pmh;
  pML=pml;
  pMI=&pmi[pmh[0].Index];
  DEBUG_WRITE("Constructing MenuBar %d %d start 2",ID,PID);
  //pMI=pmi;
  TotalPullDowns=tpd;
  TotalMenuItems= tmi;
  DEBUG_WRITE("Constructing MenuBar %d %d start 3",ID,PID);
  CurrentPosition=0,LastPopupId=ID;
  pStackTop=NULL;
  PopupActive=false;
  RedrawFlag=false;
  spacing=20;
  Position=new unsigned short[pMH[0].Number];

  DEBUG_WRITE("Constructing MenuBar %d %d pmh->number %d [%d]",ID,PID,pMH[0].Number,pMH[0].Index);
  int t;
  for(i=0,j=5;i<pMH[0].Number;i++){
  	  DEBUG_WRITE("Scanning value = %d ",pMI[i].Value);
      Position[i]=Left+j;
	  if(pMI[i].Value)
		t=strlen(pMI[i].Label);
	  else
		t=0;
	j+=t*SS_SystemSettings.Menu.FontWidth+spacing;
    }
    DEBUG_WRITE("Constructing MenuBar %d %d end",ID,PID);
 }// end constructor MENUBAR
///////////////////// menu manipulation methods /////////////////////////////
short MenuBar::LoseFocus(void){
   SS_SystemSettings.MenuActiveFlag=FALSE;
   if(PopupActive){
      DeletePopupStack();
      SS_WindowStructure.ShowWindow(PID);
      }
     Infocus=FALSE;
     show();
    return TRUE;
    }

short MenuBar::GainFocus(void){
   Infocus=TRUE;
   //SS_SystemSettings.MenuActiveFlag=TRUE;
   show();
   return TRUE;
   }

void MenuBar::MoveBy(int dx,int dy){
  int i;
  Win_Objects::MoveBy(dx,dy);
  for(i=0;i<pMH->Number;i++)
    Position[i]+=dx;
  return;
  }

void MenuBar::show(void){
  if(PopupActive){
    gl_setclippingwindow(ClipArea.Left,ClipArea.Top,
    	ClipArea.Right,ClipArea.Bottom);
    howto();
    }
  else
    Win_Objects::show();
  }   

void MenuBar::ShowMenuBar(void){  
     int i,kx,ky,fc;
     gl_fillbox(Left,Top,Width,Height,SS_SystemSettings.Menu.Color);
     gl_setfont(SS_SystemSettings.Menu.FontWidth,
     		SS_SystemSettings.Menu.FontHeight,
                SS_SystemSettings.Menu.Font);
     ky=Top+6+SS_SystemSettings.Menu.FontHeight;			      
     for(i=0;i<pMH->Number;i++)
        if(pMI[i].Value){
     	   fc=(pMI[i].State)?SS_SystemSettings.Menu.TextColor
	      :SS_SystemSettings.Menu.Separator;
	   kx=Position[i]+2+pMI[i].Key*SS_SystemSettings.Menu.FontWidth;
	   gl_setfontcolors(SS_SystemSettings.Menu.Color,fc);
           gl_write(Position[i],Top+5,pMI[i].Label);
	   gl_hline(kx,ky,kx+4,fc);
	   }
	else
	   draw3dinline(Position[i]+spacing/2,Top+2,
	           Position[i]+spacing/2,Bottom-2);
      draw3dinline(Left,Top,Right,Top);
      draw3dinline(Left,Bottom,Right,Bottom);
     if(Infocus)
       SetBar(true);
    } 

void MenuBar::howto(void){
     DEBUG_WRITE("MenuBar Howto called ");
     ShowMenuBar();
     if(Infocus){
      // SetBar(true);
       if(PopupActive){
          //SS_SystemSettings.MenuActiveFlag=TRUE;
          DEBUG_WRITE(" MenuBar Calling ShowPopups %d,%d",ID,PID);
	  ShowPopups(pStackTop);
          }
	//else
	  //SS_SystemSettings.MenuActiveFlag=FALSE;
	}  
     
       
    }// End howto	      
	      

void MenuBar::SetPositionLeft(void){
   int i; 
   for(i=CurrentPosition-1;i!=CurrentPosition;i--){
      if(i<0)
        i=pMH->Number-1;
      if(pMI[i].Value!=0){
        SetBar(false);// erase old bar
	CurrentPosition=i ;// new position is set
	Adjust();
	SetBar(true);
	return;
	}
    }
  return;
 }// end func

void MenuBar::SetPositionRight(void){
   int i; 
   for(i=CurrentPosition+1;i!=CurrentPosition;i++){
      if(i>=pMH->Number)
        i=0;
      if(pMI[i].Value!=0){
        SetBar(false);// erase old bar
	CurrentPosition=i ;// new position is set
	Adjust();
	SetBar(true);
	return;
	}
    }
  return;
 }// end func

bool MenuBar::SetPositionTo(char k){
  int i;
  k=toupper(k);
  for(i=(CurrentPosition+1)%pMH->Number;i!=CurrentPosition;i=(i+1)%pMH->Number){
      DEBUG_WRITE(" Setposto : i=%d Curr %d",i,CurrentPosition);
      if(pMI[i].Value!=0&&toupper(pMI[i].Label[pMI[i].Key])==k){
        SetBar(false);// erase old bar
        CurrentPosition=i ;// new position is set
        Adjust();
        SetBar(true);
        return 1;
        }
    }
  return 0;
 }// end func

void MenuBar::Adjust(void){
  short l,r,dl,dr,k;
  l=Position[CurrentPosition]-4;
  r=l+strlen(pMI[CurrentPosition].Label)
  	*SS_SystemSettings.Menu.FontWidth+8;
  dl=l-Left;dr=Right-r;
  if(dl<0){ // l<Left
     for(k=0;k<pMH->Number;k++)
       Position[k]+=-dl;
     howto();  
     }
  else if(dr<0){
     for(k=0;k<pMH->Number;k++)
          Position[k]-=-dr;
     howto();	  
    }
  return;
 } 
  
void MenuBar::SetBar(bool state){
     DrawBar(Position[CurrentPosition]-4,Top+4,
            	    strlen(pMI[CurrentPosition].Label)*
		    SS_SystemSettings.Menu.FontWidth+8,
		    Height-4,
     	      	    pMI[CurrentPosition].Label,
		    state, pMI[CurrentPosition].Key,
		    pMI[CurrentPosition].State); 
    return;		    
  } //end proc

/////////////// input (with popup response ) manipulation methods ////////////
void MenuBar::process(Message *WinM){
    if(PopupActive){
       pStackTop->process(WinM);// send to popup
       if(ProcessPopupResponse(WinM)) // if popup doesnot read
          ProcessMenuBarMsg(WinM);
       } 	 
    else
       ProcessMenuBarMsg(WinM);
    if(RedrawFlag){
      DEBUG_WRITE("MenuBar:Asking for ShowWindow %d %d",ID,PID);
      SS_WindowStructure.ShowWindow(PID);
      RedrawFlag=false;
      if(!PopupActive)
        SS_SystemSettings.MenuActiveFlag=FALSE;
      }
    return;   
   } // end proc 

bool MenuBar::ProcessPopupResponse(Message *WinM){
  if(WinM->Read==TRUE)
     if(PopupStatus==true){ // an enter has been hit
         if(pStackTop->pMI[PopupPosition].State){	
         if(pStackTop->pMI[PopupPosition].Value<0){
		 InsertPopup();
		 WinM->Type=NULL_EVENT;
		 }
	  else{  
		// set system message
		WinM->Type=SYSTEM_EVENT;
		WinM->Event.System.Source_Id=ID;
		WinM->Event.System.Destination_Id=PID;
		WinM->Event.System.Message=pStackTop->pMI[PopupPosition].Value;
		DeletePopup();
		}// end else 
	}	
        return false;
        }// end PopupStatus== true
     else
       return true;
  else // WinM->Read==FALSE 
    if(WinM->Type==KEYBOARD_EVENT){
       if(WinM->Event.Keyboard.Category!=SS_CURSOR){
         if(WinM->Event.Keyboard.Category==SS_NORMAL &&
  		WinM->Event.Keyboard.Value==KEY_ESCAPE){
            DeletePopup();
            WinM->Type=NULL_EVENT;
            return false;
            }
          return true;
          }
       else {
         if(WinM->Event.Keyboard.Value==KEY_ARROWLEFT){
	    DeletePopup();
	    if(pStackTop==NULL){
	      SetPositionLeft();
	      if(pMI[CurrentPosition].Value<0&&
		 pMI[CurrentPosition].State)
	        InsertPopup();
	       } 
	    WinM->Type=NULL_EVENT;
	    return false;
	    }// end ARROWLEFT
	 else if(WinM->Event.Keyboard.Value==KEY_ARROWRIGHT){
	     if(pStackTop->pMI[PopupPosition].Value<0&&
	       pStackTop->pMI[PopupPosition].State)
	        InsertPopup();
	     else{
	        DeletePopupStack();
	        SetPositionRight();
	        if(pMI[CurrentPosition].Value<0
                  &&pMI[CurrentPosition].State)
	               InsertPopup();
	       }
	     WinM->Type=NULL_EVENT;
	     return false;
	     }// end if ARROWRIGHT
	  else{
	     DeletePopupStack();
	     return true;
	     }// end else ARROWRIGHT
	 }// end SS_CURSOR else    
       }// end KEYBOARD_EVENT
     else
       return true;
  }// end proc     
       

void MenuBar::ProcessMenuBarMsg(Message *WinM){
    SetClipArea(ClipArea);
    switch(WinM->Type){
        case KEYBOARD_EVENT:
	   switch(WinM->Event.Keyboard.Category){
	       case SS_CURSOR:
		       //printf("SS_CURSOR\n");
	          switch(WinM->Event.Keyboard.Value){
	               case KEY_ARROWLEFT:
		           SetPositionLeft();
		           WinM->Type=NULL_EVENT;
			   break;
			case KEY_ARROWRIGHT:
			   SetPositionRight();	
		           WinM->Type=NULL_EVENT;
			   break;
			case KEY_ARROWDOWN:
			   if(pMI[CurrentPosition].Value<0&&
                              pMI[CurrentPosition].State&&
				!PopupActive)
				InsertPopup();
		           	WinM->Type=NULL_EVENT;
                           break;
		       default:   
		           break;
		       }// end switch Value
		   break;
		case SS_NORMAL:
		   //printf("SS_NORMAL\n");
		    if(WinM->Event.Keyboard.Value==KEY_ENTER){
		      if(pMI[CurrentPosition].Value>0
			 &&pMI[CurrentPosition].State){
		         WinM->Type=SYSTEM_EVENT;
			 WinM->Event.System.Source_Id=ID;
			 WinM->Event.System.Destination_Id=PID;
			 WinM->Event.System.Message=pMI[CurrentPosition].Value;
			 return;
			 }
		       else{ 
			if(pMI[CurrentPosition].Value<0
			  &&pMI[CurrentPosition].State)
		          InsertPopup();  
		          WinM->Type=NULL_EVENT;
			 }
		       }	
		    else
		       if(!PopupActive)
			{
				if(SetPositionTo(WinM->Event.Keyboard.Value))
		          	WinM->Type=NULL_EVENT;
			}
		     break;   	  

		 default:
		     break; 
		}// end switch category
	   break;    
	default : break;
	} // end switch type     
  return;
  }// end process


////////////// popup stack manipulation routines ////////////////////
void MenuBar::DeletePopupStack(void){
  Popup *p;
  DEBUG_WRITE(" DeletePopupStack Called %d %d",ID,PID);
  if(!pStackTop)
    return;
  else
    while(pStackTop!=NULL){
       p=pStackTop;
       pStackTop=pStackTop->Next;
       delete p;
       }
  PopupActive=false;
  LastPopupId=ID;
  RedrawFlag=true;
  return;
  }//  end func

void MenuBar::ShowPopups(Popup *p){
  if(p){
    DEBUG_WRITE(" ShowPopups Called %d %d",p->ID,p->PID);
    ShowPopups(p->Next);
    SS_WindowArea.SetAreaValid(*p);
    p->show();
    }
  return;
  }// end func

void MenuBar::DeletePopup(void){
  Popup *p;

    DEBUG_WRITE(" Delete Popup called %d %d",ID,PID);
  if(pStackTop){
     p=pStackTop;
     pStackTop=pStackTop->Next;
     LastPopupId--;
     delete p;
     }
  if(pStackTop==NULL)
    PopupActive=false;
   RedrawFlag=true;  
  return;
  }// end func


void MenuBar::InsertPopup(void){
  Popup *p;
    DEBUG_WRITE(" Insert Popup called %d %d",ID,PID);
  if(PopupActive)
     p=new Popup(++LastPopupId,PID,&pMH[-pStackTop->pMI[PopupPosition].Value],
                 pMI,&PopupStatus,&PopupPosition,&PopupX,&PopupY,
		 PopupX+14,PopupY+14);
  else
     p=new Popup(++LastPopupId,PID,&pMH[-pMI[CurrentPosition].Value],pMI,
                 &PopupStatus,&PopupPosition,&PopupX,&PopupY,
		 Position[CurrentPosition]+4,Bottom+2);
  if(p){
     p->Next=pStackTop;
     pStackTop=p;
     p->show();
     PopupActive=true;
     SS_SystemSettings.MenuActiveFlag=TRUE;
     }
  return;
  } // end func

bool MenuBar::SetItemState(bool state,unsigned Value){
  DEBUG_WRITE("Searching among %d menu-items for %u",TotalMenuItems,Value);
  for(int i=0;i<TotalMenuItems;i++)
    if(pMI[i].Value==Value){
       DEBUG_WRITE("...Found setting state to %s",(state)?"true":"false");
      if(pMI[i].State!=state){ 
        pMI[i].State=state;
        show();
        }
       //  Concurrency code goes here
       return true;
       }
 return false;
 }
  
bool MenuBar::SetPageState(bool state,unsigned Value){
  int k,i,j;
  MenuItem* pm=pMI-(pMH->Index);
  DEBUG_WRITE("Searching among %d menu-pages for %u", TotalPullDowns,Value);
  for(i=0;i< TotalPullDowns;i++)
    for(j=0,k=pMH[i].Index;j<pMH[i].Number;j++)
      if(pm[j+k].Value<0 && pMH[-pm[j+k].Value].Value==Value){
       if( pm[j+k].State!=state){
         pm[j+k].State=state;
          DEBUG_WRITE("...Found %u setting state to %s",
			Value,(state)?"true":"false");
         UpdatePages(i,Value);
         }
       return true;
       }
 return false;
 }
  
void MenuBar::UpdatePages(int i,unsigned Value){
   int k;
   Popup *p,*p1;
   if(!PopupActive){
       show();
     }  
   else{ // PopupActive is true
     if(i==0){
       if(pMI[CurrentPosition].Value<0
          	&&pMH[-pMI[CurrentPosition].Value].Value==Value){
	 DeletePopupStack();
	 RedrawFlag==false;
         SS_WindowStructure.ShowWindow(PID);
	 }
        else{
          SetClipArea(ClipArea);
	  ShowMenuBar();
	  }
       }
     else  
      for(p=pStackTop;p!=NULL;p=p->Next){
       if(p->pMH->Value==Value){
         if(p->Next==NULL)
	    DeletePopupStack();
	 else {   
	    p1=pStackTop;
	    pStackTop=p->Next;
	    p->Next=NULL;
	    for(p=p1;p!=NULL;p=p1){
	       p1=p->Next;
	       delete p;
	       }
	    }
	 RedrawFlag==true;
	 if(!PopupActive)
	   SS_SystemSettings.MenuActiveFlag=FALSE;
	 SS_WindowStructure.ShowWindow(PID);
	 break;
         }// end match;
       }// end scan for
     } // PopupActive is true
  }// end function 
