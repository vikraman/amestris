
/* This file contains the class definition which keeps track of which portion
    of the window belongs to which window */ 

#ifndef WINAREA_H
#define WINAREA_H

#include "winobject.h"

#define MAX_WINDOWS 100
#define LEFT 0
#define RIGHT 1

class WindowArea;
extern WindowArea SS_WindowArea;
/////////////////////////////////////////////////////////////////////////////

/* Assumptions :
	 -The largest rectangle ie the screen size is initially occupied.....
	  This screen is not deleted or "unlinked "........

	 -During a window insertion both back-insetion and front-insertion are
	  allowed and the new areas (partitions to the right of the "LEFT"
	  boundaries are invalidated or their references updated and invalidated)

	 -During a window deletion both back and front deletions are supported
	  Areas are invalidated only if their is multiple links to the boundaries
	  hence it is better to call UpdateInvalid /SetAreaInvalid ( followed by
	  validation by the boundary setting of the next display-able window )
	  prior to "Unlinking" the respective boundaries....

	 -Invalid 'usually' means that the grid has no idea about the winobject
	  occupying a particular area NOT that the changed area has been modified
	  or updated on the screen

	 -Currently insertion and deletion of the foremost window will be dealt
	  with...

	 -When many windows are to be created or deleted then it is done in
	  sequence ie proper steps for each display / 'un'-display are followed
	  ie entire screen is validated after each operation...

*/

/* this structure holds the information about the left and right boundary of
    each window  on the screen */
struct XInfo{
		unsigned ObjectID;
		unsigned X;
		unsigned short Valid;
		unsigned References;
		};
/* this structure holds the information about the top and bottom boundary of
    each window  on the screen */
struct YHeader{
		  struct XInfo XLines[2*MAX_WINDOWS];
		  struct YHeader *Next,*Prev;
		  unsigned Y,NumberOfXLines,References;
		  };

class WindowArea {
   //friend void DisplayWindowArea(WindowArea *pWA);
   struct YHeader Start,End,*t,*t1;
   unsigned AreaInvalid,AIminX,AImaxX,AIminY,AImaxY,AreamaxX,AreamaxY;
   unsigned NumberOfAreas,ChangeFlag;
	   short InsertXLine(unsigned x,unsigned y1,unsigned y2,unsigned side);
	   short InsertYLine(unsigned y);
	   short DeleteXLine(unsigned x,unsigned y1,unsigned y2,unsigned ID,unsigned side);
	   short DeleteYLine(unsigned y);

   public:
   	static short WindowCount;
	   WindowArea(unsigned MaxX,unsigned MaxY);
	   ~WindowArea();
	   void Link(Win_Objects &W);// links the boundary to the existing record set
	   void Unlink(Win_Objects &W);// unlinks the boundaries
	   void Unlink(int l,int t,int r,int b,int ID);// unlinks the boundaries
	   int UpdateValid(Win_Objects &W);// updates invalid portions of the screen with area occupied by W
	   int UpdateValid(unsigned Id);
	   int SetAreaValid(Win_Objects &W);//overwrites the existing record 
	                                    // about the screen by W
	   int UpdateInvalid(Win_Objects &W);
	   int SetAreaInvalid(Win_Objects &W);
	   int NumberOfInvalidAreas(void){ // House keeping routines
		 return AreaInvalid;
		 }
	   short IsChanged(void){
		 return ChangeFlag;
		 }
	   void ResetChangeFlag(void){
		 ChangeFlag=FALSE;
		 }
	   unsigned GetPartitionCount(void){
		 return NumberOfAreas;
		 }
	   int GetBoundaries(Win_Objects &W,unsigned  *buffer);
	       // returns the boundaries occupied by the object W in buffer
	   int GetRegionCount(Win_Objects &W);
	       // returns the number of regions of W
	   int GetAreasIn(unsigned left,unsigned top,unsigned right,unsigned bottom,int *buffer);
	      //  returns all the object boundaries in the area (ledt,top-right,bottom)		   
   };

#endif
