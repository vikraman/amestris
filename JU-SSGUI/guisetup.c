#include"guisetup.h"
unsigned ProgRun,FreezeWindow;
SystemInput GuiRequest;
void GetMessage(Message *pM)
{
 KeyboardInput ki;
 int icode=0;
 static int reqno;
 pM->Type=NULL_EVENT;
 if(GetCombinedInput(&GuiRequest,&ki,&icode)){
    if(icode==2){
		pM->Event.Keyboard.Control=SS_NORMAL;
	    pM->Event.Keyboard.Category=SS_NORMAL;
	    pM->Type=KEYBOARD_EVENT;
	    if(ki.type==KEY_EXTENDED)
	      pM->Event.Keyboard.Category=SS_CURSOR;
	    if(ki.type==KEY_ALT)
	      pM->Event.Keyboard.Control=SS_ALTKEY;
	    pM->Event.Keyboard.Value=ki.value;
	    return ;
        }
    else if(icode==1){
       pM->Type=REQUEST_EVENT ;
       }
    }// if get combined == true
    else{
      	pM->Type=INPUT_IDLE;
	}
   
 return;
}

int InitGuiSys(unsigned modeno)
{
 int res;
 if((res=InitGraphicsSys(modeno)))
  return res;
 ProgRun=TRUE;
 FreezeWindow=FALSE;
 InitInputEngine();
 if(!InitOutput(8888))
   return 1;
 return 0;
}

void ShutDownGuiSys(void)
{
 ProgRun=FALSE;
 CloseInputEngine();
 CloseOutput();
 ShutDownGraphicsSys();
 return;
} 
