#include "wpr.h"
#include "debug.h"
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
void WRect::Color(unsigned c){
		 if(!decomposed){
			if(!vflag)
			   return;
		    else{
			  //  gl_setclippingwindow(left,top,right,bottom);
				gl_fillbox(left,top,right-left,bottom-top,c);
	          DEBUG_WRITE("\n coloring %d %d %d %d",left,top,right-left,bottom-top);	
		}
		 }else
		   for(int i=0;i<PRECT_MAX_PARTITIONS;i++)
			   if(partitions[i])
					  dynamic_cast<WRect *>(partitions[i])->Color(c);
   }// end function		 

void WRect::MoveBy(unsigned dx,unsigned dy){
    left+=dx,right+=dx,top+=dy;bottom+=dy;
	if(decomposed) 
	 for(int i=0;i<PRECT_MAX_PARTITIONS;i++)
           if(partitions[i])
				      dynamic_cast<WRect *>(partitions[i])->MoveBy(dx,dy);										  
 }// end function
