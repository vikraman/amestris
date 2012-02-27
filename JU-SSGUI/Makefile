# Makefile for the E.G.U.I.A.G GUI server 

# Release mode
#CFLAGS= -DNODEBUG_PROG -g -Wall -c
all:  ssgui.lc mq1.lc
	@echo "NOTE:!!!ON NEWPC RUN make gui in SSGUI folder!!!"
	@strip -s ssgui.lc
#	cp *.lc ../../exe/
	cp *.lc ../
# Development compliation mode
CFLAGS= -DNODEBUG_PROG  -O3 -c -Wno-deprecated  
#CFLAGS= -DEBUG_PROG  -c -Wno-deprecated -g 
#CFLAGS= -DNODEBUG_PROG  -O3 -c -Wno-deprecated 
#all: disp ssgui mq1

#Default target

# Keyboard handler
mq1.lc:
	g++ -o mq1.lc mq1.c mesg.c 
# Display prompt
disp: 
	@echo  "DCL-JU GUI Server [Make]"
	@echo -n " Checking dependencies ."

#Main file 'ssgui'

ssgui.lc: ssgui.o  guiprocess.o menu.o winobject.o winarea.o guisetup.o mesg.o vgasetup.o event.o box.o processinput.o guiout.o font.o table.o debug.o wpr.o pr.o
	g++  -O3 -o ssgui.lc ssgui.o  guiprocess.o menu.o winobject.o winarea.o guisetup.o mesg.o vgasetup.o event.o box.o processinput.o guiout.o table.o debug.o font.o wpr.o pr.o -lvgagl -lvga -march=i686

# List of temporaries
ssgui.o: ssgui.cpp 
	g++  $(CFLAGS) -c ssgui.cpp 

guiprocess.o: guiprocess.cpp guiprocess.h readtbrc.cpp printer.c 
	g++  $(CFLAGS) -c guiprocess.cpp 

menu.o : menu.cpp menu.h
	g++  $(CFLAGS) -c menu.cpp 
	
winobject.o: winobject.cpp winobject.h 
	g++  $(CFLAGS) -c winobject.cpp 
winarea.o: winarea.cpp winarea.h
	g++  $(CFLAGS) -c winarea.cpp 
 
guisetup.o:guisetup.c guisetup.h
	gcc  $(CFLAGS) -c guisetup.c 
vgasetup.o:vgasetup.c vgasetup.h
	gcc  $(CFLAGS) -c vgasetup.c 

event.o:event.c event.h 
	gcc  $(CFLAGS) -c event.c 

box.o : box.c box.h
	gcc  $(CFLAGS) -c box.c 

processinput.o :processinput.cpp processinput.h
	g++  $(CFLAGS) -c processinput.cpp 
	
mesg.o :mesg.c mesg.h
	g++  $(CFLAGS) -c mesg.c 
	
guiout.o : guiout.cpp guiout.h
	g++  $(CFLAGS) -c guiout.cpp 
	
table.o : table.cpp table.h
	g++  $(CFLAGS) -c table.cpp 
	
font.o :  font.c font.h font_bdf.c font_bdf.h
	gcc $(CFLAGS) -c font.c 
	
debug.o : debug.c debug.h
	gcc $(CFLAGS) -c debug.c 
wpr.o   :  wpr.c wpr.h
	g++ -c wpr.c 
pr.o 	:  pr.c pr.h
	g++ -c pr.c 
# CLean up portion 
clean : 
	@echo " Cleaning temporaries...."     
	@rm -f *.o core tags ssgui.lc 
	@echo "Done"         
