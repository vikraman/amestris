/* This file defines routines for reading from the input queues */
/* inputs are via a message queue */

#ifndef PROCESSINPUT_H
#define PROCESSINPUT_H
 #include "constants.h"
 #ifdef __cplusplus
   extern "C" {
 #endif  
  
 #define KEY_EXTENDED 	 1
 #define KEY_ASCII  	 2
 #define KEY_ALT 	 4
 
 #define KEY_TAB  9
 #define KEY_BACKSPACE 127
 #define KEY_ENTER 	10
 #define KEY_ESCAPE	27
 #define KEY_ESCAPE2    91
 #define KEY_F1  	18
 #define KEY_F2  	19
 #define KEY_F3  	20
 #define KEY_F4  	21
 #define KEY_F5  	22
 #define KEY_F6  	 1
 #define KEY_F7  	 2 
 #define KEY_F8  	 3
 #define KEY_F9  	 5
 #define KEY_F10  	 6
 #define KEY_F11         7
 #define KEY_F12         8
 #define KEY_INSERT      9
 #define KEY_HOME        4
 #define KEY_PAGEUP     12
 #define KEY_DELETE     10
 #define KEY_END        11
 #define KEY_PAGEDOWN   13
 #define KEY_ARROWUP    14 
 #define KEY_ARROWDOWN  15
 #define KEY_ARROWLEFT  17
 #define KEY_ARROWRIGHT 16
/* THe two types of input are */ 
 typedef struct {
     short type;
     short value;
     }KeyboardInput;
     
  typedef struct {
      unsigned char Request_Id ;
      unsigned short Process_Id ;
      unsigned short Seq; 
      unsigned Parameters[9] ;
      unsigned Otherparams[25][8];
      char labels[800];
   }SystemInput ;
   
/*extern int MsgQueue; //key to the keyboard input queue*/
extern int RequestQueue;// key to the request queue
extern boolean InitInput(int ikey,int rkey);
//extern boolean GetInput(KeyboardInput *pki);// non block wait on input queue
extern boolean CloseInput(void);// removes the queue
//extern boolean GetSystemInput(SystemInput *pSi);// non block wait on system queue
extern boolean GetCombinedInput(SystemInput *pSi,KeyboardInput *pki,int *picode);
#ifdef __cplusplus
  }
#endif
    
#endif
