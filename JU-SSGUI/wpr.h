#include "vga.h"
#include "vgagl.h"
#include "pr.h"
class WRect : public PRect{
  public:   
  WRect(unsigned l,unsigned t,unsigned r,unsigned b):
		  PRect(l,t,r,b,true){};
   virtual PRect * NewRect(int left,int top,int right,int bottom,bool vf){
		       return new WRect(left,top,right,bottom);
			   }
   virtual ~WRect(){};
   void Color(unsigned c);
   void MoveBy(unsigned dx,unsigned dy);   
};
