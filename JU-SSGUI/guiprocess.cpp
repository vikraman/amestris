#include"guiprocess.h"
#include"fontlist.dat"
#include"processinput.h"
#include "guisetup.h"
#include "mesg.h"
#include "message.h"
#include "requestlist.h"
#include "readtbrc.cpp"
#include "winarea.h" 
#include "readresource.cpp"
#include "guiout.h"
#include "debug.h"
#include "printer.c"
#define X_MOVE 20
#define Y_MOVE 15
#include <stdio.h>
extern GraphicsContext *GuiScreenContext;
int z;
#define FEEDBACK_PROC_ID ((unsigned long)GuiRequest.Process_Id|((unsigned long)GuiRequest.Seq<<16))

#define GCLIP gl_setclippingwindow(0,0,SCREEN_MAXX,SCREEN_MAXY)

void InitSystemSettings(void)
{ 
       SS_SystemSettings.Window.BorderColor=8,
       SS_SystemSettings.Window.BaseColor=173,
       SS_SystemSettings.Window.TitleBarColor=1,
       SS_SystemSettings.Window.TitleFontColor=15,
       SS_SystemSettings.Window.DeactiveColor=8;
       SS_SystemSettings.Window.TitleFont=font9;
       SS_SystemSettings.Window.TitleFontHeight=8;
       SS_SystemSettings.Window.TitleFontWidth=8;
       SS_SystemSettings.Window.TitleBarHeight=16;
           
       SS_SystemSettings.Button.ActiveColor=0,
       SS_SystemSettings.Button.DeactiveColor=120,
       SS_SystemSettings.Button.InFocusColor=42,
       SS_SystemSettings.Button.BaseColor=170;
       SS_SystemSettings.Button.BorderColor=250;
       SS_SystemSettings.Button.LabelFont=font9;
       SS_SystemSettings.Button.LabelFontWidth=8;
       SS_SystemSettings.Button.LabelFontHeight=8;
              
       SS_SystemSettings.SystemButton.CrossColor=14;

       SS_SystemSettings.Canvas.TextBackgroundColor=30;
       SS_SystemSettings.Canvas.TextColor=255;
       SS_SystemSettings.Canvas.CanvasColor=30;
       SS_SystemSettings.Canvas.CanvasBorderColor=255;
       SS_SystemSettings.Canvas.CanvasFont=font9;
       SS_SystemSettings.Canvas.CanvasFontWidth=8;
       SS_SystemSettings.Canvas.CanvasFontHeight=8;
       SS_SystemSettings.Menu.FontHeight=8;
       SS_SystemSettings.Menu.FontWidth=8;
       SS_SystemSettings.Menu.Font=font9;
       SS_SystemSettings.Menu.Separator=25;
       SS_SystemSettings.Menu.Color=170;
       SS_SystemSettings.Menu.TextColor=0;
       SS_SystemSettings.Menu.HighlightColor=176;
       SS_SystemSettings.Menu.HighlightTextColor =15;        
      SS_SystemSettings.DefaultFont=font8;
      SS_SystemSettings.DefaultFontHeight=8;
      SS_SystemSettings.DefaultFontWidth=8;
      SS_SystemSettings.InFocusColor=40;
      SS_SystemSettings.BackgroundColor=105;

      SS_SystemSettings.WindowMoveFlag=FALSE;
      SS_SystemSettings.MenuActiveFlag=FALSE;

}


void ReceiveText(int channel ,unsigned procid , char *buffer, int length) 
{
    int start,count ;
    Mesg my_msg ;
    count= 0;
    while(count<length)
     {
       my_msg.mesg_type=procid ;
       mesg_recvb(channel,&my_msg) ;
       DEBUG_WRITE(" \n RT: Recieving %s ",my_msg.mesg_text);
       for(start= 0; start < MAXMESGDATA-1 && count < length;count++,start++)
          *(buffer+count)=my_msg.mesg_text[start] ;
     }
    *(buffer+length)='\0';
return ;
}

void SendMessage(unsigned Id,Message *WinM)
{
 if(SS_WindowStructure.DoesWindowExist(Id))
    SS_WindowStructure.Anchor->pWindow->process(WinM);
 else
   WinM->Type=NULL_EVENT;
}

void SendMessageToCurrentWindow(Message *WinM)
{

if(SS_WindowStructure.CurrentWindow)
    SS_WindowStructure.CurrentWindow->pWindow->process(WinM);
 else
   WinM->Type=NULL_EVENT;
}


void ProcessKeyboard(Message *WinM)
{
 unsigned cid,pid,wid; 
 if(SS_WindowStructure.CurrentWindow)
 { 
///// check if tab
 if(WinM->Event.Keyboard.Category==SS_NORMAL && WinM->Event.Keyboard.Value==9)
 {
   if(SS_WindowStructure.CurrentWindow->pCompCurrent)
       { 
        cid= SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetId();
	wid=SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetParentId();
	wid&=0xffff;
	pid=SS_WindowStructure.CurrentWindow->pWindow->GetProcessId();
#ifdef SENDFOCUS
	DEBUG_WRITE("Sending SS_LOSEFOCUS ");
	SendGUIOutput(FOCUS_EVENT,pid,wid,cid,SS_LOSEFOCUS,0);
#endif	
	SS_WindowStructure.CurrentWindow->pCompCurrent->info->LoseFocus();    
        if(SS_WindowStructure.CurrentWindow->pCompCurrent->Next)
            SS_WindowStructure.CurrentWindow->pCompCurrent
	 		=SS_WindowStructure.CurrentWindow->pCompCurrent->Next;
        else
            SS_WindowStructure.CurrentWindow->pCompCurrent=
	 			SS_WindowStructure.CurrentWindow->pComponents;
       } 
    else
         SS_WindowStructure.CurrentWindow->pCompCurrent
	 			=SS_WindowStructure.CurrentWindow->pComponents;

   while(SS_WindowStructure.CurrentWindow->pCompCurrent &&
		 SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetType()
							==OBJ_LABEL_ID)
   	SS_WindowStructure.CurrentWindow->pCompCurrent=
		SS_WindowStructure.CurrentWindow->pCompCurrent->Next;
   
  if(!SS_WindowStructure.CurrentWindow->pCompCurrent){
     SS_WindowStructure.CurrentWindow->pCompCurrent=
                                SS_WindowStructure.CurrentWindow->pComponents;
       while(SS_WindowStructure.CurrentWindow->pCompCurrent &&
                 SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetType()                                                        ==OBJ_LABEL_ID)
            SS_WindowStructure.CurrentWindow->pCompCurrent=
                SS_WindowStructure.CurrentWindow->pCompCurrent->Next;
     }
 
   if(SS_WindowStructure.CurrentWindow->pCompCurrent){
      SS_WindowStructure.CurrentWindow->pCompCurrent->info->GainFocus();
      cid= SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetId();
      wid=SS_WindowStructure.CurrentWindow->pCompCurrent->info->GetParentId();        wid&=0xffff;
      pid=SS_WindowStructure.CurrentWindow->pWindow->GetProcessId();
#ifdef SENDFOCUS    
      DEBUG_WRITE("Sending SS_GAINFOCUS ");
      SendGUIOutput(FOCUS_EVENT,pid,wid,cid,SS_GAINFOCUS,0);
#endif      
      }

   return;	  
 }	 

////// first pass to the currently focused component;
 if(SS_WindowStructure.CurrentWindow->pCompCurrent)
	SS_WindowStructure.CurrentWindow->pCompCurrent->info->process(WinM);
 ///////////// then to the window
 if(WinM->Type!=NULL_EVENT)
    SS_WindowStructure.CurrentWindow->pWindow->process(WinM);
 }
 WinM->Type=NULL_EVENT; 
 return;
}
 

void Run_Window_System(void) 
{
   int x,y,w,h,wx,wy;
   static int i=0;
   Message GuiM ;
   short WindowMoveFlag=FALSE;
   unsigned CURRENT_MOVE_COLOR=14;
   while(ProgRun)
   {
     GetMessage(&GuiM) ;
     GuiM.Read=FALSE; 
     switch(GuiM.Type)
      {
       case INPUT_IDLE: // Perform maintainence
	      break;
       ///////////////////////////////////////////////////////////////////////
       case REQUEST_EVENT:
                if(WindowMoveFlag==TRUE){
			    if(GuiRequest.Request_Id==RQ_SHOW_WINDOW||
			        GuiRequest.Request_Id==RQ_DESTROY_WINDOW
			        ||GuiRequest.Request_Id==RQ_HIDE_WINDOW){
			        WindowMoveFlag=FALSE;
			        SS_SystemSettings.WindowMoveFlag=FALSE;
			        }
			    }
			 vga_lockvc();
			 ServeGUIRequest();
			 vga_unlockvc();
 	      	 break;
       ////////////////////////////////////////////////////////////////////      
       case KEYBOARD_EVENT:
	if(WindowMoveFlag==TRUE){
	    if(GuiM.Event.Keyboard.Category==SS_NORMAL&&
	       GuiM.Event.Keyboard.Value==KEY_ENTER){
	       SS_WindowStructure.CurrentWindow->pWindow->show();
	       WindowMoveFlag=FALSE;
	       SS_SystemSettings.WindowMoveFlag=FALSE;
	       }
	     else
	       if(GuiM.Event.Keyboard.Category==SS_CURSOR){
	         switch(GuiM.Event.Keyboard.Value){
		    case KEY_ARROWRIGHT:
		        if(x+w+X_MOVE<SCREEN_MAXX){
	            SS_WindowStructure.CurrentWindow->pWindow->MoveBy(X_MOVE,0);
				x+=X_MOVE;
				}
		        break;
		    case KEY_ARROWLEFT:
		        if(x-X_MOVE>=0){
	              SS_WindowStructure.CurrentWindow->pWindow->MoveBy(-X_MOVE,0);
			      x-=X_MOVE;
				}
		        break;
		    case KEY_ARROWUP:
		        if(y-Y_MOVE>=0){
	             SS_WindowStructure.CurrentWindow->pWindow->MoveBy(0,-Y_MOVE);
				 y-=Y_MOVE;
				 }
		        break;
		    case KEY_ARROWDOWN:
		        if(y+h+Y_MOVE<SCREEN_MAXY){
	            SS_WindowStructure.CurrentWindow->pWindow->MoveBy(0,Y_MOVE);
				y+=Y_MOVE;
				}
		        break;
		   }	
	       GCLIP;
		   drawbox(x,y,w-1,h-1,CURRENT_MOVE_COLOR);

	         }
	     break;	 
	    }
	       
	if(GuiM.Event.Keyboard.Category==SS_CURSOR
	       && GuiM.Event.Keyboard.Value==KEY_F10);
	else    
           switch(GuiM.Event.Keyboard.Control)
	      {
	       case SS_ALTKEY:
	            if(GuiM.Event.Keyboard.Value==SS_TAB&&
				    FreezeWindow!=TRUE)
                     SS_WindowStructure.SetNextWindowActive(); 
                       else
                         if(GuiM.Event.Keyboard.Value=='m'&&
			     SS_WindowStructure.CurrentWindow){
			     if(SS_WindowStructure.CurrentWindow->pCompCurrent)
			       SS_WindowStructure.CurrentWindow->pCompCurrent->info->LoseFocus();
			     WindowMoveFlag=TRUE;
			     SS_SystemSettings.WindowMoveFlag=TRUE;
			     SS_WindowStructure.
			     	CurrentWindow->pWindow->GetDimensions(&w,&h);
			     SS_WindowStructure.
			     	CurrentWindow->pWindow->GetOrigin(&wx,&wy);
				CURRENT_MOVE_COLOR=14;
			     GCLIP;
			     drawbox(wx,wy,w-1,h-1,CURRENT_MOVE_COLOR);
			     x=wx,y=wy;
			     }
			 else
			   ProcessKeyboard(&GuiM);
              	    break;		    

               default:
			ProcessKeyboard(&GuiM);
		       break;

	     }		    
           break;  
    //////////////////////////////////////////////////////////////////////////
    default:
          SendMessageToCurrentWindow(&GuiM);
     
  } //End Type Switch     
                                                   
// message hierarchy
// continue sending messages until supervisor is reached or 
// message is invalided...
// message is invalidated by setting Type field in the Message Cell to NULL_EVENT 
// message to parent is conveyed by setting Type field to SYSTEM_EVENT

         while(GuiM.Type==SYSTEM_EVENT
                 &&GuiM.Event.System.Destination_Id!=SUPERVISOR_LEVEL)
             SendMessage(GuiM.Event.System.Destination_Id,&GuiM); 


    } //End of while (ProgRun)
    return ;
}// End of Function



short CreateWindow(unsigned id,unsigned pid,int x,int y,int w,int h,char * text,unsigned processid,int title,int bc, int bdc,int tbc,int ttc)
{
  Window *pW;
  WinList *pWL=NULL;
  SystemButton *pSB; 
  if(SS_WindowStructure.DoesWindowExist(id)){
   DEBUG_WRITE(" CW: Window exists %d",id);	  
   return FALSE;
  }
  if(pid){
	  if(!SS_WindowStructure.DoesWindowExist(pid)||
					  SS_WindowStructure.Anchor->pWindow->ChildId()){
       DEBUG_WRITE(" CW: Parent Window doesnot exist or child already present %d of pid %d",id,pid);	  
   return FALSE;
       }
   pWL=SS_WindowStructure.Anchor;
   }
  if(x+w>640/*SCREEN_MAXX*/||y+h>480/*SCREEN_MAXY*/){
	DEBUG_WRITE("CW: Window off screen limits %d [(%d,%d)-<%d,%d>] max<%d,%d>",id,x,y,w,h,SCREEN_MAXX,SCREEN_MAXY);  
    return FALSE;
  }
  if(WindowArea::WindowCount>=MAX_WINDOWS-1){
	DEBUG_WRITE("CW: Max window count exceeded");  
    return FALSE;
  }
  pW=new Window(id,pid,x,y,w,h,text,processid,
				   (bc)?bc:SS_SystemSettings.Window.BaseColor,
				   (bdc)?bdc:SS_SystemSettings.Window.BorderColor,
				   (tbc)?tbc:SS_SystemSettings.Window.TitleBarColor,
				   (ttc)?ttc:SS_SystemSettings.Window.TitleFontColor
					);
  if(pW)
   {
      if(!SS_WindowStructure.InsertWindow(pW)){
		   	  return FALSE;
	  }
     if(title){
      pSB=new SystemButton(id,
		      x+w-SS_SystemSettings.Window.TitleBarHeight-2,y+2);
     SS_WindowStructure.InsertWindowObject(pSB);
     pW->SetTitleFlag(TRUE);
     }
      if(pWL)
			  pWL->pWindow->SetChildId(id);
     
     WindowArea::WindowCount++;
	 return TRUE;
   }
  return FALSE;
}//End of Function   

short CreateButton(int Id,int Pid,int x,int y,int w,int h,char *text,
				int bc,int bdrc,int ac,int ic)
{
 Button* pB;
 if(SS_WindowStructure.DoesComponentExist(Id,Pid))
   return FALSE;
   
 pB=new Button(Id,Pid,x,y,w,h,text,(bc)?bc:SS_SystemSettings.Button.BaseColor
			   ,(bdrc)?bdrc:SS_SystemSettings.Button.BorderColor	                           ,(ac)?ac:SS_SystemSettings.Button.ActiveColor
                           ,(ic)?ic:SS_SystemSettings.Button.DeactiveColor
                            );
 if(!pB)
  return FALSE;
  return AddWindowObject(pB);
}

short CreateLabel(unsigned id,unsigned pid,int x,int y,int w,int h,char *text,int bc, int fc, unsigned char *fname )
{
  Label* pL ;
 if(SS_WindowStructure.DoesComponentExist(id,pid))
   return FALSE;
  pL=new Label(id,pid,x,y,w,h,text,(bc)?bc:SS_SystemSettings.Window.BaseColor,
                  (fc)?fc:SS_SystemSettings.Button.DeactiveColor,
                   (fname)?fname:SS_SystemSettings.Button.LabelFont);
 if(!pL)
  return FALSE ;
 return AddWindowObject(pL);
}

short CreateCheckBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text)
{
  CheckBox* pCb;
 if(SS_WindowStructure.DoesComponentExist(id,pid))
   return FALSE;
  pCb=new CheckBox(id,pid,x,y,w,h,text);
if(!pCb)
  return FALSE ;
return AddWindowObject(pCb);
}

short GetCheckBoxStatus(unsigned id,unsigned pid,short *pS){
 CheckBox *pCb;
 DEBUG_WRITE(" in function GCBS :");
 if(!SS_WindowStructure.DoesComponentExist(id,pid))
   return FALSE;
 pCb=(CheckBox *)SS_WindowStructure.GetWindowObject(id,pid);
 *pS=0;
 if(!pCb||pCb->GetType()!=OBJ_CHECKBOX_ID)
   return -1;
 *pS=pCb->GetStatus();
 DEBUG_WRITE(" CheckBoxStatus is %d ",*pS);
 return TRUE;
 }  
    
short CreateCanvas(unsigned Id,unsigned Pid,int x,int y,int w,int h,int procid,int scrnc ,int scrbdc )
{
 Canvas *pC;
 if(SS_WindowStructure.DoesComponentExist(Id,Pid))
   return FALSE;
 pC=new Canvas(Id,Pid,x,y,w,h,procid,
      (scrnc)?scrnc:SS_SystemSettings.Canvas.CanvasColor,
      (scrbdc)?scrbdc:SS_SystemSettings.Canvas.CanvasBorderColor
        );
 if(!pC)
  return FALSE;
return AddWindowObject(pC);
}


short CreateTableEditor(unsigned Id,unsigned Pid,int x,int y, char *filename )
{
 TableEditor *pT ;
 if(SS_WindowStructure.DoesComponentExist(Id,Pid))
   return FALSE;
 int i,ret ,h , w=0 , r, c ;
 FieldDescriptor  *fd;
 Field *buffer; 
 char datafilename[30];
 ret=Read_Table_Resource(&r,&c,&fd,datafilename,filename) ;
if(ret!=RC_READ_SUCCESS){
  DEBUG_WRITE("\n Read failure");	
  return FALSE ; 
 }

buffer=new Field[r*c];
DEBUG_WRITE("\n datafilename %s ", datafilename);

if(strlen(datafilename)==0)
	for(i=0 ;i< r*c ;i++)
           buffer[i].Content[0]='\0';
else
    { ret=Read_Data_File(r,c,buffer,fd,datafilename);
      if(ret==DATA_READ_ERR || ret==FILE_OPEN_ERR)
        for(i=0 ;i< r*c ;i++)
             buffer[i].Content[0]='\0';
    }           
       
for(i=0 ;i < c ;i++)
   {
    DEBUG_WRITE("\n %d", fd[i].length);
    w+=(fd[i].length*SS_SystemSettings.Canvas.CanvasFontWidth +4);
   }
  h=r*(SS_SystemSettings.Canvas.CanvasFontHeight+4) ;

  pT=new TableEditor(Id,Pid,x,y,w,h,r,c,fd,buffer);
 if(pT)
   {
     DEBUG_WRITE("td Created") ;
     pT->DrawGrids();
     pT->WriteBuffer();
   }
 else
   DEBUG_WRITE("td can not be Created") ;
//pT->Line(10,20,30,30,60);
if(!pT)
  return FALSE;
return AddWindowObject(pT) ;
     
}

short SaveTableEditor(unsigned Id,unsigned Pid,char *filename) 
{
   int r,c ;
   Field *Buffer ;
   FieldDescriptor *fd ;
   TableEditor *pT ;
   if(SS_WindowStructure.DoesComponentExist(Id,Pid)!=TRUE)
   return FALSE;
   pT=(TableEditor *)SS_WindowStructure.GetWindowObject(Id,Pid);
   if(pT)
     {
       pT->returninfo(&r,&c,&fd,&Buffer);
       if(Save_Data_File(r,c,Buffer, filename)==DATA_WRITE_SUCCESS)
           return TRUE ;
      	   
      } 
   return FALSE ;
}   

short CreateScrollableCanvas(unsigned Id,unsigned Pid,int x,int y,int w,int h,int vw,int vh,int procid,int scrnc ,int scrbdc )
{
 ScrollableCanvas *pC;
 if(SS_WindowStructure.DoesComponentExist(Id,Pid))
   return FALSE;
 pC=new ScrollableCanvas(Id,Pid,x,y,w,h,vw,vh,procid,
      (scrnc)?scrnc:SS_SystemSettings.Canvas.CanvasColor,
      (scrbdc)?scrbdc:SS_SystemSettings.Canvas.CanvasBorderColor
        );
 if(!pC || GE==ALLOCATION_ERROR)
  return FALSE;
return AddWindowObject(pC);
}

short CreateEditBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text)
{
 EditBox *pEB;
 if(SS_WindowStructure.DoesComponentExist(id,pid))
   return FALSE;
 pEB= new EditBox(id,pid,x,y,w,h,text);
 if(!pEB)
  return FALSE;
return AddWindowObject(pEB);
}

short CreatePasswordBox(unsigned id,unsigned pid,int x,int y,int w,int h,char *text)
{
 PasswordBox *pPB;
 if(SS_WindowStructure.DoesComponentExist(id,pid))
   return FALSE;
 pPB= new PasswordBox(id,pid,x,y,w,h,text);
 if(!pPB)
  return FALSE;
return AddWindowObject(pPB);
}

short CreateMenu(unsigned Id,unsigned Pid,int x,int y,int w,int h,char *menufile)
{
 short st;
 MenuBar *pM;
 MenuItem *MI;
 MenuHeader  *MH;
 char * LL;
 unsigned tpd,tmi;
 if(SS_WindowStructure.DoesComponentExist(Id,Pid))
   return FALSE;
 DEBUG_WRITE(" Reading menu definition......");
 st=ReadMenu(menufile,&MH,&MI,&LL,&tpd,&tmi);
 if(st)
  return FALSE;
DEBUG_WRITE(" Now for menu......");
 pM=new MenuBar(Id,Pid,x,y,w,h,MH,MI,LL,tpd,tmi);
 
 if(!pM)
   return FALSE;
 
 DEBUG_WRITE("\n Menu created adding....");
 return AddWindowObject(pM);
}
   

short AddWindowObject(Win_Objects *pO)
{
  unsigned id;
  if((id=pO->GetParentId())!=SUPERVISOR_LEVEL&& SS_WindowStructure.DoesWindowExist(id))
   { 
       DEBUG_WRITE("\n Adding OBJ....");
	   SS_WindowStructure.InsertWindowObject(pO) ;   
    return TRUE;
   }
  return FALSE;
}// End of Function

short ShowWindow(unsigned Id)
{
   if(SS_WindowStructure.ShowWindow(Id))
     return TRUE;
    return FALSE;
}//End of Function   

short DeleteWindow(unsigned Id)
{
 if(SS_WindowStructure.DeleteWindow(Id)){
   WindowArea::WindowCount--;
   return TRUE;
    }
  return FALSE;
}

void ShowAllWindows(void)
{
  SS_WindowStructure.ShowAllWindows();
}


void TextWrite(unsigned Id,unsigned Pid,int x,int y,char *text,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
   if(pi!=cm.end()){
			    pC=pi->second;
  				pC->Write2(x,y,text);
				}
/*	
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_CANVAS_ID)
   return;
 else
  pC=(Canvas *)pCL->info;
  pC->Write2(x,y,text);
  */
return;
}

/*Added On 7th Jan 2002 for addition of new fonts*/

void WriteText(unsigned Id,unsigned Pid,int x,int y,int tc,int tbc,int fno,char *text,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
   if(pi!=cm.end()){
			    pC=pi->second;
  				pC->Write(x,y,tc,tbc,fno,text);
				}
  return;
} 

void TextWriteSc(unsigned Id,unsigned Pid,int x,int y,char *text,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 ScrollableCanvasMap::iterator pi;
 pi=scm.find(kid);
   if(pi!=scm.end()){
	    pC=pi->second;
  	    pC->Write(x,y,1,4,5,text);
	}
 /*
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->Write(x,y,text);
  */
return;
}
 

void SetWindowTitle(unsigned Id,char *text){
     if(!SS_WindowStructure.DoesWindowExist(Id))
         return;
     SS_WindowStructure.Anchor->pWindow->set_title(text);
}

short DeleteComponent(unsigned Id,unsigned Pid){
  return SS_WindowStructure.DeleteWindowObject(Id,Pid);
 }// end procedure

short GetNextComponentInfo(unsigned id,unsigned pid,
					unsigned *nid,unsigned *ntype){
 ComponentList *pC,*pCn; 
 if(!SS_WindowStructure.DoesComponentExist(id,pid))
    return FALSE;
 for(pC=SS_WindowStructure.Anchor->pComponents;pC!=NULL;pC=pC->Next)
   if(pC->info->GetId()==id){
	  pCn=(pC->Next)?(pC->Next):(SS_WindowStructure.Anchor->pComponents);
	  *nid=pCn->info->GetId();
	  *ntype=pCn->info->GetType();
	  return TRUE;
	  }
 }					

void ServeGUIRequest(void){
  char buffer[256], temp[256];
  int chunklength=sizeof(unsigned)*6 ;
  unsigned id,pid,canid;
  int x,y ,w,h,obj_id,mode,tc,tbc,fno;
  EditBox* pE;
  Button* pB ;
  CheckBox *pCB;
  Canvas *pC;
  ScrollableCanvas *pSC;
  Label* pL ;
  MenuBar *pM;
  Win_Objects *pWO;
  CanvasMap::iterator pi;
  ScrollableCanvasMap::iterator psi;
  int i;
  FILE *fp;
 unsigned wid=GuiRequest.Parameters[1] |(GuiRequest.Process_Id<<16);
 unsigned kid=GuiRequest.Parameters[0] |(GuiRequest.Process_Id<<16);
 switch(GuiRequest.Request_Id)
 {
   case RQ_DRAW_TEXT:
   		DEBUG_WRITE("... for text write ");
		//strncpy(buffer,(char *)&GuiRequest.Parameters[4],20) ;
                 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 obj_id=GuiRequest.Parameters[5];
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                           buffer,GuiRequest.Parameters[4] ) ;
                switch(obj_id)
                 {
                      case OBJ_SCANVAS_ID :
                         TextWriteSc(id,pid,x,y,buffer,kid) ;
                         SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			   break ;
                      case OBJ_CANVAS_ID :
                          TextWrite(id,pid,x,y,buffer,kid) ;
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
                         break ; 
                  }

                break ;
    
   case RQ_DRAW_PIXEL:
                   DEBUG_WRITE("... for pixel ");
		   switch(GuiRequest.Parameters[5]){
                    case OBJ_SCANVAS_ID :
                    DrawPixelSc((unsigned)GuiRequest.Parameters[0] ,
			  wid,//(unsigned) GuiRequest.Parameters[1],
			  GuiRequest.Parameters[2],
			  GuiRequest.Parameters[3],
			  GuiRequest.Parameters[4],kid);
		     SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;	 
                    case OBJ_CANVAS_ID :
                      DrawPixel((unsigned)GuiRequest.Parameters[0] ,
                          wid,
                          GuiRequest.Parameters[2],
                          GuiRequest.Parameters[3],
                          GuiRequest.Parameters[4],kid);
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                     }     
                     break ;  
  case RQ_DRAW_CIRCLE:
                   DEBUG_WRITE("... for circle ");
		   switch(GuiRequest.Parameters[6]){
                    case OBJ_SCANVAS_ID :
                    DrawCircleSc((unsigned)GuiRequest.Parameters[0] ,
                          wid,
                          GuiRequest.Parameters[2],
                          GuiRequest.Parameters[3],
                          GuiRequest.Parameters[4],
                          GuiRequest.Parameters[5],kid);
                            SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                    case OBJ_CANVAS_ID :
                      DrawCircle((unsigned)GuiRequest.Parameters[0] ,
                          wid,
                          GuiRequest.Parameters[2],
                          GuiRequest.Parameters[3],
                          GuiRequest.Parameters[4],
                          GuiRequest.Parameters[5],kid);
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                     }
                     break ;
 
  case RQ_DRAW_BAR:
                DEBUG_WRITE("... for bar ");
		switch(GuiRequest.Parameters[7]){
              case OBJ_SCANVAS_ID :
              DrawBarSc((unsigned)GuiRequest.Parameters[0] ,
			  wid,
			  GuiRequest.Parameters[2],
			  GuiRequest.Parameters[3],
			  GuiRequest.Parameters[4],
			  GuiRequest.Parameters[5],
			  GuiRequest.Parameters[6],kid);  
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                   case OBJ_CANVAS_ID :
			  //printf("Server: Request for Draw Bar\n");
                         DrawBar((unsigned)GuiRequest.Parameters[0] ,
                          wid,
                          GuiRequest.Parameters[2],
                          GuiRequest.Parameters[3],
                          GuiRequest.Parameters[4],
                          GuiRequest.Parameters[5],
                          GuiRequest.Parameters[6],kid);	  
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                      }
                    break ;
   
  case RQ_DRAW_BARS:
		   
               DEBUG_WRITE("... for bar ");
		
		for(z =0;z<GuiRequest.Parameters[2];z++)
		{
		switch(GuiRequest.Otherparams[z][7]){
              case OBJ_SCANVAS_ID :
              DrawBarSc((unsigned)GuiRequest.Otherparams[z][0] ,
			  wid,
			  GuiRequest.Otherparams[z][2],
			  GuiRequest.Otherparams[z][3],
			  GuiRequest.Otherparams[z][4],
			  GuiRequest.Otherparams[z][5],
			  GuiRequest.Otherparams[z][6],kid);  
			  break;
              case OBJ_CANVAS_ID :
			  //Drawing Bar
                         DrawBar((unsigned)GuiRequest.Otherparams[z][0] ,
                          wid,
                          GuiRequest.Otherparams[z][2],
                          GuiRequest.Otherparams[z][3],
                          GuiRequest.Otherparams[z][4],
                          GuiRequest.Otherparams[z][5],
                          GuiRequest.Otherparams[z][6],kid);
			 //Writing Text
                	  id=(unsigned)GuiRequest.Parameters[0] ;
                  	  pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                  	  x=GuiRequest.Otherparams[z][2];
                 	  y=GuiRequest.Otherparams[z][3];
	       //added for setting text colors
			  if((pi=cm.find(kid))!=cm.end()){ 		    
		            pC=pi->second;     
	                    pC->SetTextColors(15,
		  		GuiRequest.Otherparams[z][6],
				12);
	                   }
			  //end of set textcolors
                     //TextWrite(id,pid,x+2,y+7,&GuiRequest.labels[0]+30*z,kid);
                       TextWrite(id,pid,x+2,y+2,&GuiRequest.labels[0]+30*z,kid);
			  break;
                      }
		}
		
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Otherparams[0][1],
                               GuiRequest.Otherparams[0][0],0);
	   		       break ;
  

  case RQ_WRITE_TEXTS:
		   
               DEBUG_WRITE("... for texts ");
	       /*
               DrawBar((unsigned)GuiRequest.Otherparams[z][0],wid,
                          GuiRequest.Otherparams[z][2],
                          GuiRequest.Otherparams[z][3],
                          GuiRequest.Otherparams[z][4],
                          GuiRequest.Otherparams[z][5],
                          GuiRequest.Otherparams[z][6],kid);
		*/
		for(z =0;z<GuiRequest.Parameters[2];z++)
		{
		switch(GuiRequest.Otherparams[z][7]){
              case OBJ_SCANVAS_ID :
			  break;
              case OBJ_CANVAS_ID :
                         WriteText((unsigned)GuiRequest.Otherparams[z][0] ,
                          wid,
                          GuiRequest.Otherparams[z][2],
                          GuiRequest.Otherparams[z][3],
                          GuiRequest.Otherparams[z][4],
                          GuiRequest.Otherparams[z][5],
                          GuiRequest.Otherparams[z][6],
			  &GuiRequest.labels[0]+60*z,kid);
			  break;
			}   
		}
		
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Otherparams[0][1],
                               GuiRequest.Otherparams[0][0],0);
	   		       break ;

			       
   case RQ_DRAW_LINE:
                  DEBUG_WRITE("... for line ");
		  switch(GuiRequest.Parameters[7]){
                    case OBJ_SCANVAS_ID :
                          DrawLineSc((unsigned)GuiRequest.Parameters[0] ,
			  wid,
			  GuiRequest.Parameters[2],
			  GuiRequest.Parameters[3],
			  GuiRequest.Parameters[4],
			  GuiRequest.Parameters[5],
			  GuiRequest.Parameters[6],kid);
                            SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break ;
                    case OBJ_CANVAS_ID :
                         DrawLine((unsigned)GuiRequest.Parameters[0] ,
                          wid,
                          GuiRequest.Parameters[2],
                          GuiRequest.Parameters[3],
                          GuiRequest.Parameters[4],
                          GuiRequest.Parameters[5],
                          GuiRequest.Parameters[6],kid);
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;
                         }
                      break ;
					
	   case RQ_CANVAS_SHIFT_DOWN:
			    DEBUG_WRITE("... for Canvas shift down");
	       /*
		       	    pC=(Canvas *)SS_WindowStructure.GetWindowObject
	                                       (GuiRequest.Parameters[0],wid);
               if(!pC||pC->GetType()!=OBJ_CANVAS_ID)
	         pC=NULL;
	       else
	       */
	       if((pi=cm.find(kid))!=cm.end()){ 		    
		  pC=pi->second;     
	          pC->ShiftDownBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
	       }

		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
	         break;   
	   
	   case RQ_CANVAS_SHIFT_RIGHT:
			    DEBUG_WRITE("... for Canvas shift down");
	       /*     pC=(Canvas *)SS_WindowStructure.GetWindowObject
	                                       (GuiRequest.Parameters[0],wid);
               if(!pC||pC->GetType()!=OBJ_CANVAS_ID)
	         <F2>pC=NULL;
	       else
	       */
	       if((pi=cm.find(kid))!=cm.end()){ 		    
		  pC=pi->second;     
	          pC->ShiftRightBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
	       }
	        /*  pC->ShiftRightBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);*/
		SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
	       break;   
	     
	   case RQ_CANVAS_SHIFT_LEFT:
	       DEBUG_WRITE("... for Canvas shift down");
	       if((pi=cm.find(kid))!=cm.end()){ 		    
		  pC=pi->second;     
	          pC->ShiftLeftBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
	       }
	       /*     pC=(Canvas *)SS_WindowStructure.GetWindowObject
	                                       (GuiRequest.Parameters[0],wid);
               if(!pC||pC->GetType()!=OBJ_CANVAS_ID)
	         pC=NULL;
	       else
	          pC->ShiftLeftBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
		*/
	         SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
		 break;
      
	   case RQ_CANVAS_SHIFT_UP:
	      DEBUG_WRITE("... for Canvas shift up");
	        if((pi=cm.find(kid))!=cm.end()){ 		    
		  pC=pi->second;     
	          pC->ShiftUpBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
	       }
	      /* 
	      pC=(Canvas *)SS_WindowStructure.GetWindowObject
	                                       (GuiRequest.Parameters[0],wid);
              if(!pC||pC->GetType()!=OBJ_CANVAS_ID)
	           pC=NULL;
	       else
	          pC->ShiftUpBy(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
		*/		
              SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
		break;   
	   
	   case RQ_SET_CANVAS_TEXT_COLORS:
               DEBUG_WRITE("... for setting canvas text colors" );
	       if((pi=cm.find(kid))!=cm.end()){ 		    
		  pC=pi->second;     
	          pC->SetTextColors(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3],
				GuiRequest.Parameters[4]);
	       }
	       /*pC=(Canvas *)SS_WindowStructure.GetWindowObject
	                                       (GuiRequest.Parameters[0],wid);
               if(!pC||pC->GetType()!=OBJ_CANVAS_ID)
	         pC=NULL;
	       else
	          pC->SetTextColors(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3],
				GuiRequest.Parameters[4]);
				*/
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
	       break;
  
      
      
  case RQ_SET_SCANVAS_TEXT_COLORS:
               DEBUG_WRITE("... for setting scrollable canvas text colors" );
	       /*
	       if((psi=scm.find(kid))!=scm.end()){ 		    
		  pSC=psi->second;     
	          pSC->SetTextColors(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
	       }
	       */
	       pSC=(ScrollableCanvas *)SS_WindowStructure.GetWindowObject(
	       					GuiRequest.Parameters[0],wid);
                if(!pSC||pSC->GetType()!=OBJ_CANVAS_ID)
                  pSC=NULL;
               else
	          pSC->SetTextColors(GuiRequest.Parameters[2],
		  		GuiRequest.Parameters[3]);
				
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
		  break;
                     
  case RQ_SET_TEXT :
                 DEBUG_WRITE("... for text "); 
		 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =(unsigned) GuiRequest.Parameters[1] ;
                 obj_id=GuiRequest.Parameters[2] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                        buffer,GuiRequest.Parameters[3] ) ;
                 switch(obj_id)
                  {
                      case OBJ_WINDOW_ID :
			 SetWindowTitle(id|(GuiRequest.Process_Id<<16),buffer); 
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
			       SS_SUCCESS,GuiRequest.Parameters[1],
				GuiRequest.Parameters[0],0);
			 break ;
                      case OBJ_BUTTON_ID :
			 pB=(Button *)SS_WindowStructure.GetWindowObject(id,wid);
	   	       if(pB&&pB->GetType()==OBJ_BUTTON_ID)
			  pB->SetText(buffer) ;		  
			SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
			       SS_SUCCESS,GuiRequest.Parameters[1],
				GuiRequest.Parameters[0],0);
			   break ;
      		      case OBJ_LABEL_ID : 
			  pL=(Label *)SS_WindowStructure.GetWindowObject(id,wid);
 			  if(pL&&pL->GetType()==OBJ_LABEL_ID);
                            pL->SetLabelText(buffer);
			    SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
			           SS_SUCCESS,GuiRequest.Parameters[1],
                                   GuiRequest.Parameters[0],0);
		          break;
		      case OBJ_EDITBOX_ID:
		         pE=(EditBox *)SS_WindowStructure.GetWindowObject(id,wid);
			 if(pE && pE->GetType()==OBJ_EDITBOX_ID);
			    pE->SetText(buffer);
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  break;  
                   }
 		  break ;          
  case  RQ_CREATE_WINDOW :
          DEBUG_WRITE("... window create ");
	  i=CreateWindow(
		   GuiRequest.Parameters[0]|(GuiRequest.Process_Id<<16),
		   (GuiRequest.Parameters[1])?
		   			(GuiRequest.Parameters[1]|GuiRequest.Process_Id<<16):0,
		   GuiRequest.Parameters[2],
		   GuiRequest.Parameters[3],
		   GuiRequest.Parameters[4],
		   GuiRequest.Parameters[5],
		   "Window",
		   GuiRequest.Process_Id,
		   GuiRequest.Parameters[6]);
	      
             SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
             (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[0],
	     GuiRequest.Parameters[1],0);
	     break ;
   case RQ_CREATE_EDITBOX :
                DEBUG_WRITE("... for edit box ");
		i=CreateEditBox(
                   GuiRequest.Parameters[0],
                   wid,
                   GuiRequest.Parameters[2],
                   GuiRequest.Parameters[3],
                   GuiRequest.Parameters[4],
                   GuiRequest.Parameters[5],
                    "");
                   SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                         (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		         GuiRequest.Parameters[0],0);
                   
              break ;
   case RQ_CREATE_PASSWORDBOX :
                DEBUG_WRITE("... for password box ");
		i=CreatePasswordBox(
                   GuiRequest.Parameters[0],
                   wid,
                   GuiRequest.Parameters[2],
                   GuiRequest.Parameters[3],
                   GuiRequest.Parameters[4],
                   GuiRequest.Parameters[5],
                    "");
                   SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                         (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		         GuiRequest.Parameters[0],0);
                   
              break ;
   case RQ_CREATE_LABEL :
                 DEBUG_WRITE("... for label ");
		 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 w=GuiRequest.Parameters[4] ;
                 h=GuiRequest.Parameters[5] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
			buffer,GuiRequest.Parameters[6] ) ;
                 i=CreateLabel(id,pid,x,y,w,h,buffer);
                 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                    (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		    GuiRequest.Parameters[0],0);
                 break ;
   case RQ_CREATE_BUTTON :
                 DEBUG_WRITE("... for button ");
		 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 w=GuiRequest.Parameters[4] ;
                 h=GuiRequest.Parameters[5] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                        buffer,GuiRequest.Parameters[6] ) ;
                 i=CreateButton(id,pid,x,y,w,h,buffer);
                SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                	(i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		        GuiRequest.Parameters[0],0);
		break ;
   case RQ_MENUBUTTONS:
                 DEBUG_WRITE("... for button group ");
		for(z =0;z<GuiRequest.Parameters[2];z++)
		{
		 
		 id=(unsigned)GuiRequest.Otherparams[z][0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Otherparams[z][2];
                 y=GuiRequest.Otherparams[z][3];
                 w=GuiRequest.Otherparams[z][4];
                 h=GuiRequest.Otherparams[z][5];
		 if(strcmp(&GuiRequest.labels[0]+20*z,"dummy"))
                 CreateButton(id,pid,x,y,w,h,&GuiRequest.labels[0]+20*z);
		 else
                 CreateButton(id,pid,x,y,w,h,"");

		}
		 
		SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                       SS_SUCCESS,GuiRequest.Otherparams[0][1],
                       GuiRequest.Otherparams[0][2],0);
		break;
    case RQ_CREATE_MENU:
               DEBUG_WRITE("... for menu ");
	       id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 w=GuiRequest.Parameters[4] ;
                 h=GuiRequest.Parameters[5] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                        buffer,GuiRequest.Parameters[6] ) ;
                 i=CreateMenu(id,pid,x,y,w,h,buffer);
                SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                     (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		     GuiRequest.Parameters[0],0);
                break ;
 
    case RQ_CREATE_CHECKBOX :
                DEBUG_WRITE("... for check box ");
		id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 w=GuiRequest.Parameters[4] ;
                 h=GuiRequest.Parameters[5] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                        buffer,GuiRequest.Parameters[6] ) ;
                 i=CreateCheckBox(id,pid,x,y,w,h,buffer);
                 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                    (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		    GuiRequest.Parameters[0],0);
                 break ;
     
    case RQ_DESTROY_COMPONENT:
    		 DEBUG_WRITE("... for component destroy ");
		 id=(unsigned)GuiRequest.Parameters[0];
		 i=DeleteComponent(id,wid);
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		       (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		        GuiRequest.Parameters[0],0);
		  break ;
									   
    case RQ_CREATE_TABLE :
                 DEBUG_WRITE("... for table ");
		 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
		                   buffer,GuiRequest.Parameters[4] ) ;
                 i=CreateTableEditor(id,pid,x,y,buffer) ;
		 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		    (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		    GuiRequest.Parameters[0],0);
		 break ;				   
   case RQ_SAVE_TABLE :
                 DEBUG_WRITE("... for saving table ");
		 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                                   buffer,GuiRequest.Parameters[2] ) ;
                 i=SaveTableEditor(id,pid,buffer) ;
                 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                   (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		   GuiRequest.Parameters[0],0);
                 break ;
 
    
    case RQ_CHECKBOX_STATUS :
               short stat;
	       DEBUG_WRITE("... for check box status ");
	       i=GetCheckBoxStatus(GuiRequest.Parameters[0],wid,&stat);
	       SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
	          (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		   GuiRequest.Parameters[0],stat);
	       break;					 
    
    case RQ_CREATE_CANVAS :
               DEBUG_WRITE("... for canvas ");
	       i=CreateCanvas(GuiRequest.Parameters[0] ,
   			wid,
   			GuiRequest.Parameters[2],
   			GuiRequest.Parameters[3], 
   			GuiRequest.Parameters[4] ,
   			GuiRequest.Parameters[5] ,
            GuiRequest.Process_Id,	
   			GuiRequest.Parameters[6] ,
   			GuiRequest.Parameters[7] );
               SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                 (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		    GuiRequest.Parameters[0],0);
                 break ;

                      
   case RQ_CREATE_CANVAS_SC :
               DEBUG_WRITE("... for scroll canvas ");
	       i=CreateScrollableCanvas(GuiRequest.Parameters[0] ,
                        wid,
                        GuiRequest.Parameters[2],
                        GuiRequest.Parameters[3],
                        GuiRequest.Parameters[4],
                        GuiRequest.Parameters[5],
                        GuiRequest.Parameters[6],
                        GuiRequest.Parameters[7],
            GuiRequest.Process_Id,	
                        GuiRequest.Parameters[8],
                        GuiRequest.Parameters[9]);
             /*  printf(" RQ_CRE procid %d seq %d",GuiRequest.Process_Id,
			       					GuiRequest.Seq);               */
								
	       SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                 (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		 GuiRequest.Parameters[0],0);
                 break ;
     
     case RQ_DESTROY_WINDOW :
               DEBUG_WRITE("... for window destroy ");
	     if(SS_SystemSettings.WindowMoveFlag&&
	         SS_WindowStructure.CurrentWindow->
		 	pWindow->GetId()==wid)
		  SendGUIOutput(SS_SYSTEM_BUSY,
                                GuiRequest.Process_Id,
                                SS_FAILURE,GuiRequest.Parameters[0],0,0);

                   
		else{
	          i= DeleteWindow(GuiRequest.Parameters[0]|
		            (GuiRequest.Process_Id<<16));
		   SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		           (i)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[0],0,0);
						     
               }
	       break ;

     
     case RQ_SHOW_WINDOW :
               DEBUG_WRITE("... for window show ");
	       ShowWindow(GuiRequest.Parameters[0]|
	       		(GuiRequest.Process_Id<<16));
	      SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
               break ;

     case RQ_HIDE_WINDOW :
               DEBUG_WRITE("... for window hide ");
	       SS_WindowStructure.HideWindow(kid);
	       SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
               break ;
     
     case RQ_GET_EDIT_TEXT:
                buffer[0]='\0';
                DEBUG_WRITE("... for edit text  ");  
		pE=(EditBox*)SS_WindowStructure.
			GetWindowObject(GuiRequest.Parameters[0],wid);
		if(!pE||pE->GetType()!=OBJ_EDITBOX_ID)
                   pE=NULL;
		else   
                 pE->GetText(buffer);
		 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		    (pE)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[1],
		    GuiRequest.Parameters[0],strlen(buffer));
	      if(pE)
                  TextOut(FEEDBACK_PROC_ID,buffer,strlen(buffer));  
           break;
     
     case RQ_SET_CHECKBOX_STATUS:
                DEBUG_WRITE("... for setting check box with %d ",
					GuiRequest.Parameters[2]);
		pCB=(CheckBox *)SS_WindowStructure.GetWindowObject
				(GuiRequest.Parameters[0],wid);
		if(!pCB||pCB->GetType()!=OBJ_CHECKBOX_ID)
		   pCB=NULL;
		else   
		  pCB->SetStatus(GuiRequest.Parameters[2]);
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
                break;   
      
      
      case RQ_SET_MENU_ITEM_STATE:
               DEBUG_WRITE("... for setting Menu Item State ");
	       pM=(MenuBar *)SS_WindowStructure.GetWindowObject(
     				GuiRequest.Parameters[0],wid);
               if(pM&&pM->GetType()==OBJ_MENUBAR_ID)
	          pM->SetItemState(GuiRequest.Parameters[3],
		  		   GuiRequest.Parameters[2]);
               else
	        DEBUG_WRITE(" Menu %d of %d not found ",
			GuiRequest.Parameters[0],wid);
		SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);	       
               break;
      
      case RQ_SET_MENU_PAGE_STATE:
               DEBUG_WRITE("... for setting Menu Page State ");
	       pM=(MenuBar *)SS_WindowStructure.GetWindowObject(
       					GuiRequest.Parameters[0],wid);
               if(pM&&pM->GetType()==OBJ_MENUBAR_ID)
	          pM->SetPageState(GuiRequest.Parameters[3],
		  		   GuiRequest.Parameters[2]);
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
               break;
      
      case RQ_STATE:
                DEBUG_WRITE("... for state setting");
	        pWO=SS_WindowStructure.GetWindowObject(
					GuiRequest.Parameters[0],wid);
		if(pWO)
		  if(GuiRequest.Parameters[2])
		    pWO->SetActive();
		  else
		    pWO->SetInactive();
		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
		break;    
       
       case RQ_GET_FOCUS:
                 DEBUG_WRITE("... for get focus");
		 i=SS_WindowStructure.GetFocussedId(wid);
		 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		      (i)?SS_SUCCESS:SS_FAILURE,i,0,0);
		 break;
		 
       case RQ_SET_FOCUS:
                 DEBUG_WRITE("... for set focus");
		 id=(unsigned)GuiRequest.Parameters[0]; 
		 i=SS_WindowStructure.SetFocusTo(id,wid);
		 SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
		      (i)?SS_SUCCESS:SS_FAILURE,i,0,0);
		 break;
  
      case RQ_GET_NEXT_COMPONENT_INFO:
	       DEBUG_WRITE("... for next component info");
           unsigned ntype,nid;
           id=(unsigned)GuiRequest.Parameters[0];
		   i=GetNextComponentInfo(id,wid,&nid,&ntype);
		    SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
			     (i)?SS_SUCCESS:SS_FAILURE,nid,ntype,0);
		   break;
       
	   case RQ_GUI_SHUTDOWN:
                DEBUG_WRITE("... for GUI SHUTDOWN");
	        ProgRun=0;
	        break;   		 

	case RQ_PRINT_WINDOW:
                    DEBUG_WRITE("... for PRINT WINDOW");
		    /* Uncomment the line below if parallel printer is used */
		    /*pr=open("/dev/lp0",O_WRONLY);*/
			mode=GuiRequest.Parameters[0] ;
			if(mode==1)
			{	  
                  		  GCLIP;
				  /*
				  Uncomment appropriate call depending on the
				  type of printer used in the order mentioned 
				  below.
				  1.For epson printers
				  2.For HP Printers using PCL(PCL5 and above)
				  3.Any Printer supporting Postscript Language
				  4.Postscript network printer
				  */
				  /*dump_screen();*/
				  /*HP_dump_screen();*/
				  /*PS_dump_screen();*/
				  /*PS_network_dump_screen();*/
				  HP_network_dump_screen();
			}
			if(mode==2)
			{	  
                  		GCLIP;
				/*
				  Uncomment appropriate call depending on the
				  type of printer used in the order mentioned 
				  below.
				  1.For epson printers
				  2.For HP Printers using PCL(preferably PCL5)
				  3.Any Printer supporting Postscript Language
				*/
				/*dump_infile();*/
				HP_dump_infile();
				/*PS_dump_infile();*/
			}
			if(mode==3)
			{	  
				/*
				  Uncomment appropriate call depending on the
				  type of printer used in the order mentioned 
				  below.
				  1.For printer connected to parallel port
				  2.For network printers
				*/
				/*dump_file_to_prn();*/
				dump_file_to_network_prn();
			}
		    /* Uncomment the line below if parallel printer is used */
		       /* close(pr); */				
                        SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                                  SS_SUCCESS,GuiRequest.Parameters[1],
                        GuiRequest.Parameters[0],0);
	      break ;
     
	     /*Added on 7th Jan 2002 for adding new fonts*/

	case RQ_WRITE_TEXT:
       
                 id=(unsigned)GuiRequest.Parameters[0] ;
                 pid =wid;//(unsigned) GuiRequest.Parameters[1] ;
                 x=GuiRequest.Parameters[2] ;
                 y=GuiRequest.Parameters[3] ;
                 tc=GuiRequest.Parameters[4] ;
                 tbc=GuiRequest.Parameters[5] ;
                 fno=GuiRequest.Parameters[6] ;
                 obj_id=GuiRequest.Parameters[8];
                 ReceiveText(RequestQueue,(unsigned)GuiRequest.Process_Id,
                           buffer,GuiRequest.Parameters[7] ) ;
                switch(obj_id)
                 {
                      case OBJ_CANVAS_ID :
                          WriteText(id,pid,x,y,tc,tbc,fno,buffer,kid) ;
			  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
                         break ; 
                  }
                  break;
	      
	 /*   case RQ_FREEZE_WINDOW:
	                 DEBUG_WRITE("... for FREEZE WINDOW") ;
	                FreezeWindow=TRUE  ;
	   	SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                    GuiRequest.Parameters[0],0);
		break;*/
		
	    case RQ_DEFREEZE_WINDOW:
		    DEBUG_WRITE("... for DeFREEZE WINDOW") ;
		    FreezeWindow=FALSE ;
		   SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
	                                  SS_SUCCESS,GuiRequest.Parameters[1],
	                                       GuiRequest.Parameters[0],0);
		   break ;
           
           case RQ_SET_COLOR:
	           DEBUG_WRITE("...for Color Change ");
		 pB=(Button *)SS_WindowStructure.GetWindowObject(
		 		GuiRequest.Parameters[0],wid);
		 if(pB)
		   pB->SetColors(GuiRequest.Parameters[2],
		   		GuiRequest.Parameters[3],
				GuiRequest.Parameters[4],
				GuiRequest.Parameters[5]);

		  SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0); 
		break;		
	          
   case RQ_SET_CANVAS_KEY_STATE:
                  DEBUG_WRITE("... for keystate ");
		  switch(GuiRequest.Parameters[3]){
              case OBJ_SCANVAS_ID :
                     SetKeyStateSc((unsigned)GuiRequest.Parameters[0] ,
			 		 wid, GuiRequest.Parameters[2],kid);
                     SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
			  		break ;
              case OBJ_CANVAS_ID :
                     SetKeyState((unsigned)GuiRequest.Parameters[0] ,
			 		 wid, GuiRequest.Parameters[2],kid);
                     SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,
                               SS_SUCCESS,GuiRequest.Parameters[1],
                                GuiRequest.Parameters[0],0);
                         
                      break ;
	           }
		  break;
   case RQ_GET_COMPONENT_STATE:
		DEBUG_WRITE("...Requesting next component state");
	        pWO=SS_WindowStructure.GetWindowObject(GuiRequest.Parameters[0],wid);//GuiRequest.Parameters[1]);
		SendGUIOutput(SS_FEEDBACK,FEEDBACK_PROC_ID,(pWO)?SS_SUCCESS:SS_FAILURE,GuiRequest.Parameters[0],GuiRequest.Parameters[1],pWO->active);		
		break;
   		default  : DEBUG_WRITE("... for UNKNOWN \a");
                DEBUG_WRITE("\n  Request  was % d\n ",GuiRequest.Request_Id);
		break;
     }// End of switch 
return ;
}
               
   
 
void DrawPixel(unsigned Id,unsigned Pid,int x,int y,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
 if(pi!=cm.end()){
    pC=pi->second;
    pC->Pixel(x,y,color);
    }
	
 /*
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_CANVAS_ID)
   return;
 else
  pC=(Canvas *)pCL->info;
  pC->Pixel(x,y,color);
  */
return;
}
void DrawPixelSc(unsigned Id,unsigned Pid,int x,int y,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 ScrollableCanvasMap::iterator pi;
 pi=scm.find(kid);
 /*
 if(pi!=scm.end()){
    pC=pi->second;
    pC->Pixel(x,y,color);
    }

*/
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->Pixel(x,y,color);

return;
}

void DrawLine(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
  if(pi!=cm.end()){
	   pC=pi->second;
	   pC->Line(x1,y1,x2,y2,color);
	 }
/*	
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_CANVAS_ID)
   return;
 else
  pC=(Canvas *)pCL->info;
  pC->Line(x1,y1,x2,y2,color);
  */
return;
}
void DrawLineSc(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 ScrollableCanvasMap::iterator pi;
 pi=scm.find(kid);
 /*
  if(pi!=scm.end()){
	   pC=pi->second;
	   pC->Line(x1,y1,x2,y2,color);
	 }
*/
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->Line(x1,y1,x2,y2,color);

return;
}
void DrawCircle(unsigned Id,unsigned Pid,int x,int y,int r,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
 if(pi!=cm.end()){
	    pC=pi->second;
            pC->Circle(x,y,r,color);
	 }
/*	
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_CANVAS_ID)
   return;
 else
  pC=(Canvas *)pCL->info;
  pC->Circle(x,y,r,color);
  */
return;
}
void DrawCircleSc(unsigned Id,unsigned Pid,int x,int y,int r,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 ScrollableCanvasMap::iterator pi;
 pi=scm.find(kid);
 /*
 if(pi!=scm.end()){
	    pC=pi->second;
            pC->Circle(x,y,r,color);
	 }
*/
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->Circle(x,y,r,color);

return;
}


void DrawBar(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
 if(pi!=cm.end()){
	pC=pi->second;
    pC->Bar(x1,y1,x2,y2,color);
 }	
 /*
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_CANVAS_ID)
   return;
 else
  pC=(Canvas *)pCL->info;
  pC->Bar(x1,y1,x2,y2,color);
  */
return;
}
void SetKeyState(unsigned Id,unsigned Pid,unsigned kstate,unsigned kid)
{
 ComponentList *pCL;
 Canvas *pC;
 CanvasMap::iterator pi;
 pi=cm.find(kid);
 if(pi!=cm.end()){
	pC=pi->second;
    pC->SetKeyState((kstate)?true:false);
 }	
 return;
}		
void DrawBarSc(unsigned Id,unsigned Pid,int x1,int y1,int x2,int y2,unsigned color,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 ScrollableCanvasMap::iterator pi;
 /*
 pi=scm.find(kid);
 if(pi!=scm.end()){
	pC=pi->second;
    pC->Bar(x1,y1,x2,y2,color);
 }*/
	
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->Bar(x1,y1,x2,y2,color);

return;
}

void SetKeyStateSc(unsigned Id,unsigned Pid,unsigned kstate,unsigned kid)
{
 ComponentList *pCL;
 ScrollableCanvas *pC;
 if(!SS_WindowStructure.DoesWindowExist(Pid))
  return;
 for(pCL=SS_WindowStructure.Anchor->pComponents;pCL;pCL=pCL->Next)
  if(pCL->info->GetId()==Id)
    break;
 if(!pCL||pCL->info->GetType()!=OBJ_SCANVAS_ID)
   return;
 else
  pC=(ScrollableCanvas *)pCL->info;
  pC->SetKeyState((kstate)?true:false);
 
}		
