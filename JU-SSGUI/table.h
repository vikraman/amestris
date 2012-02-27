#ifndef TABLE_H
#define TABLE_H
#include "winobject.h"
#include "vga.h"
#include"vgagl.h"
#include"box.h"
#define ALPHANUMERIC 10
#define NUMERIC 20  
#define MAX_FIELD_LENGTH 50
#define LOCKED 51
#define UNLOCKED 52
 struct FieldDescriptor
{
   short  type ;
   short length ;
   short locked ;
};
struct Field 
{
  char Content[MAX_FIELD_LENGTH] ;
};

class TableEditor: public Canvas 
{
 protected : 
 // cgc currnt context 
   // vgc virtual context 
  int Rows , Columns ;
  FieldDescriptor* Theader;//[60] ;
  Field *Buffer ;
  int CurrentRow , CurrentColumn ;
  bool EditMode ;
  int barh,barw,units ;
  int dx , dy ;
  int cpx,cpy ;
  int CellPosition, chpos, vpos ;
 
 public :
   TableEditor(unsigned id, unsigned pid , int x, int y , int w, int h, int r,int c,FieldDescriptor *fd , Field  *info);

   void howto() ;
   void DrawGrids();
   void CellDraw(bool highlight) ;
   void Write(int x,int y,char *text, int backcolor) ;
//void CharWrite(int x,int y,char ch) ;
   void process(Message *WinM) ;
   void returninfo(int *row , int *col , FieldDescriptor **fp, Field **buffer);
   void changetablestate(int col) ;
   void WriteBuffer(void);
   short GainFocus(void){
     Infocus=TRUE;
     EditMode=false ;
     CellDraw(false) ;
     CellDraw(true);
     show();
     }
   short LoseFocus(void){
     //if(EditMode)
         EditMode=false;
     CellDraw(false);
     
     Infocus=FALSE;
     show();
     }
 };
#endif   
