#include "winarea.h"
#include "vgagl.h"
WindowArea SS_WindowArea(640,480);
#define GCLIP gl_setclippingwindow(0,0,640,480);

short WindowArea::WindowCount=0;

static int ll;
WindowArea::WindowArea(unsigned MaxX,unsigned MaxY){
	Start.Next=&End;
	End.Prev=&Start;
	Start.Prev=End.Next=NULL;
	AreamaxX=MaxX,AreamaxY=MaxY;
	Start.Y=0;End.Y=MaxY;
	Start.NumberOfXLines=End.NumberOfXLines=2;
	Start.References=End.References=1;
	Start.XLines[0].X=0;Start.XLines[1].X=MaxX;
	Start.XLines[0].References=1;
	Start.XLines[1].References=1;
	Start.XLines[0].Valid=Start.XLines[1].Valid=TRUE;
	End.XLines[0].X=0;End.XLines[1].X=MaxX;
	End.XLines[0].References=1;
	End.XLines[1].References=1;
	End.XLines[0].Valid=End.XLines[1].Valid=TRUE;
	AIminX=MaxX;AImaxY=0;AIminY=MaxY;AImaxX=0;
	AreaInvalid=0;
	ChangeFlag=0;
	NumberOfAreas=1;
	}// end proc



WindowArea::~WindowArea(){
	for(t=Start.Next,t1=t->Next;t!=&End;t=t1,t1=t->Next)
	   delete t;
         }// end proc



void WindowArea::Link(Win_Objects &W){
	   InsertYLine(W.Top);
	   InsertYLine(W.Bottom);
	   InsertXLine(W.Right,W.Top,W.Bottom,RIGHT);
	   InsertXLine(W.Left,W.Top,W.Bottom,LEFT);
	   if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	  }// end proc

void WindowArea::Unlink(Win_Objects &W){
	  DeleteYLine(W.Top);
	  DeleteYLine(W.Bottom);
	  DeleteXLine(W.Left,W.Top,W.Bottom,W.ID,RIGHT);
	  DeleteXLine(W.Right,W.Top,W.Bottom,W.ID,LEFT);
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	 }// end proc

void WindowArea::Unlink(int l,int t,int r,int b,int ID){
	  DeleteYLine(t);
	  DeleteYLine(b);
	  DeleteXLine(l,t,b,ID,RIGHT);
	  DeleteXLine(r,t,b,ID,LEFT);
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	 }// end proc

short WindowArea::InsertYLine(unsigned y){
	 for(t=&Start;t!=NULL&&y>t->Y;t=t->Next);
	 if(t&&t->Y==y)
	  {
	  t->References++;
	  return TRUE;
	  }

	 else {
	   t1=(struct YHeader *) new (struct YHeader);
	   if(!t1)
		 return FALSE;
	   t=t->Prev;
	   *t1=*t;
	   t1->Y=y;
	   t1->Prev=t;
	   t1->Next=t->Next;
	   t->Next->Prev=t1;
	   t->Next=t1;
	   t1->References=1;
	   ChangeFlag=TRUE;
	   NumberOfAreas+=t1->NumberOfXLines-1;
	   //GCLIP;
	   //gl_printf(20,10*(ll++),"Adding Y %d",t1->Y);
	   }
	  return TRUE;
	}// end proc



short WindowArea::InsertXLine(unsigned x,unsigned y1,unsigned y2,unsigned side){

	 int i,n;
	 for(t=&Start;t&&t->Y<y1;t=t->Next);

	 if(!t||t->Y!=y1)
		return FALSE;

	 while(t&&t->Y<y2){
	   for(i=0;t->XLines[i].X<x;i++);
	   if(x==t->XLines[i].X){ // x demarker exists
		   t->XLines[i].References++;
		   }
	   else{
		   //GCLIP;
		   //gl_printf(20,10*(ll++),"Adding X  %d",x);
			
		   // x demarker doesnot exist create new;
		   ChangeFlag=TRUE;
		   NumberOfAreas++;

		   for(n=t->NumberOfXLines;n>i;n--)
			 t->XLines[n]=t->XLines[n-1];
		   t->XLines[i].X=x;
		   t->XLines[i].References=1;
		   t->NumberOfXLines++;
		   t->XLines[i].ObjectID=t->XLines[i-1].ObjectID;
		   if(side==LEFT){
			  t->XLines[i].Valid=FALSE;
			  AreaInvalid++;

			  if(AIminX>t->XLines[i].X)
				 AIminX=t->XLines[i].X;
			  //else
				 if(AImaxX<t->XLines[i+1].X)
				   AImaxX=t->XLines[i+1].X;
			  if(AIminY>t->Y)
				AIminY=t->Y;
			  //else
				if(AImaxY<t->Next->Y)
				   AImaxY=t->Next->Y;
			}
		  else
			 t->XLines[i].Valid=TRUE;
		  }
	   // next line
	   t=t->Next;
	  }
	 return TRUE;
   }// end proc




short WindowArea::DeleteXLine(unsigned x,unsigned y1,unsigned y2,unsigned ID,unsigned side){
	 int i,n;
	 for(t=&Start;t&&t->Y<y1;t=t->Next);

	 if(!t||t->Y>y2)
		return FALSE;

	 while(t&&t->Y<y2){
	   for(i=0;t->XLines[i].X<x;i++);
	   if(x==t->XLines[i].X){ // x demarker exists
		 t->XLines[i].References--;
		 if(t->XLines[i].References==0){
			ChangeFlag=TRUE;
			NumberOfAreas--;
			if(t->XLines[i].Valid==FALSE)
				 AreaInvalid--;
			t->NumberOfXLines--;
			// shift one postion
			for(n=i;n<t->NumberOfXLines;n++)
			  t->XLines[n]=t->XLines[n+1];
			}
		 else // More references to the line exist... check of ID was current
		   if(t->XLines[i].ObjectID==ID&&side==LEFT){  // if current then invalidate
			  t->XLines[i].Valid = FALSE;
			  AreaInvalid++;
			  if(AIminX>t->XLines[i].X)
				AIminX=t->XLines[i].X;
			  //else
				if(AImaxX<t->XLines[i+1].X)
				   AImaxX=t->XLines[i+1].X;

			  if(AIminY>t->Y)
				AIminY=t->Y;
			  //else
				if(AImaxY<t->Next->Y)
				  AImaxY=t->Next->Y;
			  }
		 }
	   // next line
	  t=t->Next;

	  }

	 return TRUE;
   }// end proc




short WindowArea::DeleteYLine(unsigned y){
	int i;
	for(t=&Start;t&&t->Y<y;t=t->Next);
	if(!t||t->Y!=y)
	  return FALSE;
	t->References--;
	if(t!=&Start&&t!=&End&&t->References==0){
		// cut links

		for(i =0;i<t->NumberOfXLines-1;i++)
		   if(t->XLines[i].Valid==FALSE)
			  AreaInvalid--;
		t->Prev->Next=t->Next;
		t->Next->Prev=t->Prev;

		// links cut
		ChangeFlag=TRUE;
		NumberOfAreas-=t->NumberOfXLines-1;
		delete t;
		}
	return TRUE;
   }// end proc





int WindowArea::UpdateValid(Win_Objects &W){
	 int i;
	 for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	 if(!t)
	  return AreaInvalid;
	 while(t&&t->Y<W.Bottom){
		for(i=0;i<t->NumberOfXLines;i++)
		  if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right
		 			 && t->XLines[i].Valid==FALSE){
			 t->XLines[i].ObjectID=W.ID;
			 t->XLines[i].Valid=TRUE;
			   AreaInvalid--;
			/*GCLIP;
			gl_fillbox(t->XLines[i].X,t->Y,10,10,0);*/
			}
		t=t->Next;
		}
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	  return AreaInvalid;
	 }// end proc


int WindowArea::SetAreaValid(Win_Objects &W){
	int i;
	 for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	  if(!t||t->Y!=W.Top)
		return AreaInvalid;
	  while(t&&t->Y<W.Bottom){
		for(i=0;i<t->NumberOfXLines-1;i++){
		  if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right){
			 t->XLines[i].ObjectID=W.ID;
			 if(t->XLines[i].Valid==FALSE){
				t->XLines[i].Valid=TRUE;
				AreaInvalid--;
				}
		  }
		}
		t=t->Next;
		}
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	  return AreaInvalid;
	 }// end proc


int WindowArea::UpdateInvalid(Win_Objects &W){
	 int i;
	 for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	 if(!t)
	  return AreaInvalid;
	 while(t&&t->Y<W.Bottom){
		for(i=0;i<t->NumberOfXLines;i++)
		  if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right&&
					t->XLines[i].Valid==TRUE){
			 t->XLines[i].Valid=FALSE;
			   AreaInvalid++;
			   if(AIminX>t->XLines[i].X)
					AIminX=t->XLines[i].X;
			   //else
				 if(AImaxX<t->XLines[i+1].X)
				   AImaxX=t->XLines[i+1].X;

			   if(AIminY>t->Y)
				  AIminY=t->Y;
			  // else
				 if(AImaxY<t->Next->Y)
					AImaxY=t->Next->Y;
			}
		t=t->Next;
		}
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	  return AreaInvalid;
	 }// end proc


int WindowArea::SetAreaInvalid(Win_Objects &W){
	int i;
	 for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	  if(!t||t->Y!=W.Top)
		return AreaInvalid;
	  while(t&&t->Y<W.Bottom){
		for(i=0;i<t->NumberOfXLines-1;i++){
		  if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right
						&&t->XLines[i].Valid==TRUE){
			 t->XLines[i].Valid=FALSE;
			 AreaInvalid++;
			   if(AIminX>t->XLines[i].X)
					AIminX=t->XLines[i].X;
			   //else
				 if(AImaxX<t->XLines[i].X)
				   AImaxX=t->XLines[i].X;

			   if(AIminY>t->Y)
				  AIminY=t->Y;
			  // else
				 if(AImaxY<t->Next->Y)
					AImaxY=t->Next->Y;
		  }
		}
		t=t->Next;
		}
	  if(AreaInvalid==0){
		AIminX=AreamaxX,AIminY=AreamaxY;
		AImaxX=0;AImaxY=0;
		}
	  return AreaInvalid;
	 }// end proc

/* returns the boundaries (exposed areas) of a window as
	 left,top,right,bottom
	 and the number of regions in return value */

int WindowArea::GetBoundaries(Win_Objects &W,unsigned *buffer){
	int i,j=0,count=0;
	//GCLIP;
	//gl_printf(20,40,"Searching for %d [%d %d %d %d]",
	//W.ID,W.Left,W.Top,W.Right,W.Bottom);
	for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	 //gl_printf(320,40+j*8,"Searching for %d <%4d vs %4d>",W.ID,W.Top,t->Y);
	//gl_printf(320,40+j*8,"Searching for %d <%4d vs %4d>",W.ID,W.Top,t->Y);

	if(!t||t->Y!=W.Top)
	  return count;
	while(t&&t->Y<W.Bottom){
	//gl_printf(80,10*(ll++)," S :%d",t->Y);
	
		for(i=0;i<t->NumberOfXLines;i++){
		   if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right
					&&t->XLines[i].ObjectID==W.ID){
			 buffer[j++]=t->XLines[i].X;
			 buffer[j++]=t->Y;
			 buffer[j++]=t->XLines[i+1].X;
			 buffer[j++]=t->Next->Y;
			 count++;
       //gl_printf(0,10*(ll++)," WA: %d %d %d %d ",t->XLines[i].X,t->Y
       //				,t->XLines[i+1].X,t->Next->Y);
       
			 }
		  }
		 t=t->Next;
		}
   // gl_printf(80,10*(ll++)," returning :%d",count);
    return count;
   }// end proc

/* returns the number of exposed areas of a window */
int WindowArea::GetRegionCount(Win_Objects &W){
	int i,count=0;
	for(t=&Start;t&&t->Y<W.Top;t=t->Next);
	if(!t||t->Y!=W.Top)
	  return count;
	while(t&&t->Y<W.Bottom){
		for(i=0;i<t->NumberOfXLines;i++){
		   if(t->XLines[i].X>=W.Left&&t->XLines[i].X<W.Right
					&&t->XLines[i].ObjectID==W.ID){
			 count++;

			 }
		   }
		 t=t->Next;
		}
	return count;
   }// end proc

/* returns the number and boundaries of partions containing a particular area
 information format is
	  (left:top:right:bottom)
*/

int WindowArea::GetAreasIn(unsigned left,unsigned top,
					unsigned right,unsigned bottom,int *buffer){
	 int count=0,i,j=0;
	 for(t=&Start;t&&t->Next->Y<=top;t=t->Next);
	 if(!t)
	  return count;
	 while(t&&t->Y<bottom){
		for(i=0;i<(t->NumberOfXLines-1)&&t->XLines[i+1].X<=left;i++);
		if(i<t->NumberOfXLines-1)
		  while(t->XLines[i].X<right){
			 // copy information into the array
			 buffer[j++]=t->XLines[i].X;
			 buffer[j++]=t->Y;
			 buffer[j++]=t->XLines[i+1].X;
			 buffer[j++]=t->Next->Y;
			 count++;
			 
			 /*GCLIP;
			 gl_fillbox(t->XLines[i].X,t->Y,
			 	t->XLines[i+1].X-t->XLines[i].X,
				t->Next->Y-t->Y,35+i);*/
			 i++;
			 }
		
		// move on to the next line
		t=t->Next;
		}
	 //sleep(1);
	 return count;
	 }

////////////////////////////////////////////////////////////////////////////

