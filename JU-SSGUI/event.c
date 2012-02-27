#include<sys/time.h>
#include<sys/stat.h>
#include"vga.h"
#include"event.h"
#include"processinput.h"
extern unsigned ProgRun;
struct timeval EventTV;
unsigned long EventDelay,EventCallNo;
Message MESSAGE;

int InitInputEngine(void)
{

 if(!InitInput(7777,9999))
  return 1;
 else 

  return 0;
}

int CloseInputEngine(void)
{
 CloseInput();
 return 0;
}

