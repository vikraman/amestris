#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include "table.h"
#define FILE_OPEN_ERR 2
#define RC_ERR 3 
#define RC_READ_SUCCESS 1
#define DATA_WRITE_SUCCESS 1 
#define DATA_READ_ERR 4
FILE *fp ;
FieldDescriptor *pfd ;
int allnumeric(char* a)
{
  for(;*a!=0;a++)
    if(*a>='0' && *a<='9')
       continue ;
    else
       return 0 ;
  return 1 ;
}  
int Read_Table_Resource(int *Row, int *Col, FieldDescriptor **fd, char *loadfilename,char *rcfilename)
{
  int r,c , ret ;
  char signature[70], type[3] , locked[3];
  
  fp=fopen(rcfilename,"rt");
  if(!fp)
     return FILE_OPEN_ERR ;
  printf("\n File Opend Successfully") ; 
  ret=fscanf(fp,"%s",signature);
  
  if(ret==EOF || ret!=1 || strcasecmp("TABLEEDITOR", signature)!=0)
     return RC_ERR ;
 printf("\n Signature Found") ; 
   if(fscanf(fp,"%d%d",&r,&c)!=2)
       return RC_ERR ;
   *Row=r ;
   *Col=c ;
   printf("\n Rows %d Cols %d ", r,c);
   pfd=new FieldDescriptor[c] ;
   *fd=pfd ;
   for(int i=0 ;i< c ;i++)
    {
       if(fscanf(fp,"%s%d%s",type,&r,locked)!=3)
             return RC_ERR ;
  
       if(type[0]=='A' || type[0]=='N')
         {
           pfd[i].type=(type[0]=='A'? ALPHANUMERIC :NUMERIC ) ;
           pfd[i].length=r ; 
         printf("\n Field %d Type %s Length %d ",i,type,r);
         }
       else
         {   
                printf("\n returning rc err") ;
             return RC_ERR ;
         }
       if(locked[0]=='Y'||locked[0]=='N')
         pfd[i].locked=(locked[0]=='Y' ? LOCKED : UNLOCKED) ;
       else
          { 
	    printf("\n Returning rc err") ;
	     return RC_ERR ;
	  }
    }
    if(fscanf(fp,"%s",signature)!=1)
      strcpy(loadfilename,"") ;
    else
    {
       if(strstr(signature,"LOADFILE")==NULL)
          strcpy(loadfilename,"") ;
       else
         {
            char *loc ;
            loc=index(signature,'=');
            strcpy(loadfilename,(++loc));
         }
    }
   fclose(fp);
 printf("\n returning Read Success ");
   return RC_READ_SUCCESS ;
 }

 int Read_Data_File(int row, int col , Field *buffer,FieldDescriptor *pf, char *filename)
 {
   int i ,j , pos ;
   fp=fopen(filename,"rt");
   if(!fp)
     return FILE_OPEN_ERR ;
   else
     {
        for(i=0 ;i<row ; i++)
	 for(j=0 ;j < col ;j++)
	   {
	      pos=j+i*col  ;
	      if(fscanf(fp,"%s",buffer[pos].Content)!=1)
	        return DATA_READ_ERR ;
	      if(pf[j].type==NUMERIC)
	        {
		  if(!allnumeric(buffer[pos].Content))
		    { if(pf[j].length<3)
		        strcpy(buffer[pos].Content,"_");
		      else
		        strcpy(buffer[pos].Content,"ERR");
	            }
		}    
	   }   
    }
    fclose(fp);
    return RC_READ_SUCCESS ;
}


int Save_Data_File(int row, int col, Field *buffer,char *filename)
{
   int i ,j , pos ;
   fp=fopen(filename,"wt");
   if(!fp)
     return FILE_OPEN_ERR ;
   else
     {
        for(i=0 ;i<row ; i++)
        {
            for(j=0 ;j < col ;j++)
           {
              pos=j+i*col  ;
              fprintf(fp,"%s ",buffer[pos].Content) ;
                
           }
        fprintf(fp,"\n") ;
       } 

    }
    fclose(fp);
    return DATA_WRITE_SUCCESS ;
}

