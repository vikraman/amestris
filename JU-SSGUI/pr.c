#include "pr.h"
#include "stdio.h"
#include "debug.h"
#include <stdlib.h>
#include <unistd.h>
#include"vga.h"
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))

/*********************** Rectangles ********************************/

Rect::Rect(int l,int t,int r,int b) {
	  DEBUG_WRITE("\n Constructing rectangle %d %d %d %d",l,t,r,b);
	  left=l;
	  top=t;
	  right=r;
	  bottom=b;
	  if (left > right || top > bottom) {
		right = left ;
		bottom = top;
		throw BadRect();
	  }
      			
}// end constructor

Rect* Rect::Intersection(Rect& r){
    DEBUG_WRITE("\n Rect::Calculating intersection ");
	int il,it,ir,ib;
       il=max(left,r.left);
       it=max(top,r.top);
       ir=min(right,r.right);
       ib=min(bottom,r.bottom);
       if(il>=ir||it>=ib)
	       return NULL;
       else
	       return new Rect(il,it,ir,ib);
}// end Intersection



bool Rect:: Contains(Rect& r){
  if(left <=r.left && top <= r.top && right >= r.right && bottom >= r.bottom)
		  return true;
  return false;
} // end  Contains

bool Rect:: LeftTo(Rect& r){
	if(right==r.left )	
		  return true;
   return false;	
}


bool Rect:: RightTo(Rect &r){
	 if(left==r.right)
		 return true;
	 return false;
}

bool Rect:: BottomTo(Rect &r){
     if(top==r.bottom)
          return true;
     return false;
}

bool Rect:: TopTo(Rect &r){
     if(r.top ==bottom)
          return true;
     return false;
}


bool Rect:: LeftOf(Rect& r){
	if(right==r.left && top==r.top && bottom == r.bottom)	
		  return true;
   return false;	
}

bool Rect:: RightOf(Rect &r){
	 if(left==r.right && top==r.top && bottom == r.bottom)
		 return true;
	 return false;
}

bool Rect:: BottomOf(Rect &r){
     if(top==r.bottom && left==r.left && right == r.right)
          return true;
     return false;
}

bool Rect:: TopOf(Rect &r){
     if(r.top ==bottom && left==r.left && right == r.right)
          return true;
     return false;
}

bool Rect::EqualTo(Rect &r){
		DEBUG_WRITE("\n Equality check");
						
  if(left==r.left && top == r.top && right == r.right && bottom == r.bottom)
		  return true;
  return false;
}// end function EqualTo
/********************** Partition Rectangles *************************/

PRect::PRect(int l,int t,int r,int b,bool vf):Rect(l,t,r,b){
    for(int i=0;i<=PRECT_MAX_PARTITIONS;i++)
		partitions[i]=NULL;	
	decomposed=false;
	vflag=vf;
	mflag=false;
}// end constructor PRect;

PRect::~PRect(){
  for(int i=0;i<=PRECT_MAX_PARTITIONS;i++)
		  if(partitions[i]){
				  DEBUG_WRITE("\n deleting partition %d",i);
				  delete partitions[i];
				  partitions[i]=NULL;
		  }
}// end destructor PRect;


		
void PRect::Split(Rect &r){
		  
		 if(decomposed)
				  return;
		  DEBUG_WRITE("\n PR::Spliting.....");
		  // make top
		  if(top==r.top)
			  partitions[p_top]=NULL;
		  else
			  partitions[p_top]=NewRect(left,top,right,r.top,vflag);	  
	  	  // make left
		  if(left==r.left)
			  partitions[p_left]=NULL;
		  else
			  partitions[p_left]=
					  NewRect(left,r.top,r.left,r.bottom,vflag);
		  // make center
		  partitions[p_center]=
				  NewRect(r.left,r.top,r.right,r.bottom,!vflag);
		  // make right
		  if(right==r.right)
			  partitions[p_right]=NULL;
		  else
			  partitions[p_right]=
					  NewRect(r.right,r.top,right,r.bottom,vflag);
					  
		  // make bottom	  
		  if(bottom==r.bottom)
			  partitions[p_bottom]=NULL;
		  else
			  partitions[p_bottom]=
					  NewRect(left,r.bottom,right,bottom,vflag);	  
          
		  //set decomposition flag
		  decomposed=true;
		  mflag=true;


}// end function Split

/* ---------------Auxillary functions for  BreakUp()----------------------- */
void PRect::LeftCenterMerge(void){
  PRect *pr,*pr1;
  DEBUG_WRITE("\n Trying Left Merge.....");  
  if(!partitions[p_left]
	||!partitions[p_left]->decomposed
    ||partitions[p_left]->partitions[p_center]->decomposed
	||!partitions[p_left]->partitions[p_center]->LeftOf(*partitions[p_center])
	)
	   return;
   // expand the left side of center to left's center
   partitions[p_center]->left=partitions[p_left]->partitions[p_center]->left;
   pr=partitions[p_left]->partitions[p_left];// save left of left
   pr1=partitions[p_left];
   partitions[p_left]->partitions[p_left]=NULL; // set left of left to null
   partitions[p_left]=pr; //left = left's previous left
   delete pr1; // delete left
   mflag=true;
   DEBUG_WRITE("\n Left Merge ");
   
}// end function LeftMerge

void PRect::RightCenterMerge(void){
  DEBUG_WRITE("\n Trying Right Merge.....");  
  PRect *pr,*pr1;	
	if(!partitions[p_right]
	||!partitions[p_right]->decomposed
    ||partitions[p_right]->partitions[p_center]->decomposed
	||!partitions[p_right]->partitions[p_center]->RightOf(*partitions[p_center])
	)
	   return;
   // expand the left side of center to right's center
   partitions[p_center]->right=partitions[p_right]->partitions[p_center]->right;
   pr=partitions[p_right]->partitions[p_right];// save right of right
   pr1=partitions[p_right];
   partitions[p_right]->partitions[p_right]=NULL; // set right of right to null
   partitions[p_right]=pr; //right = right's previous right
   delete pr1; // delete right;
   mflag=true;
   DEBUG_WRITE("\n Right Merge ");
}// end function RightMerge


void PRect::TopCenterMerge(void){
  DEBUG_WRITE("\n Trying Top Merge.....");  
  PRect *pr,*pr1;	
	if(!partitions[p_top]
	||!partitions[p_top]->decomposed
	||(partitions[p_top]->left!=partitions[p_top]->partitions[p_center]->left)
	||(partitions[p_top]->right!=partitions[p_top]->partitions[p_center]->right)
    ||partitions[p_top]->partitions[p_center]->decomposed
	||!partitions[p_top]->partitions[p_center]->TopOf(*partitions[p_center])
	)
	   return;
   // expand the top side of center to top's center
   partitions[p_center]->top=partitions[p_top]->partitions[p_center]->top;
   pr=partitions[p_top]->partitions[p_top];// save top of top
   pr1=partitions[p_top];
   partitions[p_top]->partitions[p_top]=NULL; // set top of top to null
   partitions[p_top]=pr; //top = top's previous left
   mflag=true;
   delete pr1; // delete top;
   DEBUG_WRITE("\n Top Merge ");
   
}// end function TopMerge

void PRect::BottomCenterMerge(void){
  DEBUG_WRITE("\n Trying Bottom Merge.....");  
  PRect *pr,*pr1;	
  if(!partitions[p_bottom]
   ||!partitions[p_bottom]->decomposed
   ||(partitions[p_bottom]->left!=partitions[p_bottom]->partitions[p_center]->left)
   ||(partitions[p_bottom]->right!=partitions[p_bottom]->partitions[p_center]->right)
   ||partitions[p_bottom]->partitions[p_center]->decomposed
   ||!partitions[p_bottom]->partitions[p_center]->BottomOf(*partitions[p_center])
	)
	   return;
   // expand the bottom side of center to bottom's center
   partitions[p_center]->bottom=
		   partitions[p_bottom]->partitions[p_center]->bottom;
   pr=partitions[p_bottom]->partitions[p_bottom];// save bottom of bottom
   pr1=partitions[p_bottom];
   partitions[p_bottom]->partitions[p_bottom]=NULL; // set bottom of bottom to null
   partitions[p_bottom]=pr; //bottom = bottom's previous left
    mflag=true;
   delete pr1; // delete bottom;
   DEBUG_WRITE("\n Bottom Merge ");
 }

void PRect::BreakUp(Rect& r){
  int i;		
  DEBUG_WRITE("\n PR:: breaking....");
  Rect* pir = Intersection(r);  // find the intersected area
  if(!pir)
		  return;		// return if no intersection
  DEBUG_WRITE("\n ...... checking for cover");
  if(EqualTo(*pir)){    // case that pir covers this
      DEBUG_WRITE("\n Cover found");
	  for(i=0;i<=PRECT_MAX_PARTITIONS;i++)
		 if(partitions[i]){
		   delete partitions[i];
		   partitions[i]=NULL;
		 }
      partitions[p_center]=NewRect(left,top,right,bottom,!vflag);
      decomposed=true;
      mflag=true;
  } // end if Equal to
  else if(! decomposed){
		  DEBUG_WRITE("\n Decomposing PRect");
	      // break up into partitions;
		  Split(*pir); 
  }// end of if(! decomposed)
  else{
		  DEBUG_WRITE("\n Working on decomposed PRect");
		  if(pir->Contains(*partitions[p_center])){
			 bool dflag=false;	  
	         for(i=0;i<PRECT_MAX_PARTITIONS;i++)
				if(partitions[i]&&partitions[i]->decomposed)
				   dflag=true;
	        if(!dflag){
		        DEBUG_WRITE(" ... Found better breakup");
				for(i=0;i<=PRECT_MAX_PARTITIONS;i++)
		          if(partitions[i]){
		               delete partitions[i];
		               partitions[i]=NULL;
		          }
				decomposed=false;
				mflag=true;
				Split(*pir);
				delete pir;
				return;		
				}// end check dflag	
			 
		  }// end check better breakup
		    // recurse along partions
		    for(i=0;i<PRECT_MAX_PARTITIONS;i++)
				  if(partitions[i])
						  partitions[i]->BreakUp(r);

		    // since the center is of
		    partitions[p_center]->Merge(r); // opposite visibility
		  
		   if(!partitions[p_center]->decomposed){ 
	      		LeftCenterMerge();       
			    RightCenterMerge();
			    TopCenterMerge();
			    BottomCenterMerge(); 
			}// merge check
  }		  
  delete pir; 
}// End function Breakup

/* ---------------Auxillary functions for  Merge()----------------------- */
void PRect::CenterLeftMerge(void){
	DEBUG_WRITE("\n Trying Center Left Merge......");	
    if(  !partitions[p_left]
       || partitions[p_left]->decomposed
       || !partitions[p_center]->partitions[p_center]->RightOf(*partitions[p_left]))
      return;
 //expand left of center and its center 
	partitions[p_center]->left=left;
	partitions[p_center]->partitions[p_center]->left=left;
 // delete left
	delete partitions[p_left];
   	partitions[p_left]=NULL;
	mflag=true;
        puts("..Done");
}// end function CenterLeftMerge

void PRect::CenterRightMerge(void){
	DEBUG_WRITE("\n Trying Center Right Merge ....");
    if(  !partitions[p_right]
       || partitions[p_right]->decomposed
       || !partitions[p_center]->partitions[p_center]->LeftOf(*partitions[p_right]))
      return;
 //expand right of center and its center 
	partitions[p_center]->right=right;
	partitions[p_center]->partitions[p_center]->right=right;
 // delete right
	delete partitions[p_right];
   	partitions[p_right]=NULL;
	mflag=true;
	puts("...Done");
}// end function CenterRightMerge

void PRect::CenterTopMerge(void){
	DEBUG_WRITE("\n Trying Center Top Merge....");	
    if( !partitions[p_top]
       || partitions[p_top]->decomposed
       || !partitions[p_center]->partitions[p_center]->BottomOf(*partitions[p_top]))
      return;
 //expand top of center and its center 
	partitions[p_center]->top=top;
	partitions[p_center]->partitions[p_center]->top=top;
 // delete top
	delete partitions[p_top];
   	partitions[p_top]=NULL;
	mflag=true;
	puts("..Done");
}// end function CenterTopMerge


void PRect::CenterBottomMerge(void){
	DEBUG_WRITE("\n Trying Center Bottom Merge......");
    if( !partitions[p_bottom]
       || partitions[p_bottom]->decomposed
       || !partitions[p_center]->partitions[p_center]->TopOf(*partitions[p_bottom]))
      return;
 //expand bottom of center and its center 
	partitions[p_center]->bottom=bottom;
	partitions[p_center]->partitions[p_center]->bottom=bottom;
 // delete bottom
	delete partitions[p_bottom];
   	partitions[p_bottom]=NULL;
	mflag=true;
	puts("...Done");
}// end function CenterBottomMerge



void PRect::Merge(Rect &r){
		Rect *pir;
		DEBUG_WRITE("\n Merging....");
		if(!decomposed) // if already whole then quit
				return;
	  	if(!(pir=Intersection(r)))
				return ; // return if no intersection
        
		DEBUG_WRITE("\n Checking for cover");
        if(EqualTo(*pir)){    // case that pir covers this
	       DEBUG_WRITE("\n Cover found"); // a cover has found
	       for(int i=0;i<=PRECT_MAX_PARTITIONS;i++) // delete the partitions
	         if(partitions[i]){
	            delete partitions[i];
	            partitions[i]=NULL;
	            }
          decomposed=false;
	  mflag=true;
	  } // end if Equal to
	 /* 
		else  if(decomposed && EqualTo(*partitions[p_center])){
		    delete partitions[p_center];
	        partitions[p_center]=NULL;
			decomposed=false;
			DEBUG_WRITE("\n Deleting previous cover and re-Splitting...");
			Split(*pir);
		 }*/		
	 else {
		      DEBUG_WRITE("\n Merge:Working with decomposed rectangles.....");
			  // for decomposed partitions
			  // recurse 
			   for(int i=0;i<PRECT_MAX_PARTITIONS;i++)
				 if(partitions[i]&&partitions[i]->decomposed)
				      partitions[i]->Merge(r);
			   
			   // coz the center is of opposite visibility
			      partitions[p_center]->BreakUp(r);
               // check for merging
			   if(partitions[p_center]->decomposed &&
					!partitions[p_center]->partitions[p_center]->decomposed){  
				  // CenterLeftMerge
			        CenterLeftMerge();
			     // CenterRightMerge
			        CenterRightMerge();
			     // CenterTopMerge
			        CenterTopMerge();
			     // CenterBottomMerge
			        CenterBottomMerge();
                 // check if totally merged
			        if(EqualTo(*(partitions[p_center]->partitions[p_center]))){
			        //delete partions coz whole visible
				     DEBUG_WRITE("\n Merging into whole......");
	                for(int i=0;i<=PRECT_MAX_PARTITIONS;i++) 
			           if(partitions[i]){
	    		          delete partitions[i];
	           		      partitions[i]=NULL;
	            		  }
          			  decomposed=false;
				  mflag=true;

			        }// merge cover
			   }// merge check
	  }// End Else of Equal To	
      delete pir; // free temp rect;	 
}// End function Merge
