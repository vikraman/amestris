/* This file contains routines about the feedback sent from the server to 
   the client processes */

#ifndef GUIOUT_H
#define GUIOUT_H
#ifdef __cplusplus
 extern "C" {
#endif  
#include "event.h"
#include "constants.h"
extern int goq;
// The message format 
struct smt{
       unsigned type,A,B,C,D;
       };
extern struct smt GuiM;       
 
extern boolean InitOutput(int key); // Initialises the output queue
extern boolean SendGUIOutput(unsigned type ,unsigned procid,unsigned A,unsigned B ,unsigned C,unsigned D);
    // places the output from the server to the applications 
void TextOut(unsigned procid , char *buffer , int length );
extern boolean CloseOutput(void);// removes the output queue
#ifdef __cplusplus
 }
#endif  
#endif
