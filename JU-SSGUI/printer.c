/*
	
	This file contains functions for printing contents of screen for
	differnt printers.Based on the type of the printer used like 
	epson/HP, Postscript/PCL, Parallel/Network, corresponding functions
	are uncommented.All other functions are commented and are kept for 
	future use, incase they are required.

*/

#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include "vga.h"
#include "vgagl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define ESC 0x1b


/* These 2 values should be read from config file */
unsigned short port_no = 9100;
char printer_ip[100] = "192.168.1.50";  

extern FILE *pd;
/*
extern int pr;

static const unsigned char InitializePrinter[]={ESC,'@',13};
static const unsigned char Chartable[]={ESC,'t',1};
static const unsigned char EnablePrintChar[]={ESC,'6'};
static const unsigned char CharSet[]={ESC,'R',0};
static const unsigned char CharPitch[]={ESC,'P'};
static const unsigned char PrintStyle[]={ESC,'x',1};
static const unsigned char VerticalSpacing[]={ESC,'2'};
static const unsigned char PageLength[]={ESC,'C',0x42,13};
static const unsigned char FeedLineInch[]={ESC,'J',0x91};
static const unsigned char RelHPrintPos[]={ESC,0x5c,0x60,0};
//const unsigned char SelectDoubleDensity[]={ESC,'K'};
static const unsigned char SelectDoubleDensity[]={ESC,'L'};
static const unsigned char SetLineFeed[]={ESC,0x4a,0x18};
static const unsigned char CarriageReturn='\r';
static const unsigned char FormFeed=0x0c;
static const unsigned char Init_Printer[]={ESC,'@'};
*/
/* Added for HP Laser Jet Printer(5M) 19/06/07 */
static const unsigned char HP_ResetPrinter[]={ESC,69};
static const unsigned char HP_Orientation[]={ESC,38,108,49,79};
static const unsigned char HP_CarriageReturn[]={10};
static const unsigned char HP_LineFeed[]={13};
static const unsigned char HP_FormFeed[]={'\f'};
static const unsigned char HP_PosCursor[]={ESC,'*','p','4','0','0','x','9','0','Y'};
static const unsigned char HP_RGResolution[]={ESC,'*','t','7','5','R'};
static const unsigned char HP_RGPresentation[]={ESC,'*','r','0','F'};
static const unsigned char HP_StartRG[]={ESC,'*','r','1','A'};
static const unsigned char HP_TransferRGData[]={ESC,'*','b','8','0','W'};
static const unsigned char HP_EndRG[]={ESC,'*','r','C'};

/* Added for Postscript Compatible Laser Jet Printers 02/07/07 */
static const unsigned char PS_Init[]={'%','!','\n'};
static const unsigned char PS_Start[]={'{','<'};
static const unsigned char PS_End[]={'>','}','\n'};
static const unsigned char PS_Save[]="gsave\n";
static const unsigned char PS_Translate[]="45 60 translate\n";
static const unsigned char PS_Scale[]="500 700 scale\n";
static const unsigned char PS_Cols[]="640\n";
static const unsigned char PS_Rows[]="480\n";
static const unsigned char PS_Depth[]="1\n";
static const unsigned char PS_Matrix[]="[640 0 0 480 0 0]\n";
static const unsigned char PS_Image[]="image\n";
static const unsigned char PS_Restore[]="grestore\n";
static const unsigned char PS_Showpage[]="showpage\n";

/*
unsigned char lo_byte,hi_byte;
int height=480;
int width=640;

void dump_screen();
void dump_infile();
void dump_file_to_prn();
void HP_dump_screen();
void PS_dump_screen();
*/
void HP_dump_infile();
void HP_network_dump_screen();
void PS_dump_infile();
void PS_network_dump_screen();
void dump_file_to_network_prn();
/*								
void dump_screen()
{
	int x,y,yofs,no=0;
	unsigned char bits[8]={128,64,32,16,8,4,2,1};
	unsigned char BitData,MaxBits;
	write(pr,&InitializePrinter,3);
	write(pr,&Chartable,3);
	write(pr,&EnablePrintChar,2);
	write(pr,&CharSet,3);
	write(pr,&CharPitch,2);
	write(pr,&PrintStyle,3);
	write(pr,&VerticalSpacing,2);
	write(pr,&PageLength,4);
	write(pr,&FeedLineInch,3);
	write(pr,&RelHPrintPos,4);

	hi_byte=(width/256);
	lo_byte=(width%256);

	for(y=0;y<480;y+=8)
	{
		write(pr,&SelectDoubleDensity,2);
		write(pr,&lo_byte,1);
		write(pr,&hi_byte,1);
		for(x=0;x<width;x++)
		{
			BitData=0;
			if(y+7<=height)
				MaxBits=7;
			else
				MaxBits=height-y;
			for(yofs=0;yofs<=MaxBits;yofs++)
			{
				if(vga_getpixel(x,yofs+y) >0)
				{
					no=vga_getpixel(x,yofs+y);
					if((no==0)||(no==255)||(no==28)||(no==55)||(no==15)||(no==10)||(no==12)||(no==9)||(no==96)||(no==119)||(no==4)||(no==72)||(no==41)||(no==43))
						BitData =(BitData | bits[yofs]);
				}
			}
			write(pr,&BitData,1);
		}
		write(pr,&CarriageReturn,1);
		write(pr,&SetLineFeed,3);
		write(pr,&RelHPrintPos,4);
	}
	write(pr,&FormFeed,1);
	write(pr,&Init_Printer,2);
	write(pr,&CarriageReturn,1);
}	
	

void dump_infile()
{
	int x,y,yofs,no=0;
	unsigned char bits[8]={128,64,32,16,8,4,2,1};
	unsigned char BitData,MaxBits;

	pd=fopen("graph.prn","ab+");

	fwrite(&InitializePrinter,1,3,pd);
	fwrite(&Chartable,1,3,pd);
	fwrite(&EnablePrintChar,1,2,pd);
	fwrite(&CharSet,1,3,pd);
	fwrite(&CharPitch,1,2,pd);
	fwrite(&PrintStyle,1,3,pd);
	fwrite(&VerticalSpacing,1,2,pd);
	fwrite(&PageLength,1,4,pd);
	fwrite(&FeedLineInch,1,3,pd);
	fwrite(&RelHPrintPos,1,4,pd);

	hi_byte=(width/256);
	lo_byte=(width%256);
	
	for(y=0;y<480;y+=8)
	{
		fwrite(&SelectDoubleDensity,1,2,pd);
		fwrite(&lo_byte,1,1,pd);
		fwrite(&hi_byte,1,1,pd);
		for(x=0;x<width;x++)
		{
			BitData=0;
			if(y+7<=height)
				MaxBits=7;
			else
				MaxBits=height-y;
			for(yofs=0;yofs<=MaxBits;yofs++)
			{
				if(vga_getpixel(x,yofs+y) >0)
				{
					no=vga_getpixel(x,yofs+y);
					if((no==0)||(no==255)||(no==28)||(no==55)||(no==15)||(no==10)||(no==12)||(no==9)||(no==96)||(no==119)||(no==4)||(no==72)||(no==41)||(no==43))
						BitData =(BitData | bits[yofs]);
				}
			}
			fwrite(&BitData,1,1,pd);
		}
		fwrite(&CarriageReturn,1,1,pd);
		fwrite(&SetLineFeed,1,3,pd);
		fwrite(&RelHPrintPos,1,4,pd);
	}
	fwrite(&FormFeed,1,1,pd);
	fwrite(&Init_Printer,1,2,pd);
	fwrite(&CarriageReturn,1,1,pd);
	fclose(pd);
}

void HP_dump_screen()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={128,64,32,16,8,4,2,1};
	unsigned char BitData;
	write(pr,&HP_ResetPrinter[0],2);
	write(pr,&HP_Orientation[0],5);
	write(pr,&HP_PosCursor[0],10);
	write(pr,&HP_RGResolution[0],6);
	write(pr,&HP_RGPresentation[0],5);
	write(pr,&HP_StartRG[0],5);
	for(y=0;y<480;y++)
	{
		write(pr,&HP_TransferRGData[0],6);
		for(x=0;x<640;x=x+8)
		{
			BitData=0;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
if((no==0)||(no==255)||(no==28)||(no==55)||(no==15)||(no==10)||(no==12)||(no==9)||(no==96)||(no==119)||(no==4)||(no==72)||(no==41)||(no==43))
					BitData =(BitData | bits[xofs]);
				}
			}
			write(pr,&BitData,1);
		}
	}
	write(pr,&HP_EndRG[0],4);
	write(pr,&HP_FormFeed,1);
}
*/	
void HP_network_dump_screen()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={128,64,32,16,8,4,2,1};
	unsigned char BitData;
	unsigned char data[3];
	int s;
	struct sockaddr_in svr_addr;
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == -1)
	{
		printf("ssgui,dump_screen: Unable to create socket()\n");
		return;
	}
	memset(&svr_addr,0,sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(port_no);
	svr_addr.sin_addr.s_addr = inet_addr(printer_ip);
	if(svr_addr.sin_addr.s_addr == INADDR_NONE)
	{
		printf("ssgui,dump_screen: Unable to form Server Address\n");
		close(s);
		return;
	}
	if(connect(s,(struct sockaddr *)&svr_addr,sizeof(svr_addr))==-1)
	{
		printf("ssgui,dump_screen: Connect to server failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_ResetPrinter[0],2,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 1Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_Orientation[0],5,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 2Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_PosCursor[0],10,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 3Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_RGResolution[0],6,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 4Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_RGPresentation[0],5,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 5Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_StartRG[0],5,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 6Sending data to printer failed\n");
		close(s);
		return;
	}
	for(y=0;y<480;y++)
	{
		if(send(s,&HP_TransferRGData[0],6,MSG_DONTWAIT)<0)
		{
		printf("ssgui,dump_screen: 7Sending data to printer failed\n");
		close(s);
		return;
		}
		for(x=0;x<640;x=x+8)
		{
			BitData=0;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
if((no==0)||(no==255)||(no==28)||(no==55)||(no==15)||(no==10)||(no==12)||(no==9)||(no==96)||(no==119)||(no==4)||(no==72)||(no==41)||(no==43))
					BitData =(BitData | bits[xofs]);
				}
			}
			if(send(s,&BitData,1,MSG_DONTWAIT)<0)
			{
			   printf("ssgui,dump_screen: 8Sending data to "
					   "printer failed\n");
			   close(s);
			   return;
			}
		}
		usleep(5000);
	}
	if(send(s,&HP_EndRG[0],4,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 9Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_FormFeed[0],1,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 10Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&HP_ResetPrinter[0],2,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 1Sending data to printer failed\n");
		close(s);
		return;
	}
	close(s);
}

void HP_dump_infile()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={128,64,32,16,8,4,2,1};
	unsigned char BitData;

	pd=fopen("graph.prn","ab+");
	fwrite(&HP_ResetPrinter[0],1,2,pd);
	fwrite(&HP_Orientation[0],1,5,pd);
	fwrite(&HP_PosCursor[0],1,10,pd);
	fwrite(&HP_RGResolution[0],1,6,pd);
	fwrite(&HP_RGPresentation[0],1,5,pd);
	fwrite(&HP_StartRG[0],1,5,pd);
	
	for(y=0;y<480;y++)
	{
		fwrite(&HP_TransferRGData[0],1,6,pd);
		for(x=0;x<640;x=x+8)
		{
			BitData=0;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
if((no==0)||(no==255)||(no==28)||(no==55)||(no==15)||(no==10)||(no==12)||(no==9)||(no==96)||(no==119)||(no==4)||(no==72)||(no==41)||(no==43))
					BitData =(BitData | bits[xofs]);
				}
			}
			fwrite(&BitData,1,1,pd);
		}
	}
	fwrite(&HP_EndRG[0],1,4,pd);
	fwrite(&HP_FormFeed,1,1,pd);
	fclose(pd);
}
/*
void dump_file_to_prn()
{
        char buff[2];

        pd=fopen("graph.prn","r");
        do
        {
               fread(buff,1,1,pd);
               write(pr,buff,1);
        }while(!feof(pd));
        fclose(pd);
        pd=fopen("graph.prn","wb");
        fclose(pd);
}

void PS_dump_screen()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
	unsigned char BitData;
	unsigned char data[3];
	write(pr,PS_Init,3);
	write(pr,PS_Save,strlen((const char *)PS_Save));
	write(pr,PS_Translate,strlen((const char*)PS_Translate));
	write(pr,PS_Scale,strlen((const char *)PS_Scale));
	write(pr,PS_Cols,strlen((const char *)PS_Cols));
	write(pr,PS_Rows,strlen((const char *)PS_Rows));
	write(pr,PS_Depth,strlen((const char *)PS_Depth));
	write(pr,PS_Matrix,strlen((const char *)PS_Matrix));
	write(pr,PS_Start,2);
	for(y=479;y>=0;y--)
	{
		for(x=0;x<640;x=x+8)
		{
			BitData=0xff;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
					if((no==0)||(no==255)||(no==28)||
					(no==55)||(no==15)||(no==10)||
					(no==12)||(no==9)||(no==96)||
					(no==119)||(no==4)||(no==72)||
					(no==41)||(no==43))
					BitData = (BitData & bits[xofs]);
				}
			}
			sprintf((char *)data,"%02x",BitData);
			write(pr,data,2);
		}
	}
	write(pr,PS_End,3);
	write(pr,PS_Image,strlen((const char *)PS_Image));
	write(pr,PS_Restore,strlen((const char *)PS_Restore));
	write(pr,PS_Showpage,strlen((const char *)PS_Showpage));
}
*/

/*
	This function reads pixcels from screen using vgalib and dumps it in 
	graph.prn file in post script raster graphic format.
*/
/*
void PS_dump_infile()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
	unsigned char BitData;
	unsigned char data[3];
	pd=fopen("graph.prn","ab+");
	if( pd == NULL )
	{
		printf("ssgui,dump_infile: Unable to open graph.prn file\n");
		return;
	}
	fwrite(PS_Init,1,3,pd);
	fwrite(PS_Save,1,strlen((const char *)PS_Save),pd);
	fwrite(PS_Translate,1,strlen((const char *)PS_Translate),pd);
	fwrite(PS_Scale,1,strlen((const char *)PS_Scale),pd);
	fwrite(PS_Cols,1,strlen((const char *)PS_Cols),pd);
	fwrite(PS_Rows,1,strlen((const char *)PS_Rows),pd);
	fwrite(PS_Depth,1,strlen((const char *)PS_Depth),pd);
	fwrite(PS_Matrix,1,strlen((const char *)PS_Matrix),pd);
	fwrite(PS_Start,1,2,pd);
	for(y=479;y>=0;y--)
	{
		for(x=0;x<640;x=x+8)
		{
			BitData=0xff;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
					if((no==0)||(no==255)||(no==28)||
					(no==55)||(no==15)||(no==10)||
					(no==12)||(no==9)||(no==96)||
					(no==119)||(no==4)||(no==72)||
					(no==41)||(no==43))
					BitData = (BitData & bits[xofs]);
				}
			}
			sprintf((char *)data,"%02x",BitData);
			fwrite(data,1,2,pd);
		}
	}
	fwrite(PS_End,1,3,pd);
	fwrite(PS_Image,1,strlen((const char *)PS_Image),pd);
	fwrite(PS_Restore,1,strlen((const char *)PS_Restore),pd);
	fwrite(PS_Showpage,1,strlen((const char *)PS_Showpage),pd);
	fclose(pd);
}
*/
/*
	This function reads pixcels from screen using vgalib and sends it to 
	network printer on a socket in post script raster graphic format.
*/
/*
void PS_network_dump_screen()
{
	int x,y,xofs,no=0;
	unsigned char bits[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
	unsigned char BitData;
	unsigned char data[3];
	int s;
	struct sockaddr_in svr_addr;
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == -1)
	{
		printf("ssgui,dump_screen: Unable to create socket()\n");
		return;
	}
	memset(&svr_addr,0,sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(port_no);
	svr_addr.sin_addr.s_addr = inet_addr(printer_ip);
	if(svr_addr.sin_addr.s_addr == INADDR_NONE)
	{
		printf("ssgui,dump_screen: Unable to form Server Address\n");
		close(s);
		return;
	}
	if(connect(s,(struct sockaddr *)&svr_addr,sizeof(svr_addr))==-1)
	{
		printf("ssgui,dump_screen: Connect to server failed\n");
		close(s);
		return;
	}
	// Sending config data for raster graphics before sending pixcel data
	if(send(s,&PS_Init[0],3,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 1Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Save[0],strlen((const char *)&PS_Save[0]),MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 2Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Translate[0],strlen((const char *)&PS_Translate[0]),
	   MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 3Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Scale[0],strlen((const char *)&PS_Scale[0]),
	   MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 4Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Cols[0],strlen((const char *)&PS_Cols[0]),MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 5Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Rows[0],strlen((const char *)&PS_Rows[0]),MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 6Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Depth[0],strlen((const char *)&PS_Depth[0]),
           MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 7Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Matrix[0],strlen((const char *)&PS_Matrix[0]),
           MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 8Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Start[0],2,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 9Sending data to printer failed\n");
		close(s);
		return;
	}
        // End of config data and start of pixcel data 
	for(y=479;y>=0;y--)
	{
		for(x=0;x<640;x=x+8)
		{
			BitData=0xff;
			for(xofs=0;xofs<=7;xofs++)
			{
				if(vga_getpixel(x+xofs,y) >0)
				{
					no=vga_getpixel(x+xofs,y);
					if((no==0)||(no==255)||(no==28)||
					(no==55)||(no==15)||(no==10)||
					(no==12)||(no==9)||(no==96)||
					(no==119)||(no==4)||(no==72)||
					(no==41)||(no==43))
					BitData = (BitData & bits[xofs]);
				}
			}
			sprintf((char *)data,"%02x",BitData);
	                if(send(s,&data[0],2,MSG_DONTWAIT)<0)
			{
				printf("ssgui,dump_screen:" 
                                "10Sending data to printer failed\n");
				close(s);
				return;
			}
		}
		usleep(5000);
	}
        // End of pixcel data and start of configuration data 
	if(send(s,&PS_End[0],3,MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 11Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Image[0],strlen((const char *)&PS_Image[0]),
           MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 12Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Restore[0],strlen((const char *)&PS_Restore[0]),
           MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 13Sending data to printer failed\n");
		close(s);
		return;
	}
	if(send(s,&PS_Showpage[0],strlen((const char *)&PS_Showpage[0]),
           MSG_DONTWAIT)<0)
	{
		printf("ssgui,dump_screen: 14Sending data to printer failed\n");
		close(s);
		return;
	}
	close(s);
}
*/
/* 
	This function reads a Post Script file containig pixcel data, written
   	by an earlier call to PS_dump_infile(), sends it to a network 
  	printer by creating a socket and connecting to server.
*/

void dump_file_to_network_prn()
{
        char buff[2];
	int s;
	struct sockaddr_in svr_addr;
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == -1)
	{
		printf("ssgui,dump_file: Unable to create socket()\n");
		return;
	}
	memset(&svr_addr,0,sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(port_no);
	svr_addr.sin_addr.s_addr = inet_addr(printer_ip);
	if(svr_addr.sin_addr.s_addr == INADDR_NONE)
	{
		printf("ssgui,dump_file: Unable to form Server Address\n");
		close(s);
		return;
	}
	if(connect(s,(struct sockaddr *)&svr_addr,sizeof(svr_addr))==-1)
	{
		printf("ssgui,dump_file: Connect to server failed\n");
		close(s);
		return;
	}
        pd=fopen("graph.prn","r");
	if(pd == NULL)
	{
		printf("ssgui,dump_file: Unable to open graph.prn\n");
		close(s);
	     	return;
	}
        do
        {
               fread(buff,1,1,pd);
	       if(send(s,&buff[0],1,MSG_DONTWAIT)<0)
	       {
		    printf("ssgui,dump_file: Sending data to printer failed\n");
		    fclose(pd);
		    close(s);
		    return;
	       }
        }while(!feof(pd));
	close(s);
        fclose(pd);
        pd=fopen("graph.prn","wb");
	if(pd != NULL)
        	fclose(pd);
}
