#ifndef MENU_H
#define MENU_H

#include "winobject.h"
#define OBJ_MENUBAR_ID 10
#include "debug.h"



// forward declarations

 class Popup;
 class MenuBar;
// definition of a menu-item
  typedef struct{
    char *Label;
    bool State; // true =active
    int  Value; // positive values denote return values
                // negetive values denote pull down starters
                // 0 denotes a separator;
    int Key;    // offset to key
    }MenuItem;

// definition of menu-pulldown headers

  typedef struct{
       int Index,  // starting index in the menu-item array;
           Number, // number of items in the pulldown ;
           Value;
       }MenuHeader ;
  
     

// definition of popup-menus

class Popup:public Win_Objects
{
     friend class MenuBar;
  public:
     unsigned short CurrentPosition,textwidth,spacing,LastPopupId,vPosition,
     vNumber;
     unsigned short *pP,*pX,*pY;
     bool *pS;
     MenuHeader *pMH;
     MenuItem *pMI;
     Popup *Next;
     Popup(unsigned ID,unsigned PID,
	  MenuHeader *pmh,MenuItem *pmi,
	  bool *ps,unsigned short *pp,unsigned short *px,unsigned short *py,
     	  unsigned x,unsigned y);
     	  
     virtual void show(void);
     virtual void process(Message *msg);
     virtual void ReportPosition(bool state);
     virtual void SetPositionUp(void);
     virtual void SetPositionDown(void);
     virtual void SetPositionTo(char k);
     virtual void SetBar(bool state);
     virtual ~Popup();
  }; // end class definiton
  
// definition of menu bar
 class MenuBar:public Win_Objects
 {
    public:
    unsigned short CurrentPosition,spacing,
    		LastPopupId,PopupPosition,PopupX,PopupY;
    MenuHeader *pMH;
    MenuItem *pMI;
    char *pML;
    Popup *pStackTop;
    bool PopupActive;
    bool PopupStatus;
    bool RedrawFlag;
    unsigned short *Position;
    unsigned TotalPullDowns,TotalMenuItems;
    MenuBar(int id,int pid,int x,int y,int w,int h,MenuHeader *pmh,
    	MenuItem *pmi,char *pml,unsigned tpd,unsigned tmi);
    virtual void show(void);
    virtual void howto(void);
    virtual void ShowMenuBar(void);
    virtual void process(Message *msg);
    virtual void SetPositionRight(void);
    virtual void SetPositionLeft(void);
    virtual bool SetPositionTo(char k);
    virtual void SetBar(bool state);
    virtual void Adjust(void);
    virtual void ProcessMenuBarMsg(Message *WinM);
    virtual bool ProcessPopupResponse(Message *WinM);
    // popup manipulation routines 
    virtual void InsertPopup(void);
    virtual void DeletePopup(void);
    virtual void ShowPopups(Popup *p);
    virtual void DeletePopupStack(void);
    virtual bool SetItemState(bool state,unsigned Value); 
    virtual bool SetPageState(bool state,unsigned Value); 
    virtual void UpdatePages(int i,unsigned Value); 
    // focus functions
    virtual short LoseFocus(void);
    virtual short GainFocus(void);
    virtual void MoveBy(int dx,int dy);
    virtual ~MenuBar(){
       //DEBUG_WRITE("Deleting menu %d of %d ......",ID,PID);
       /*if(Position)
           delete Position;
       if(pMH)
           delete pMH;
       if(pMI){
       	   pMI=pMI-(pMH->Index);
           delete pMI;
	   }
       if(pML)
           delete pML;
       DEBUG_WRITE(".......Completed Deleting menu %d of %d ",ID,PID);*/
     }
  };// End Class Definition  

#endif


