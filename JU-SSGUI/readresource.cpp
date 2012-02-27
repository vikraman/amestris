#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "menu.h"
#include "debug.h"
#define  MD_TERMINATION1 -1;
#define  MD_TERMINATION2 -2;
#define FORMAT_ERROR 1
#define FILE_ERROR 2
#define READ_ERROR 4

FILE *fm;   		
 char c;
 long off;
 bool errflag;
 static int lli,gmhi,gmii;

// read a number from the file stream fm
int read_number(void){
    char number[15];
    int i=0;
    while(c!='[')
      c=fgetc(fm); // read past [
    while(c!=']'&&i<14){
       c=fgetc(fm);
       number[i++]=c;
       }
    number[i]='\0';
    return(atoi(number));
    }// end func

int read_next_number(void){
    char number[12];
    int i=0;
    c=fgetc(fm);
    if(c!='['){
     ungetc(c,fm);
     return -1;
     }
    while(c!=']'&&i<12){
       c=fgetc(fm);
       if(isdigit(c))
        number[i++]=c;
       }
    number[i]='\0';
    DEBUG_WRITE(" Read_next_number found %s ",number);
    return(atoi(number));
    }// end func
// finds required space and total number of labels and popups
short read_menu_pass1(char filename[],int *ptpd,int *ptmi,int * plength){
    char signature[6]="";
    short flag=0;
    int pcnt,scnt;
    int i,length=0,pi,count;
    pcnt=scnt=0;
    if((fm=fopen(filename,"r"))==NULL){
      DEBUG_WRITE("\n\a RMP1: %s couldnot be opened",filename);
      flag|=FILE_ERROR;
      goto exitproc;
      }
    for(c=fgetc(fm);c!=EOF&&isspace(c);c=fgetc(fm));
    if(c==EOF){
      DEBUG_WRITE("\n\a RMP1: %s :file format error",filename);
      flag|=FILE_ERROR;
      goto exitproc;
      }
    for(i=0;i<5&&c!=EOF;i++,c=fgetc(fm))
       signature[i]=c;
    signature[5]='\0';
    if(strcmp(signature,"MENU:")){
       DEBUG_WRITE("\n\a RMP1: %s :file format error",filename);
       flag|=FORMAT_ERROR;
       goto exitproc;
       }
    // next read in the number of popups
    pcnt=read_number();
    if(pcnt<=0){
      flag=READ_ERROR;
      goto exitproc;
      }
    off=ftell(fm);
    i=length=pi=count=0;
    while(!feof(fm)){
       count++;
       c=fgetc(fm);
       // insert code to compensate for occurence of '&';
       if(c=='\\'){
         scnt++;
	 continue;
	 }
       if(c=='"'){
	   i++;
	   if(i%2)
	      pi=count; // mark start of label on odd encounter
	   else 
	      length+=count-pi; // calculate the length of label and
	     		     // add to length	
	 }// end encounter check
      
       }// end while
    if(i%2){
      DEBUG_WRITE("\n\a RMP1: %s :file format (quotes) error",filename);
      flag|=FORMAT_ERROR;
      goto exitproc;
      }
   // return values;
   *ptpd=pcnt;
   *plength=length;
   *ptmi=i/2+scnt;
exitproc:
    DEBUG_WRITE(" Total PullDowns %d Separator %d Items %d",pcnt,scnt,i/2+scnt);  
			return flag;
  }


// recieves allocated buffer ,reads and stores values into it 
// assumes that fm is past the initial menu
void read_menu_pass2(char *pll,MenuHeader *pmh,MenuItem *pmi,int n){
  int cnt,mhi,mii,pdid;
  short sflag=FALSE;
  mhi=gmhi;mii=gmii;
  gmii+=n;
  for(cnt=0;cnt<n;cnt++,mii++){
      // read in label and number 
      sflag=FALSE;
      while(c!='"'){  // read in trailing whilw space characters
           c=fgetc(fm);
	      if(c==EOF){
		errflag=true;
		DEBUG_WRITE("\n\a [%d/%d] EOF while scanning for label ",cnt+1,n);
		return;
		}
	   if(c=='\\'){
	     pmi[mii].Value=0; // a separator
	     sflag=TRUE;
	     break;
	     }
	   }
      c=fgetc(fm);
      if(c==EOF){
        errflag=true;
	return;
	}
      if(sflag==TRUE){
	continue;
	}
      pmi[mii].Label=&pll[lli]; 
      pmi[mii].State=true; // default state is active
      pmi[mii].Key=0;
      while(c!='"'){
          // inserting code for '&'
	   if(c=='&')
	     pmi[mii].Key=&pll[lli]-pmi[mii].Label;
	   else 
	     pll[lli++]=c;
          c=fgetc(fm);
	      if(c==EOF){
		errflag=true;
		DEBUG_WRITE("\n\a [%d/%d] Unexpected EOF while reading label ",cnt+1,n);
		return;
		}
          }
      pll[lli++]='\0';
      pmi[mii].Value=read_number(); // read in value
      DEBUG_WRITE("\n RMP2 :[%d/%d] Reading string<%d> %s [%d]",
      	cnt+1,n,pmi[mii].Key,pmi[mii].Label,pmi[mii].Value);
      pdid=read_next_number();
      do{
           c=fgetc(fm);     // move to next line
	   if(c==EOF){
		DEBUG_WRITE("\n\a[%d,%d] Unexpected end of file while reading terminator ",cnt+1,n);
		errflag=true;
		return;
		}
	   }while(c!=';'&&c!='{'&&c!='}');
      if(c=='{'){
	pmh[++gmhi].Number=pmi[mii].Value;
	pmh[gmhi].Index=gmii;
	pmh[gmhi].Value=pdid;
	DEBUG_WRITE("RMP2: >>> Entering sub-menu %s(no %d) [%d][%d]",
		pmi[mii].Label,gmhi,pmh[gmhi].Number, pmh[gmhi].Value);
	pmi[mii].Value=-gmhi;
	read_menu_pass2(pll,pmh,pmi,pmh[gmhi].Number);
	if(errflag==true)
	  return;
	}// end sub-menu recursion if
    }// end load for 
  return;
 }
 
 short ReadMenu(char filename[],
   MenuHeader **ppmh,MenuItem **ppmi,char **ppll,
   		unsigned *pmhno,unsigned *pmino)
{
  int tpd,tmi,length;
  MenuHeader *pmh;
  MenuItem *pmi;
  char * mll;
  int st=read_menu_pass1(filename,&tpd,&tmi,&length);
  lli=0;
  gmhi=-1;
  gmii=0;
  errflag=false;
  if(!st){
      DEBUG_WRITE("\n Allocating %d bytes for  Menu Items",length);
	  if(!(mll=new char[length])){
      DEBUG_WRITE(" RM : Memory Allocation err for menu label ");
      return FALSE;
      }
    DEBUG_WRITE("\n Allocating %d  headers  ",tpd);
    if(!(pmh=new MenuHeader[tpd+1])){
    DEBUG_WRITE(" RM : Memory Allocation err for menu header ");
      return FALSE;
      }
		
    DEBUG_WRITE("\n Allocating %d items  ",tmi);
    if(!(pmi=new MenuItem[tmi+1])){
    DEBUG_WRITE(" RM : Memory Allocation err for menu items ");
     return FALSE;
     }
		
    fseek(fm,off,SEEK_SET);
    read_menu_pass2(mll,pmh,pmi,1);
    if(errflag)
      return -1;
    fclose(fm);
    *ppmh=pmh;
    *ppll=mll;
    *ppmi=pmi;
    *pmhno=tpd;
    *pmino=tmi;
    }
   return st;
  }
