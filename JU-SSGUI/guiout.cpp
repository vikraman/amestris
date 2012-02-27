#include "guiout.h"
#include <string.h>
#include "vgagl.h"
#include "mesg.h" 
#include "debug.h"
/*  variables */
int goq;
struct smt GuiM;
/* function definitions */

boolean InitOutput(int key){
  sleep(1);
  if((goq=create_mq(key))!=-1)
    return true;
  else
   return false;
  } /* end proc */

  
boolean CloseOutput(void){
  sleep(1);
  remove_mq(goq);
  return true;
  }/* end proc */

boolean SendGUIOutput(unsigned type ,unsigned processid,unsigned A,unsigned B,unsigned C,unsigned D)
{
   static int n=1;
   Mesg out_msg;
    out_msg.mesg_type = processid;
    GuiM.type=type;
    GuiM.A=A;
    GuiM.B=B;
    GuiM.C=C;
    GuiM.D=D;
    memcpy(out_msg.mesg_text,&GuiM,sizeof(struct smt)); 
    DEBUG_WRITE("\n SGO [%d] ::  %d %x %d %d %d %d ",n++,processid,type,A,B,C,D);
    out_msg.mesg_len=sizeof(struct smt);
    mesg_send(goq,&out_msg);
    return true;
}/* end proc */

void TextOut(unsigned procid , char *buffer , int length )
{
   char temp[40] ;
   int start,end ,i;
   Mesg my_msg ;
   my_msg.mesg_type=(long)procid ;
   if(length < MAXMESGDATA-1)
   {
       DEBUG_WRITE("\n TO:: Sending %s ",buffer);
       strncpy(my_msg.mesg_text,buffer,length);
       my_msg.mesg_type=(long)procid ;
       my_msg.mesg_len=strlen(buffer);
       mesg_send(goq,&my_msg);
       return ;
   }
   else
      { start=0 ; end=MAXMESGDATA-2 ;

      my_msg.mesg_type=procid ;
      while(length >0)
       {
         for(i=0 ;start<=end ;start++,i++)
                temp[i]=*(buffer+start) ;
         strncpy(&my_msg.mesg_text[0],temp,MAXMESGDATA-2);
         DEBUG_WRITE(" \n Sending %s",temp);

         my_msg.mesg_len=MAXMESGDATA-1 ;
         mesg_send(goq,&my_msg);
         length-=MAXMESGDATA -1 ;
         start+=MAXMESGDATA -1 ;
         end+=MAXMESGDATA -1 ;
        }
        return ;
      }//End of else
 }// End of function


