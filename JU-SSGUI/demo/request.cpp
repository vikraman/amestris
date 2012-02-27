#include "message.h"
#include "request.h"
#include "requestlist.h"
#include "mesg.h"
#include "stdio.h"
#include "stdlib.h"
int RequestChannel ;
int GMChannel;
int req_seq;
bool CreateRequestQ(int key)
 {
  sleep(1);
  if((RequestChannel=create_mq(key))!=-1)
    return true;
  else
    return false; 
}

bool CreateGOQ(int key)
 {
  if((GMChannel=create_mq(key))!=-1)
    return true;
  else
    return false;
}

short GUILogIn(void){
 if(!CreateRequestQ(9999))
   return 0;
 if(!CreateGOQ(8888))
   return 0;
 req_seq=1;
 return getpid();
 }



bool GetGUIOutput(int Channel,unsigned procid,struct smt *g){
   Mesg my_msg;
  int n;
   my_msg.mesg_type=(req_seq<<16)|procid;
   n=mesg_recvb(Channel,&my_msg);
   if(errno==EIDRM){
     printf("\n %d/GGO:: Connection to server lost .....\n",procid);
     exit(1);
     }
   if(n<0&&errno==ENOMSG)
     return false;

     memcpy(g,my_msg.mesg_text,n);
   return TRUE;
   }

bool GUIOutput(int Channel,unsigned procid,struct smt *g){
   Mesg my_msg;
   int n;
   my_msg.mesg_type=procid;
   n=mesg_recvi(Channel,&my_msg);
   if(errno==EINVAL){
        printf("\n %d/GO:: Connection to server lost .....\n",procid);
        exit(1);
       }
   if(n<0&&errno==ENOMSG)
     return false;
   memcpy(g,my_msg.mesg_text,n);
   return TRUE;
   }


void SendGUIRequest(int Channel , SystemInput sr )
{
   Mesg my_msg;
   if((!req_seq)||(++req_seq > 1<<16))
	   req_seq=1;
	  
  // printf("\nSGR procid =%d ,seq= %d ,req= %d",sr.Process_Id,req_seq,sr.Request_Id);
   my_msg.mesg_type=(req_seq<<16)|sr.Process_Id  ;// Non_KeyBoard process
   my_msg.mesg_text[0]=sr.Request_Id  ;
   memcpy(&my_msg.mesg_text[1],&sr.Parameters[0],sizeof(int)*9) ;
   my_msg.mesg_len=sizeof(SystemInput)-sizeof(unsigned short);
   mesg_send(Channel,&my_msg);
   if(errno==EIDRM){
     printf("\n %d/SGR:: Connection to server lost .....\n",sr.Process_Id );
     exit(1);
     }
   ;
   return ;
}

void TextIn(int channel ,unsigned procid , char *buffer, int length)
{
    int start,count ;
    Mesg my_msg ;
    count= 0;
     while(count<length)
     {
       my_msg.mesg_type=(req_seq<<16)|procid ;
       mesg_recvb(channel,&my_msg) ;
       for(start= 0; start < MAXMESGDATA-1 && count < length;count++,start++){
          *(buffer+count)=my_msg.mesg_text[start] ;
          }
     }
    *(buffer+length)='\0';
return ;
}


void SendText(int Channel , unsigned procid , char *buffer , int length )
{
   char temp[40] ;
   int start,end ,i;
   Mesg my_msg ;
   if(length < MAXMESGDATA-1)
   {
       strncpy(my_msg.mesg_text,buffer,length);
       my_msg.mesg_type=/*(req_seq<<16)|*/procid;
       my_msg.mesg_len=strlen(buffer);
       mesg_send(Channel,&my_msg);
       if(errno==EIDRM){
         printf("\n %d/ST:: Connection to server lost .....\n",procid);
         exit(1);
         }
		      
       return ;
   }
   else
      { 
      start=0 ; end=MAXMESGDATA-2 ;
      while(length >0)
       { 
       	 for(i=0 ;start<=end ;start++,i++)
          	temp[i]=*(buffer+start) ;
         strncpy(&my_msg.mesg_text[0],temp,MAXMESGDATA-2);
         my_msg.mesg_len=MAXMESGDATA-1 ;
         my_msg.mesg_type=/*(req_seq<<16)|*/procid;
         mesg_send(Channel,&my_msg);
         if(errno==EIDRM){
           printf("\n %d/ST:: Connection to server lost .....\n",procid);
           exit(1);
           }
         length-=MAXMESGDATA -1 ;
         start+=MAXMESGDATA -1 ;
         end+=MAXMESGDATA -1 ;
        }
        return ;
      }//End of else
 }// End of function      

int  Create_Canvas(unsigned Id,unsigned Pid,unsigned short procid,int x,int y,int w,int h,int scrnc ,int scrbdc)
{

   SystemInput Request ;
   struct smt g;
   Request.Request_Id=RQ_CREATE_CANVAS;
   Request.Process_Id=procid ;
   Request.Parameters[0]=Id ;
   Request.Parameters[1]=Pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=scrnc ;
   Request.Parameters[7]=scrbdc ;
   SendGUIRequest(RequestChannel,Request) ;
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
}
int Create_ScrollableCanvas(unsigned Id,unsigned Pid,unsigned short procid,int x,int y,int w,int h, int vw, int vh,int scrnc =0,int scrbdc =0)
{
   SystemInput Request ;
   struct smt g;
   Request.Request_Id=RQ_CREATE_CANVAS_SC;
   Request.Process_Id=procid ;
   Request.Parameters[0]=Id ;
   Request.Parameters[1]=Pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=vw ;
   Request.Parameters[7]=vh ;
   Request.Parameters[8]=scrnc ;
   Request.Parameters[9]=scrbdc ;
   SendGUIRequest(RequestChannel,Request) ;
   puts("\n Waiting for confirmation");
   GetGUIOutput(GMChannel,Request.Process_Id,&g);
   puts("\n confirmation obtained");
   
   return g.A;
}

short Create_Table_Editor(unsigned id , unsigned pid,unsigned short procid,int x, int y, char *rcfilename)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_TABLE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=strlen(rcfilename);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned)procid,rcfilename,strlen(rcfilename));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
} 
short Save_Table_Editor(unsigned id, unsigned pid,unsigned short procid,
char *filename)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_SAVE_TABLE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=strlen(filename);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned)procid,filename,strlen(filename));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
   
}



void Draw_Bar(unsigned id,unsigned pid ,unsigned short procid,int tx,int ty,int bx,int by,int color)
{
   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_BAR ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=tx ;
   Request.Parameters[3]=ty ;
   Request.Parameters[4]=bx ;
   Request.Parameters[5]=by ;
   Request.Parameters[6]=color ;
   Request.Parameters[7]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
void Draw_BarSc(unsigned id,unsigned pid ,unsigned short procid,int tx,int ty,int
bx,int by,int color)
{
   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_BAR ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=tx ;
   Request.Parameters[3]=ty ;
   Request.Parameters[4]=bx ;
   Request.Parameters[5]=by ;
   Request.Parameters[6]=color ;
   Request.Parameters[7]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);

    ; 
   return ;
}


void Draw_Line(unsigned id,unsigned pid ,unsigned short procid,int tx,int ty,int bx,int by,int color)
{
   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_LINE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=tx ;
   Request.Parameters[3]=ty ;
   Request.Parameters[4]=bx ;
   Request.Parameters[5]=by ;
   Request.Parameters[6]=color ;
   Request.Parameters[7]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
void Draw_LineSc(unsigned id,unsigned pid ,unsigned short procid,int tx,int ty,int bx,int by,int color)
{
   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_LINE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=tx ;
   Request.Parameters[3]=ty ;
   Request.Parameters[4]=bx ;
   Request.Parameters[5]=by ;
   Request.Parameters[6]=color ;
   Request.Parameters[7]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);

    ; 
   return ;
}

void Draw_Pixel(unsigned id , unsigned pid ,unsigned procid,int x,int y,int color)
{
  SystemInput Request ;
   Request.Request_Id=RQ_DRAW_PIXEL ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=color ;
   Request.Parameters[5]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
    return ;
}
void Draw_PixelSc(unsigned id , unsigned pid ,unsigned procid,int x,int y,int color)
{
  SystemInput Request ;
   Request.Request_Id=RQ_DRAW_PIXEL ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=color ;
   Request.Parameters[5]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
void Draw_Circle(unsigned id , unsigned pid ,unsigned procid,int x,int y,int r ,int color)
{
  SystemInput Request ;
   Request.Request_Id=RQ_DRAW_CIRCLE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=r ;
   Request.Parameters[5]=color ;
   Request.Parameters[6]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
void Draw_CircleSc(unsigned id , unsigned pid ,unsigned procid,int x,int y,int r ,int color)
{
  SystemInput Request ;
   Request.Request_Id=RQ_DRAW_CIRCLE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=r ;
   Request.Parameters[5]=color ;
   Request.Parameters[6]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}

void Set_Canvas_Key_State(unsigned id,unsigned pid,unsigned short procid,short kstate){
  SystemInput Request ;
   Request.Request_Id=RQ_SET_CANVAS_KEY_STATE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=kstate ;
   Request.Parameters[3]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
}

void Set_Canvas_Key_State_Sc(unsigned id,unsigned pid,unsigned short procid,short kstate){
  SystemInput Request ;
   Request.Request_Id=RQ_SET_CANVAS_KEY_STATE ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=kstate ;
   Request.Parameters[3]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request) ;
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
}

int Create_EditBox(unsigned id,unsigned pid,unsigned short procid,int x,int y,int w,int h)
{
   struct smt g;   
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_EDITBOX; 
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   SendGUIRequest(RequestChannel,Request) ;
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
}

int Create_PasswordBox(unsigned id,unsigned pid,unsigned short procid,int x,int y,int w,int h)
{
   struct smt g;   
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_PASSWORDBOX; 
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   SendGUIRequest(RequestChannel,Request) ;
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
}

void Draw_Text(unsigned id,unsigned pid ,unsigned short procid,int x,int y
,char *text)
{
   
   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=strlen(text);
   Request.Parameters[5]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
void Draw_TextSc(unsigned id,unsigned pid ,unsigned short procid,int x,int y
,char *text)
{

   SystemInput Request ;
   Request.Request_Id=RQ_DRAW_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=strlen(text);
   Request.Parameters[5]=OBJ_SCANVAS_ID ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}


 int Set_Text_Button(unsigned id,unsigned pid,unsigned short procid,
                    char * text)
 {
   SystemInput Request ;
   Request.Request_Id=RQ_SET_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=OBJ_BUTTON_ID ;
   Request.Parameters[3]=strlen(text);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   return 1;
}

int Set_Text_Label(unsigned id,unsigned pid,unsigned short procid,
                    char * text)
 {
   SystemInput Request ;
   Request.Request_Id=RQ_SET_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=OBJ_LABEL_ID ;
   Request.Parameters[3]=strlen(text);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   return 1;
}

int Set_Text_EditBox(unsigned id,unsigned pid,unsigned short procid,
                    char * text)
 {
   SystemInput Request ;
   Request.Request_Id=RQ_SET_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=OBJ_EDITBOX_ID ;
   Request.Parameters[3]=strlen(text);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   return 1;
 }  

int Set_Text_PasswordBox(unsigned id,unsigned pid,unsigned short procid,
                    char * text){
	return Set_Text_EditBox(id,pid,procid,text);
	}	    

void Set_Window_Title(unsigned id,unsigned pid ,unsigned short procid,char *text)
{
   SystemInput Request ;
   Request.Request_Id=RQ_SET_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=OBJ_WINDOW_ID ;
   Request.Parameters[3]=strlen(text);
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}
short Get_CheckBox_Status(unsigned id,unsigned pid,unsigned short procid,short *pStatus)
{
  struct smt g;
  SystemInput Request ;
  Request.Request_Id=RQ_CHECKBOX_STATUS;
  Request.Process_Id=procid ;
  Request.Parameters[0]=id ;
  Request.Parameters[1]=pid ;
  SendGUIRequest(RequestChannel,Request);
  GetGUIOutput(GMChannel,procid,&g);
  *pStatus=(short)g.D;
  return g.D;
  
}   

short  Create_Label(unsigned id,unsigned pid,unsigned short procid ,
                       int x,int y , int w, int h,char *text)

{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_LABEL ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=strlen(text) ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
}

short Create_Button(unsigned id,unsigned pid,unsigned short procid ,
                           int x,int y , int w, int h,char *text)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_BUTTON ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=strlen(text) ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;

}

short Create_Menu(unsigned id,unsigned pid,unsigned short procid ,
                           int x,int y , int w, int h,char *filename){
   struct smt g;			   
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_MENU ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=strlen(filename) ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid ,filename ,strlen(filename));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;

}

			   
short Create_CheckBox(unsigned id,unsigned pid,unsigned short procid ,
                        int x,int y , int w, int h,char *text)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_CREATE_CHECKBOX ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=strlen(text) ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;

}

int Delete_Window(unsigned id, unsigned short procid)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_DESTROY_WINDOW ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,procid,&g);
    return g.A;
      
 }
 

void Show_Window(unsigned id, unsigned short procid)
{
     SystemInput Request ;
     Request.Request_Id=RQ_SHOW_WINDOW ;
     Request.Process_Id=procid ;
     Request.Parameters[0]=id ;
     SendGUIRequest(RequestChannel,Request);
     struct smt g;
     GetGUIOutput(GMChannel,procid,&g);
     return ;
 }   

void Hide_Window(unsigned id, unsigned short procid)
{
     SystemInput Request ;
     Request.Request_Id=RQ_HIDE_WINDOW ;
     Request.Process_Id=procid ;
     Request.Parameters[0]=id ;
     SendGUIRequest(RequestChannel,Request);
     struct smt g;
     GetGUIOutput(GMChannel,procid,&g);
     return ;
 }   

int Create_Window(unsigned id , unsigned pid ,unsigned short procid,int x,int y ,int w, int h,int Title)
{
   SystemInput Request ;
   struct smt g;
   unsigned p=procid;
   Request.Request_Id=RQ_CREATE_WINDOW ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=w ;
   Request.Parameters[5]=h ;
   Request.Parameters[6]=Title ;
   /*
   Request.Parameters[7]=bdc ;
   Request.Parameters[8]=tdc ;
   Request.Parameters[9]=ttc ;*/
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,p,&g);
   return g.A;
}


extern int Get_Text(unsigned id,unsigned pid,unsigned short procid,
                    char * Buffer){
   SystemInput Request;
    struct smt g;
    Request.Request_Id=RQ_GET_EDIT_TEXT;
    Request.Process_Id=procid ;
    Request.Parameters[0]=id ;
    Request.Parameters[1]=pid ;
    SendGUIRequest(RequestChannel,Request);
    GetGUIOutput(GMChannel,procid,&g);
    if(g.A)
      TextIn(GMChannel,procid,Buffer,g.D);
	else
	 Buffer[0]='\0';	
    return g.A;
};   

void  Set_CheckBox(unsigned id,unsigned pid,unsigned short procid, short Status){
 SystemInput Request;
  Request.Request_Id=RQ_SET_CHECKBOX_STATUS;
      Request.Process_Id=procid ;
      Request.Parameters[0]=id ;
      Request.Parameters[1]=pid ;
      Request.Parameters[2]=Status;
      SendGUIRequest(RequestChannel,Request);
      struct smt g;
      GetGUIOutput(GMChannel,procid,&g);
      
      }
void Set_Text_Colors(unsigned id,unsigned pid,unsigned short procid ,
      unsigned fc,unsigned bc,unsigned fno=0){
      SystemInput Request;
       Request.Request_Id=RQ_SET_CANVAS_TEXT_COLORS;
       Request.Process_Id=procid ;
       Request.Parameters[0]=id ;
       Request.Parameters[1]=pid ;
       Request.Parameters[2]=fc;
       Request.Parameters[3]=bc;
       Request.Parameters[4]=fno;
       SendGUIRequest(RequestChannel,Request);
       struct smt g;
       GetGUIOutput(GMChannel,procid,&g);
       }
						  
void Set_Text_Colors_Sc(unsigned id,unsigned pid,unsigned short procid ,
      unsigned fc,unsigned bc){
      SystemInput Request;
       Request.Request_Id=RQ_SET_SCANVAS_TEXT_COLORS;
       Request.Process_Id=procid ;
       Request.Parameters[0]=id ;
       Request.Parameters[1]=pid ;
       Request.Parameters[2]=fc;
       Request.Parameters[3]=bc;
       SendGUIRequest(RequestChannel,Request);
       struct smt g;
       GetGUIOutput(GMChannel,procid,&g);
       }
						  
void Set_Component_State(unsigned id,unsigned pid,unsigned short procid,
      short State)
  {
   SystemInput Request;
   Request.Request_Id=RQ_STATE;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=State;
   SendGUIRequest(RequestChannel,Request);
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return;
  }

void Set_Menu_Item(unsigned id,unsigned pid,unsigned short procid,
unsigned Value,short State)
        {
	   SystemInput Request;
	   Request.Request_Id=RQ_SET_MENU_ITEM_STATE;
	   Request.Process_Id=procid ;
	   Request.Parameters[0]=id ;
	   Request.Parameters[1]=pid ;
	   Request.Parameters[2]=Value;
	   Request.Parameters[3]=State;
	   SendGUIRequest(RequestChannel,Request);
           struct smt g;
           GetGUIOutput(GMChannel,procid,&g);
	   return;
         }

void Set_Menu_Page(unsigned id,unsigned pid,unsigned short procid,
unsigned Value,short State)
        {
	   SystemInput Request;
	   Request.Request_Id=RQ_SET_MENU_PAGE_STATE;
	   Request.Process_Id=procid ;
	   Request.Parameters[0]=id ;
	   Request.Parameters[1]=pid ;
	   Request.Parameters[2]=Value;
	   Request.Parameters[3]=State;
	   SendGUIRequest(RequestChannel,Request);
	   struct smt g;
	   GetGUIOutput(GMChannel,procid,&g);
	   return;
	}

short Delete_Window_Component(unsigned id,unsigned pid,unsigned short procid)
{
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_DESTROY_COMPONENT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
 }

short Set_Focus_To(unsigned id,unsigned pid,unsigned short procid){
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_SET_FOCUS ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,procid,&g);
   return g.A;
 }
  
unsigned Get_Focussed_Component(unsigned pid,unsigned short procid){
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_GET_FOCUS ;
   Request.Process_Id=procid ;
   Request.Parameters[1]=pid ;
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,procid,&g);
    return g.B;
 }

short Get_Next_Component_Info(unsigned id,unsigned pid,
                unsigned short procid,unsigned *nextid,unsigned *nexttype){
   struct smt g;
   SystemInput Request ;
   Request.Request_Id=RQ_GET_NEXT_COMPONENT_INFO;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   SendGUIRequest(RequestChannel,Request);
   GetGUIOutput(GMChannel,procid,&g);
   if(g.A){
     *nextid=g.B;
     *nexttype=g.C;
     }  
   else{
     nextid=NULL;
     nexttype=NULL;
    }
   return g.A;
}

void Shift_Up_By(unsigned id,unsigned pid,unsigned short procid,unsigned
		dy,unsigned color){
 SystemInput Request ;
 Request.Request_Id=RQ_CANVAS_SHIFT_UP  ;
 Request.Process_Id=procid ;
 Request.Parameters[0]=id ;
 Request.Parameters[1]=pid ;
 Request.Parameters[2]=dy ;
 Request.Parameters[3]=color ;
 SendGUIRequest(RequestChannel,Request);
 struct smt g;
 GetGUIOutput(GMChannel,procid,&g);

}	

void Shift_Down_By(unsigned id,unsigned pid,unsigned short procid,unsigned dy,unsigned color){

 struct smt g;
 SystemInput Request ;
 Request.Request_Id=RQ_CANVAS_SHIFT_DOWN  ;
 Request.Process_Id=procid ;
 Request.Parameters[0]=id ;
 Request.Parameters[1]=pid ;
 Request.Parameters[2]=dy ;
 Request.Parameters[3]=color ;
 SendGUIRequest(RequestChannel,Request);
 GetGUIOutput(GMChannel,procid,&g);
}

void Shift_Left_By(unsigned id,unsigned pid,unsigned short procid,unsigned dx,unsigned color){

 struct smt g;
 SystemInput Request ;
 Request.Request_Id=RQ_CANVAS_SHIFT_LEFT  ;
 Request.Process_Id=procid ;
 Request.Parameters[0]=id ;
 Request.Parameters[1]=pid ;
 Request.Parameters[2]=dx ;
 Request.Parameters[3]=color ;
 SendGUIRequest(RequestChannel,Request);
 GetGUIOutput(GMChannel,procid,&g);
}

void Shift_Right_By(unsigned id,unsigned pid,unsigned short procid,
					unsigned dx,unsigned color){

 struct smt g;
 SystemInput Request ;
 Request.Request_Id=RQ_CANVAS_SHIFT_RIGHT  ;
 Request.Process_Id=procid ;
 Request.Parameters[0]=id ;
 Request.Parameters[1]=pid ;
 Request.Parameters[2]=dx ;
 Request.Parameters[3]=color ;
 SendGUIRequest(RequestChannel,Request);
 GetGUIOutput(GMChannel,procid,&g);
}

void Set_Button_Colors(unsigned id,unsigned pid,unsigned short procid,
		unsigned FocusColor,unsigned ButtonColor,
		unsigned ActiveFontColor,unsigned InactiveFontColor){

 struct smt g;
 SystemInput Request ;
 Request.Request_Id=RQ_SET_COLOR  ;
 Request.Process_Id=procid ;
 Request.Parameters[0]=id ;
 Request.Parameters[1]=pid ;
 Request.Parameters[2]=FocusColor ;
 Request.Parameters[3]=ButtonColor ;
 Request.Parameters[4]=ActiveFontColor ;
 Request.Parameters[5]=InactiveFontColor ;
 SendGUIRequest(RequestChannel,Request);
 GetGUIOutput(GMChannel,procid,&g);
}

void Shutdown_GUI(void)
{
  struct smt g;
 SystemInput Request ;
 Request.Request_Id=RQ_GUI_SHUTDOWN  ;
 SendGUIRequest(RequestChannel,Request);
}

void  DeFreeze_Window(unsigned short procid)
{
  struct smt g;
  SystemInput Request ;
  Request.Request_Id=RQ_DEFREEZE_WINDOW  ;
  Request.Process_Id=procid ;
  SendGUIRequest(RequestChannel,Request);
  GetGUIOutput(GMChannel,procid,&g);
}

/*void  Freeze_Window(unsigned short procid)
{
  struct smt g;
  SystemInput Request ;
  Request.Process_Id=procid ;
  Request.Request_Id=RQ_FREEZE_WINDOW  ;
  SendGUIRequest(RequestChannel,Request);
  GetGUIOutput(GMChannel,procid,&g);
}*/

void  Print_Window(unsigned short procid,int mode)
{
  struct smt g;
  SystemInput Request ;
	Request.Process_Id=procid ;
	Request.Request_Id=RQ_PRINT_WINDOW  ;
 	Request.Parameters[0]= mode;
	SendGUIRequest(RequestChannel,Request);
	GetGUIOutput(GMChannel,procid,&g);
}

void Write_Text(unsigned id,unsigned pid ,unsigned short procid,int x,int y
,int fc,int bc,int fno,char * text)
{
   
   SystemInput Request ;
   Request.Request_Id=RQ_WRITE_TEXT ;
   Request.Process_Id=procid ;
   Request.Parameters[0]=id ;
   Request.Parameters[1]=pid ;
   Request.Parameters[2]=x ;
   Request.Parameters[3]=y ;
   Request.Parameters[4]=fc;
   Request.Parameters[5]=bc ;
   Request.Parameters[6]=fno ;
   Request.Parameters[7]=strlen(text);
   Request.Parameters[8]=OBJ_CANVAS_ID ;
   SendGUIRequest(RequestChannel,Request);
   SendText(RequestChannel,(unsigned )procid , text,strlen(text));
   struct smt g;
   GetGUIOutput(GMChannel,procid,&g);
   return ;
}

