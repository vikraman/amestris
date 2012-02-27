#include<stdio.h>
#include<stdlib.h>

#include "request.h"
//#include "lc_commn.h"

#define MIS_WINDOW_1 1000
#define MIS_WINDOW_2 2000
#define MIS_BUTTON_1 3000
#define MIS_BUTTON_2 3010
#define MIS_BUTTON_3 3020
#define MIS_CANVAS_1 2000
#define MIS_CANVAS_2 2010
#define MIS_CANVAS_3 2020
#define MIS_CANVAS_4 2030
#define MIS_CANVAS_5 2040
#define MIS_CANVAS_6 2050
#define MIS_CANVAS_7 2060
#define MIS_CANVAS_8 2070
#define IN_COLOR1 25
#define IN_COLOR2 27
#define OUT_COLOR1 245

typedef struct
{
        char ParameterName[25];
        unsigned char GraphType;
        unsigned char ScansFactor;
        float RangeValue,T1,T2;
        char YaxisLabel[5];
        float  Value[7];
}ANALOG;

typedef struct
{
        char ParameterName[25];
        unsigned char GraphType;
        unsigned char ScansFactor;
        float RangeValue,T1,T2;
        char YaxisLabel[5];
        unsigned char  Status;
}STATUS;

//Extern Declarations

//tern char project_name[10],console_name[20];
extern  ANALOG anal_parameter[16];
extern  STATUS stat_parameter[16];
//tern FILE *fp;
//tern pid_t hproc1;


/*Declarations*/
static char prev_anal[16];
static float prev_anal_val[16];
static char prev_stat[16]={0};

static int graph_val[5]={0,2,3,4,6};
static int graph_id[5]={2000,2010,2020,2030,2040};
static int p=0,count=0;
static int but_val[5]={1,7,8,9,10};
static int but_id[5]={317,318,314,315,316};

static int but_stat[7]={3,6,7,9,10,14,15};

static int xstep=1,ystep[5],xpix=25,ypix=22,xshift=150,xcnt=0,xaxis=20;
static float prev_val[5]={0.0,0.0,0.0,0.0,0.0};
static int buffx[11]={20,48,76,104,132,160,188,216,244,272,300};
//static int buffx[22]={20,34,48,62,76,90,104,128,132,146,160,174,188,202,216,230,244,258,272,286,300};
static int buffy[16][12];
static int y1,y2=28;
void Display_Mis2_Window1();
void Display_Mis2_Window2();

//void Display_Missile1_Window();
void Mis_drawline(int ,int  ,char *text);
void Mis_draw_surv_graph(unsigned int canvas_id,int window_id,int xval,float th1,float th2,int no);
void Mis_plot_surviel_window1();
void Mis_plot_surviel_window2();
void Mis_Init_Missile1_Variables();
void Mis_plot_surv_graph(unsigned int, int ,int );
extern void Button_Display(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i);
extern void Button_Create(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i);

extern void Button_Display1(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i);
extern void Button_Create1(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[16],int i);
//void Display_Lc_Status(char *text,int status);
/****************************************************************************/

extern pid_t hproc1;

/*	main()
	{

		Display_Mis2_Window1();
		Display_Mis2_Window2();
	}*/
/**************************************************************************/

void Display_Mis2_Window1()
{

        int i,j,k;
        char str[40];
        char cc[30]="  CC ------> LC :  ";
        char text[30]={'\0'};

        Create_Window(MIS_WINDOW_1,0,hproc1,320,0,320,479,1);
	Set_Window_Title(MIS_WINDOW_1,0,hproc1,"MIS2_WINDOW1");

        Create_Canvas(MIS_CANVAS_1,MIS_WINDOW_1,hproc1,320,195,320,130,29);
        Create_Canvas(MIS_CANVAS_2,MIS_WINDOW_1,hproc1,320,325,320,130,29);
        Create_Canvas(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,323,40,320,155,173);
        Create_Canvas(MIS_CANVAS_8,MIS_WINDOW_1,hproc1,320,453,320,26,29);
	Set_Canvas_Key_State(MIS_CANVAS_1,MIS_WINDOW_1,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_2,MIS_WINDOW_1,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_8,MIS_WINDOW_1,hproc1,1);
		

        sprintf(text,"%s",cc);
        Button_Create(MIS_CANVAS_8,MIS_WINDOW_1,hproc1,322,454,320,25,text,2);
        Button_Display(MIS_CANVAS_8,MIS_WINDOW_1,hproc1,322,454,320,23,text,2);

        /*status buttons*/

        for(i=0,j=0,k=0;i<=6;i++)
        {
                if(((i%3)==0)&&(i!=0))
                {
                        j=0;
                        k+=30;
                }

                sprintf(str," %s",stat_parameter[but_stat[i]].ParameterName);
                Button_Create(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,j,k,106,30,str,40);
                Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,j,k,106,30,stat_parameter[but_stat[i]].ParameterName,40);
                j+=107;
        }

        /*analog buttons with values*/

        k=0;
        for(i=0;i<5;i++)
        {
                sprintf(str,"%s",anal_parameter[but_val[i]].ParameterName);
                str[14]='\0';
                Button_Create1(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,91,63,40,str,47);
                Button_Display1(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,91,63,40,str,47);
                sprintf(str,"%5.2f",anal_parameter[but_val[i]].Value[0]);
                Button_Create(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,126,63,28,str,47);
                Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,126,63,28,str,47);
                k=k+64;
        }

        Button_Create(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,107,60,213,30,"DEBUG PROCESS",40);
        Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,107,60,213,30,"DEBUG PROCESS",40);

	for(i=0;i<2;i++)
		Mis_drawline(graph_id[i],MIS_WINDOW_1,anal_parameter[graph_val[i]].ParameterName);
        Show_Window(MIS_WINDOW_1,hproc1);

	for(i=0;i<2;i++)
        {
                Mis_draw_surv_graph(graph_id[i],MIS_WINDOW_1,0,anal_parameter[graph_val[i]].T1,anal_parameter[graph_val[i]].T2,i);
        }

}

/**************************************************************************/

void Display_Mis2_Window2()
{
        int i,j,k;
        char str[40];
        char text[30]={'\0'};

        Create_Window(MIS_WINDOW_2,0,hproc1,320,0,320,479,1);
	Set_Window_Title(MIS_WINDOW_2,0,hproc1,"MIS2_WINDOW2");
        Create_Canvas(MIS_CANVAS_3,MIS_WINDOW_2,hproc1,320,65,320,130,29);
        Create_Canvas(MIS_CANVAS_4,MIS_WINDOW_2,hproc1,320,195,320,130,29);
        Create_Canvas(MIS_CANVAS_5,MIS_WINDOW_2,hproc1,320,325,320,130,29);
        Create_Canvas(MIS_CANVAS_7,MIS_WINDOW_2,hproc1,319,40,320,30,3);
	
	Set_Canvas_Key_State(MIS_CANVAS_3,MIS_WINDOW_2,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_4,MIS_WINDOW_2,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_5,MIS_WINDOW_2,hproc1,1);
	Set_Canvas_Key_State(MIS_CANVAS_7,MIS_WINDOW_2,hproc1,1);
	
        Button_Create(MIS_CANVAS_7,MIS_WINDOW_2,hproc1,1,1,100,30,str,3);
        Button_Display(MIS_CANVAS_7,MIS_WINDOW_2,hproc1,1,1,318,28,"AUTO LAUNCH STATUS",3);
      for(i=0;i<3;i++)
               Mis_drawline(graph_id[i+2],MIS_WINDOW_2,anal_parameter[graph_val[i+2]].ParameterName);
        Show_Window(MIS_WINDOW_2,hproc1);
        for(i=0;i<3;i++)
        {
               Mis_draw_surv_graph(graph_id[i+2],MIS_WINDOW_2,0,anal_parameter[graph_val[i+2]].T1,anal_parameter[graph_val[i+2]].T2,i+2);
        }

}
/***************************************************************************/
void Mis_drawline(int canvas_id,int window_id,char *text)
{ 
	Set_Text_Colors(canvas_id,window_id,hproc1,2,29,1);
	Draw_Text(canvas_id,window_id,hproc1,0,1,text);
	Draw_Bar(canvas_id,window_id,hproc1,252,2,60,15,2);
}

/****************************************************************************/
void Mis_draw_surv_graph(unsigned int canvas_id,int window_id,int xval,float th1,float th2,int no)
{
	int x=20,y=20,i,j=0;
	char text[10]={'\0'};
	xstep=1;
	ystep[no]=(int)((th2)/3);	

	//draw grid/
	do
	{
        	Draw_Line(canvas_id,window_id,hproc1,x,20,x,110,102);
		x=x+28;
	}while(x<=319);

	do
	{
        	Draw_Line(canvas_id,window_id,hproc1,20,y,319,y,102);
		y=y+22;
	}while(y<=110);
        
	Draw_Line(canvas_id,window_id,hproc1,20,19,319,19,255);

	//draw y-axis/

        Draw_Line(canvas_id,window_id,hproc1,20,21,20,111,255);

	Set_Text_Colors(canvas_id,window_id,hproc1,255,29,0);
	for(i=0,j=78;i<4;i++,j-=21)
	{
		sprintf(text,"%3d",ystep[no]*(i+1));
		text[3]='-';
		text[4]='\0';
        	Draw_Text(canvas_id,window_id,hproc1,0,j,text);
	}

	//draw x-axis
        Draw_Line(canvas_id,window_id,hproc1,20,111,319,111,255);
	
	Set_Text_Colors(canvas_id,window_id,hproc1,255,29,0);
 	j=count;
	for(i=15;i<=319;i+=27,j++)
	{
		sprintf(text,"%3d",j*2);
		text[3]='\0';
                Draw_Text(canvas_id,window_id,hproc1,i,112,text);
        }

	//draw thresholds
        
	y=110-(int)((th1/ystep[no])*ypix);
	if(y>110)
		y=110;
	else if (y<20)
		y=20;
	Draw_Line(canvas_id,window_id,hproc1,20,y,319,y,41);

        y=110-(int)((th2/ystep[no])*ypix);
	if(y>110)
		y=110;
	else if (y<20)
		y=20;
	Draw_Line(canvas_id,window_id,hproc1,20,y,319,y,47);
}
/****************************************************************************/



void Mis_plot_surv_graph(unsigned int canvas_id,int window_id,int no)
{
	float th1,th2;
	float value;
	int i;

	th1=anal_parameter[graph_val[no]].T1;
	th2=anal_parameter[graph_val[no]].T2;
	value=anal_parameter[graph_val[no]].Value[0];

	y1=110-(int)((prev_val[no]/ystep[no])*ypix);
	if(y1>110)
	{
		value=0;
		prev_val[no]=value;
		y1=110;
	}
	else if(y1<20)
	{
		y1=20;
	}
	else
		prev_val[no]=anal_parameter[graph_val[no]].Value[0];

	y2=110-(int)((value/ystep[no])*ypix);
	if(y2>110)
	{
		value=0;
		prev_val[no]=value;
		y2=110;
	}
	else if(y2<20)
	{
		y2=20;
	}

	if((value>th1)&&(value<th2))
        	Draw_Line(graph_id[no],window_id,hproc1,buffx[p],y1,buffx[p+1],y2,2);
	else
        	Draw_Line(graph_id[no],window_id,hproc1,buffx[p],y1,buffx[p+1],y2,2);
	
	buffy[no][p]=y1;
        buffy[no][p+1]=y2;
                        
	prev_anal[graph_val[no]]=-1;
	prev_anal_val[graph_val[no]]=-1;
}


/****************************************************************************/

void Mis_plot_surviel_window1()
{

        char str[40],color;
        int i,j,k,no;
        int m=0,n=0;
        char text[10]={'\0'};
					
       //Status buttons
        for(i=0,j=0,k=0;i<=6;i++)

        {
                if(((i%3)==0)&&(i!=0))
                {
                        j=0;
                        k+=30;
                }
                if((stat_parameter[but_stat[i]].Status==1))
                {
                        color=47;
                        prev_stat[but_stat[i]]=color;
                }
         if((stat_parameter[but_stat[i]].Status==0)||(prev_stat[but_stat[i]]!=color))
                 {
                        color=40;
                        prev_stat[but_stat[i]]=color;
                 }
                if(prev_stat[but_stat[i]]!=color)
                {
                        sprintf(str," %s",stat_parameter[but_stat[i]].ParameterName);
                        Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,j,k,106,30,str,color);
                }
                j+=107;
        }
        //Analog buttons

        k=0;
        for(i=0;i<5;i++)
        {
               if((anal_parameter[but_val[i]].Value[0]>=anal_parameter[but_val[i]].T1)&&(anal_parameter[but_val[i]].Value[0]<=anal_parameter[but_val[i]].T2))
                        color=47;
                else
                        color=40;

                if(prev_anal[but_val[i]]!=color)
                {
                        sprintf(str,"%s",anal_parameter[but_val[i]].ParameterName);
                        str[6]='\0';
                        Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,91,63,32,str,color);
                        prev_anal[but_val[i]]=color;
                }
                if(anal_parameter[but_val[i]].Value[0]!=prev_anal_val[but_val[i]])
                {
                        sprintf(str,"%5.2f",anal_parameter[but_val[i]].Value[0]);
                        Button_Display(MIS_CANVAS_6,MIS_WINDOW_1,hproc1,k,124,63,32,str,color);
                        prev_anal_val[but_val[i]]=anal_parameter[but_val[i]].Value[0];
                }
                k=k+64;
        }

        //analog graphs

        for(i=0;i<2;i++)
        {
                // Parameter Name & Value

               if((anal_parameter[graph_val[i]].Value[0]>=anal_parameter[graph_val[i]].T1)&&(anal_parameter[graph_val[i]].Value[0]<=anal_parameter[graph_val[i]].T2))
                        color=2;
                else
                        color=40;

                if(prev_anal[graph_val[i]]!=color)
                {
                        Set_Text_Colors(graph_id[i],MIS_WINDOW_1,hproc1,color,29,1); 
 			Draw_Text(graph_id[i],MIS_WINDOW_1,hproc1,0,1,anal_parameter[graph_val[i]].ParameterName);
                        prev_anal[graph_val[i]]=color;
                }
                if(prev_anal_val[graph_val[i]]!=anal_parameter[graph_val[i]].Value[0])
                {
                        sprintf(str,"%6.2f",anal_parameter[graph_val[i]].Value[0]);
                        Draw_Bar(graph_id[i],MIS_WINDOW_1,hproc1,252,2,60,15,color);   
   			Set_Text_Colors(graph_id[i],MIS_WINDOW_1,hproc1,255,color,0);
                        Draw_Text(graph_id[i],MIS_WINDOW_1,hproc1,258,3,str);
                        prev_anal_val[graph_val[i]]=anal_parameter[graph_val[i]].Value[0];
                }
        }

        //Graphs
        for(i=0;i<2;i++)
        {
                Mis_plot_surv_graph(graph_id[i],MIS_WINDOW_1,i);
        }

       if(xcnt==9)
        {
                count=count+7;
                for(no=0;no<2;no++)
                {
                Set_Text_Colors(graph_id[no],MIS_WINDOW_1,hproc1,255,29,0);
                        for(i=0;i<10;i++)
                        {

                                Draw_Line(graph_id[no],MIS_WINDOW_1,hproc1,buffx[i],buffy[no][i],buffx[i+1],buffy[no][i+1],29);

                         }

        	Draw_Bar(graph_id[no],MIS_WINDOW_1,hproc1,12,112,302,18,29);
        	j=count;
        		for(i=15;i<=319;i+=27,j++)
        		{
                		if(j==500)
                		{
                        		j=0;
                        		count=0;
                		}

                        	sprintf(text,"%3d",j*2);
                        	text[3]='\0';
                        	Draw_Text(graph_id[no],MIS_WINDOW_1,hproc1,i,112,text);
        		}
                        	buffy[no][0]=buffy[no][7];
                        	buffy[no][1]=buffy[no][8];
                        	buffy[no][2]=buffy[no][9];
                        	buffy[no][3]=buffy[no][10];
		}	

                        	for(i=0;i<3;i++)
                        	{

                                	Draw_Line(graph_id[no],MIS_WINDOW_1,hproc1,buffx[i],buffy[no][i],buffx[i+1],buffy[no][i+1],2);
                        	}
                
                xcnt=2;
                p=2;
        }

        y2=y2+1;
        p++;
        xcnt++;

}

void Mis_plot_surviel_window2()
{
        char str[40],color;
        int i,j,k,no;
        int m=0,n=0;
        char text[10]={'\0'};
					
        //analog graphs

        for(i=0;i<3;i++)
        {
                // Parameter Name & Value

               if((anal_parameter[graph_val[i+2]].Value[0]>=anal_parameter[graph_val[i+2]].T1)&&(anal_parameter[graph_val[i+2]].Value[0]<=anal_parameter[graph_val[i+2]].T2))
                        color=2;
                else
                        color=40;

                if(prev_anal[graph_val[i+2]]!=color)
                {
                        Set_Text_Colors(graph_id[i+2],MIS_WINDOW_2,hproc1,color,29,1);
                        Draw_Text(graph_id[i+2],MIS_WINDOW_2,hproc1,0,1,anal_parameter[graph_val[i+2]].ParameterName);
                        prev_anal[graph_val[i+2]]=color;
                }
                if(prev_anal_val[graph_val[i+2]]!=anal_parameter[graph_val[i+2]].Value[0])
                {
                        sprintf(str,"%6.2f",anal_parameter[graph_val[i+2]].Value[0]);
                        Draw_Bar(graph_id[i+2],MIS_WINDOW_2,hproc1,252,2,60,15,color);
                        Set_Text_Colors(graph_id[i+2],MIS_WINDOW_2,hproc1,255,color,0);
                        Draw_Text(graph_id[i+2],MIS_WINDOW_2,hproc1,258,3,str);
                        prev_anal_val[graph_val[i+2]]=anal_parameter[graph_val[i+2]].Value[0];
                }
        }

        //Graphs
        for(i=0;i<3;i++)
        {
                Mis_plot_surv_graph(graph_id[i+2],MIS_WINDOW_2,i+2);
        }

       if(xcnt==9)
        {
                count=count+7;
                for(no=0;no<3;no++)
                {
                	Set_Text_Colors(graph_id[no+2],MIS_WINDOW_2,hproc1,255,29,0);
                        for(i=0;i<10;i++)
			{	
                                Draw_Line(graph_id[no+2],MIS_WINDOW_2,hproc1,buffx[i],buffy[no+2][i],buffx[i+1],buffy[no+2][i+1],29);

                 	}

        		Draw_Bar(graph_id[no+2],MIS_WINDOW_2,hproc1,12,112,302,18,29);
        		j=count;
        		for(i=15;i<=319;i+=27,j++)
        		{
                		if(j==500)
                		{
                        		j=0;
                        		count=0;
                		}

                        		sprintf(text,"%3d",j*2);
                        		text[3]='\0';
                        		Draw_Text(graph_id[no+2],MIS_WINDOW_2,hproc1,i,112,text);
        		}
                        buffy[no+2][0]=buffy[no+2][7];
                        buffy[no+2][1]=buffy[no+2][8];
                        buffy[no+2][2]=buffy[no+2][9];
                        buffy[no+2][3]=buffy[no+2][10];
		}	
                        for(i=0;i<3;i++)
                        {

                                Draw_Line(graph_id[no+2],MIS_WINDOW_2,hproc1,buffx[i],buffy[no+2][i],buffx[i+1],buffy[no+2][i+1],2);
                        }
                
                xcnt=2;
                p=2;
        }

        y2=y2+1;
        p++;
        xcnt++;

}


/****************************************************************************/

/*void Button_Create(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[10],int i)
{
         Draw_Line(canvas,window,hproc,x,y,x+w-1,y,IN_COLOR1); // out top
         Draw_Line(canvas,window,hproc,x+1,y+1,x+w-3,y+1,IN_COLOR2);// in top
         Draw_Line(canvas,window,hproc,x,y+h-1,x+w-1,y+h-1,OUT_COLOR1);//out bottom
         Draw_Line(canvas,window,hproc,x+1,y+h-2,x+w-2,y+h-2,OUT_COLOR1);// in bottom
         Draw_Line(canvas,window,hproc,x,y,x,y+h-1,IN_COLOR1); //out left
         Draw_Line(canvas,window,hproc,x+1,y,x+1,y+h-2,IN_COLOR2);// in left
         Draw_Line(canvas,window,hproc,x+w-1,y,x+w-1,y+h-1,OUT_COLOR1);// out right
         Draw_Line(canvas,window,hproc,x+w-2,y+1,x+w-2,y+h-2,OUT_COLOR1);// in right

}
/****************************************************************************/

/*void Button_Display(unsigned canvas,unsigned window,unsigned hproc,int x,int y,int w,int h,char text[10],int i)
{
         Draw_Bar(canvas,window,hproc,x+2,y+2,w-4,h-4,i);
         Set_Text_Colors(canvas,window,hproc,255,i,0);
         Draw_Text(canvas,window,hproc,x+6,y+6,text);
}
/****************************************************************************/

/*void Delete_Missile1_Window()
{
	Delete_Window(MIS_WINDOW_1,hproc1);
}*/
/****************************************************************************/

/*void Init_Missile1_Variables()
{

}*/
/************************************************************************/

/*void Display_Lc_Status(char *text,int status)
{
	char str[9]={'\0'};
  	sprintf(str,"%8s",text);
	if(status==1)	/*OK*/
//		status=2;/*green*/
//	else
//		status=4;/*red*/
//	Button_Display(MIS_CANVAS_8,MIS_WINDOW_1,hproc1,100,2,100,23,str,status);
//}
/************************************************************************/
