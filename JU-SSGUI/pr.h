#ifndef PR_H
#define PR_H

class BadRect{};
/* definition of rectangles */
class Rect{
	public:
		  int left,top,right,bottom;
	public:
		  Rect(int left,int top,int right,int bottom);
    virtual Rect* Intersection(Rect& r);
	virtual bool  Contains(Rect& r);
    virtual bool EqualTo(Rect &r);
    virtual bool  LeftTo(Rect& r);
    virtual bool  RightTo(Rect& r);
    virtual bool  TopTo(Rect& r);
    virtual bool  BottomTo(Rect& r);
    
    virtual bool  LeftOf(Rect& r);
    virtual bool  RightOf(Rect& r);
    virtual bool  TopOf(Rect& r);
    virtual bool  BottomOf(Rect& r);
}; // end class Rect	

/* definition of partitionable rectangles */
enum portions { p_top=0,p_left=1,p_bottom=2,p_right=3,p_center=4};
#define PRECT_MAX_PARTITIONS 4

class PRect: public Rect {
    protected: 
		  PRect* partitions[5];
		  friend  void Show(PRect &pr);
		  bool decomposed;
		  bool vflag;
		  bool mflag;
		  void Split(Rect &r);
		  
		  void LeftCenterMerge(void);
		  void RightCenterMerge(void);
		  void TopCenterMerge(void);
		  void BottomCenterMerge(void);
		  
		  void CenterLeftMerge(void);
		  void CenterRightMerge(void);
		  void CenterTopMerge(void);
		  void CenterBottomMerge(void);
				  
		  
  public :
		   PRect(int left,int top,int right,int bottom,bool vf);
		   virtual PRect * NewRect(int left,int top,
						   				int right,int bottom,bool vf){
			return new PRect(left,top,right,bottom,vf);
		    }		
	      virtual      ~PRect();		   
          virtual bool Decomposed(void){return decomposed;}
		  virtual void BreakUp(Rect& r);
		  virtual void Merge(Rect& r);
		  virtual bool Mergeable(void){ return (decomposed)?false:true;} 
		  
}; //end class PRect


extern void Show(PRect &pr);
#endif
