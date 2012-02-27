#include "table.h"


TableEditor::TableEditor(unsigned id, unsigned pid , int x, int y , int w, int h, int r,int c,FieldDescriptor *fd , Field  *info): Canvas(id,pid,x,y,w,h,0,
SS_SystemSettings.Canvas.CanvasColor,SS_SystemSettings.Canvas.CanvasBorderColor)
{
   int i ;
   Buffer=info ;
   CurrentRow=CurrentColumn=0 ;
   EditMode=false ;
   Rows=r ;
   Columns=c ;
   Theader=fd;
   int sum=0 ; 
  units=SS_SystemSettings.Canvas.CanvasFontWidth ;
  barh=SS_SystemSettings.Canvas.CanvasFontHeight+4 ;
  cpx=cpy=0 ;
  CellPosition=0 ;
  barw=Theader[CurrentColumn].length*units+4;
  
  chpos=0 ;

}
void TableEditor::howto(void)
{
 gl_copyboxfromcontext(vgc,0,0,Width,Height,Left,Top);
 if(Infocus){
   draw3dinbox(Left,Top,Width,Height);
   }
   else{
	 drawbox(Left,Top,Width,Height,BorderColor);
	 drawbox(Left+1,Top+1,Width+14,Height+14,BorderColor);
	 drawbox(Left,Top,Width,Height,27);
        } 
   
}
void TableEditor::WriteBuffer()
{  
   int i ,j ;
   cpy=0 ;
   CellPosition=0 ;
   for(i=0 ;i < Rows ;i++)
     {
      cpx=0 ;
      for(j=0 ;j< Columns ;j++)
      {
       barw=Theader[j].length*units+4;
       CellDraw(false);
       //sleep(1);
       cpx+=barw ;
       CellPosition++ ;
      }
      cpy+=barh ;
     } 
  CellPosition=cpx=cpy=0 ;
  barw=Theader[CurrentColumn].length*units+4;

}  
void TableEditor::DrawGrids(void )
{
   int i , sum=0 ; 
   for(i=0 ;i < Columns;i++)
    {
       sum+=(Theader[i].length*SS_SystemSettings.Canvas.CanvasFontWidth+4) ;
       Line(sum,0,sum,Width,0);
    }
   for(i=1 ;i< Rows ;i++)
    {
      sum=i*(SS_SystemSettings.Canvas.CanvasFontHeight+4) ;
     Line(0,sum,Width,sum,0);
    }
 
 
}
void TableEditor::changetablestate(int col)
{
 if(col<Columns)
  Theader[col].locked=(Theader[col].locked==UNLOCKED?LOCKED:UNLOCKED);
 return ;
} 
void TableEditor::CellDraw(bool highlight)
{
   int i ;
    char pbuffer[MAX_FIELD_LENGTH+1]; 
        strcpy(pbuffer,(Buffer+CellPosition)->Content);//,MAX_FIELD_LENGTH);
	//pbuffer[Theader[CurrentColumn].length]=0 ;
   if(highlight)
     { 
      if(Theader[CurrentColumn].locked==UNLOCKED)
           i=90 ;
      else
           i=39 ;
     Bar(cpx+1,cpy+1,barw-1,barh-1,i);
     }
   else
     { i=SS_SystemSettings.Canvas.CanvasColor ; 
       Bar(cpx+1,cpy+1,barw-1,barh-1,SS_SystemSettings.Canvas.CanvasColor);
     }
   if(!EditMode)      
	 Write(cpx+2,cpy+2,pbuffer,i);
   else
        { 
               Write(cpx+2,cpy+2,pbuffer,i);
               Line(cpx+(vpos)*units+1,cpy+1,cpx+(vpos)*units+1,cpy+barh-1,4) ;
        }
}       
void TableEditor::Write(int x,int y,char *text,int backcolor)
{
  gl_setfontcolors(backcolor,TextColor);
  gl_setfont(SS_SystemSettings.Canvas.CanvasFontWidth,
  SS_SystemSettings.Canvas.CanvasFontHeight,
  SS_SystemSettings.Canvas.CanvasFont);
  gl_setcontext(vgc);
  gl_write(x,y,text);
  gl_setcontext(cgc);
}// End Proc
void TableEditor::returninfo(int *row , int *col , FieldDescriptor **fp, Field **buffer)
{
  *row=Rows ;
  *col=Columns ;
  *fp=Theader ;
  *buffer=Buffer ;
  return ;
}  
   
void TableEditor::process(Message *WinM)
{
 char pbuff[MAX_FIELD_LENGTH+1] ; 
if(WinM->Type==KEYBOARD_EVENT )
   {
     switch(WinM->Event.Keyboard.Category)
    {
      case SS_CURSORBLOCK :
  	
     switch(WinM->Event.Keyboard.Value)
     {
        case KEY_ARROWLEFT:  
             if(!EditMode)
             {
              if(CurrentColumn!=0)
                 {
                       //Bar(cpx+1,cpy+1,barw-1,barh-1,SS_SystemSettings.Canvas.CanvasColor);
		      CellDraw(false);	

                       CellPosition-- ;
                       CurrentColumn-- ;
		       barw=Theader[CurrentColumn].length*units+4;
                       cpx-=barw ;
                       CellDraw(true);
                  }
              }
              else 
               {   // While Editing 
                   if(vpos!=0)
                      {
                         vpos-- ;
                         CellDraw(true);
                       }
                 }             
              break ;

        case KEY_ARROWRIGHT:   
                     if(!EditMode)
              {
              
              if(CurrentColumn!=(Columns-1))  
              	{
                    CellDraw(false) ;
		    CellPosition++ ;
                   CurrentColumn++ ; 
                  cpx+=barw ;
		  barw=Theader[CurrentColumn].length*units+4;
                  //cpx+=barw ;
                  CellDraw(true);
                }
              }
              else 
               {   // While Editing 
                   if(vpos!=chpos)//Theader[CurrentColumn].length)
                      {
                         vpos++ ;
                         CellDraw(true); 

                       }
                 }             
              break ;         

        case KEY_ARROWUP:   
             if(!EditMode)
             {
              
              if(CurrentRow)
               {
              CellDraw(false) ;
	      CurrentRow--;
              CellPosition-=Columns ;
               barw=Theader[CurrentColumn].length*units+4;
                  cpy-=barh ;
               CellDraw(true);
              }
              }  
              break ;


        case KEY_ARROWDOWN:  
             if(!EditMode)
             {
              if(CurrentRow!=Rows-1)
               {
               CellDraw(false);
	       CellPosition+=Columns ;
               CurrentRow++;
               barw=Theader[CurrentColumn].length*units+4;
               cpy+=barh ;
	       CellDraw(true);
                }
              }
              break ;
       case KEY_DELETE:
             if(EditMode)
	     {
	      if(vpos<chpos)
	       {
	         for(int i=vpos;i<chpos;i++)
		   (Buffer+CellPosition)->Content[i]=
		      (Buffer+CellPosition)->Content[i+1];
		   chpos--;
		 CellDraw(true);
		}  
	     }     
             break; 
        }// End of inner switch and the case SS_CUR....;
       break;
     //
      
   case SS_NORMAL :
       switch(WinM->Event.Keyboard.Value)
         {
              case KEY_ENTER :
                              if(Theader[CurrentColumn].locked!=LOCKED )
			         {
			       if(!EditMode)
                                  {
                                    EditMode=true ;
                                    // And initialize others
                                    vpos=0 ;
                                    chpos=strlen((Buffer+CellPosition)->Content); 
                                   CellDraw(true);      
                                  }
                               else
                                 {
                                   (Buffer+CellPosition)->Content[chpos]='\0' ;
                                   EditMode=false ;
                                   CellDraw(true) ;
                                 }
				 }
                                break ;
           case KEY_BACKSPACE :
                                if(EditMode)
                                {
                                 if(vpos!=0)
                                   {
                                     for(int i=vpos;i<=chpos;i++)
     (Buffer+CellPosition)->Content[i-1]=(Buffer+CellPosition)->Content[i];
                                  chpos-- ;
                                 vpos-- ;  
				  CellDraw(true);
                                  }
				}
				break ;
          default:
                char c= WinM->Event.Keyboard.Value ;
		short j=Theader[CurrentColumn].type;
          if(EditMode && (j==ALPHANUMERIC || (j==NUMERIC && ((c>='0' && c<='9')||c=='.'))))
             {
		if(chpos<Theader[CurrentColumn].length){
			chpos++ ;   
			for(int i=chpos ;i>vpos ;i--)
		 (Buffer+CellPosition)->Content[i]=(Buffer+CellPosition)->Content[i-1];
		 (Buffer+CellPosition)->Content[vpos]=WinM->Event.Keyboard.Value ;
		 vpos++ ;
		 CellDraw(true) ;
            		}
              }
	      
         }//End of Switch and the case SS_NORMAL
	 break;
}// End of ouer switch 
	WinM->Type=NULL_EVENT ;
  this->show() ;
  }// End of if
return ;
}




















    

