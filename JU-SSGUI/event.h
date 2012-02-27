#ifndef EVENT_H
#define EVENT_H
#include<stdlib.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<unistd.h>

#define INPUT_IDLE 717
#define KEYBOARD_EVENT 728
#define SYSTEM_EVENT 736
#define REQUEST_EVENT 719
#define NULL_EVENT 0 
extern unsigned long EventDelay,EventCallNo;
//extern int mq_id;

typedef struct{
    short Type,Read;
       union{ 
         /* struct{
              unsigned X,Y;
              unsigned MouseEvent;
	  }Mouse;*/
         struct{
               unsigned Category;
               unsigned Control;
               unsigned Value; 
          }Keyboard;
        struct{
              unsigned Source_Id , Destination_Id, Message ;
              }System ;
       }Event;  
 }Message;
extern Message MESSAGE;

extern int InitInputEngine(void);
extern int CloseInputEngine(void);
#endif 
