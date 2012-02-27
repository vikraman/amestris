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
#ifdef __cplusplus
  }
#endif
    
#endif
