#ifndef DEBUG_H
#define DEBUG_H
#ifdef DEBUG_PROG
	#ifdef __cplusplus
	 extern "C" {
	#endif 
	#include <stdio.h>
	#include <stdlib.h>
        #include <sys/time.h>  
	      
	
        extern FILE *debug_fp;
	extern struct timeval dtv;
        
	
	/* define debug file write */
	#define DEBUG_WRITE(args...)  \
	   { \
	     gettimeofday(&dtv,NULL);\
	     fprintf(debug_fp , "\n <<<<< (%ld || %ld) %s [%d] ",dtv.tv_sec,dtv.tv_usec,__FILE__,__LINE__); \
	     fprintf(debug_fp , "\n\t ***>   "## args); \
	     fflush(debug_fp);\
	   } 
	 
	/* define debug file closure */
	#define DEBUG_END(args...) \
	   { \
	    gettimeofday(&dtv,NULL);\
	    fprintf(debug_fp,"\n (%ld || %ld) : %s [%d]",dtv.tv_sec,dtv.tv_usec,__FILE__,__LINE__); \
	    fprintf(debug_fp,"\n\t***>  "## args);\
	    fprintf(debug_fp,"\n .............Session Ended \n");\
	    fclose(debug_fp); \
	   } 
	
	 /* define debug file open */   
	#define DEBUG_START(filename) \
	      { \
	       debug_fp=fopen(filename,"wt"); \
	       if(!debug_fp){ \
		 fprintf(stderr,"\n\a DEBUG : cannot open file %s \n",filename); \
		 exit(1); \
		 } \
	         gettimeofday(&dtv,NULL);\
		 fprintf(debug_fp,"\n (%ld || %ld): %s[%d]\n\t Session Started..........",dtv.tv_sec,dtv.tv_usec,__FILE__,__LINE__); \
	      }  	 

	#ifdef __cplusplus
	  } 
	#endif 

#else

  #define DEBUG_START(filename) ;
  #define DEBUG_END ;
  #define DEBUG_WRITE(args...) ;
  #define DEBUG_SETUP ;
#endif  
#endif		  
	      
