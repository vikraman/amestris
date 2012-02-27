#ifndef KEY_VAL_H
#define KEY_VAL_H
#include "keyboardproc.h"

 typedef struct {
	 char Name[3][30];
	 unsigned short Scancode;
	 unsigned char Type ;
	 unsigned short Value;
	 unsigned short AlternateValue ;
 }keyval;

 keyval SS_KeyList[130]= {
  { "SCANCODE_ESCAPE","","",1,0,27,27}
 ,{ "SCANCODE_1","","",2,0,49,33}
 ,{ "SCANCODE_2","","",3,0,50,64}
 ,{ "SCANCODE_3","","",4,0,51,35}
 ,{ "SCANCODE_4","","",5,0,52,36}
 ,{ "SCANCODE_5","","",6,0,53,37}
 ,{ "SCANCODE_6","","",7,0,54,94}
 ,{ "SCANCODE_7","","",8,0,55,38}
 ,{ "SCANCODE_8","","",9,0,56,42}
 ,{ "SCANCODE_9","","",10,0,57,40}
 ,{ "SCANCODE_0","","",11,0,48,41}
 ,{ "SCANCODE_MINUS","","",12,0,45,95}
 ,{ "SCANCODE_EQUAL","","",13,0,61,43}
 ,{ "SCANCODE_BACKSPACE","","",14,0,8,8}
 ,{ "SCANCODE_TAB","","",15,0,9,9}
 ,{ "SCANCODE_Q","","",16,0,113,81}
 ,{ "SCANCODE_W","","",17,0,119,87}
 ,{ "SCANCODE_E","","",18,0,101,69}
 ,{ "SCANCODE_R","","",19,0,114,82}
 ,{ "SCANCODE_T","","",20,0,116,84}
 ,{ "SCANCODE_Y","","",21,0,121,89}
 ,{ "SCANCODE_U","","",22,0,117,85}
 ,{ "SCANCODE_I","","",23,0,105,73}
 ,{ "SCANCODE_O","","",24,0,111,79}
 ,{ "SCANCODE_P","","",25,0,112,80}
 ,{ "SCANCODE_BRACKET_LEFT","","",26,0,91,123}
 ,{ "SCANCODE_BRACKET_RIGHT","","",27,0,93,125}
 ,{ "SCANCODE_ENTER","","",28,0,13,13}
 ,{ "SCANCODE_LEFTCONTROL","","",29,5,29,SS_CTRLKEY}
 ,{ "SCANCODE_A","","",30,0,97,65}
 ,{ "SCANCODE_S","","",31,0,115,83}
 ,{ "SCANCODE_D","","",32,0,100,68}
 ,{ "SCANCODE_F","","",33,0,102,70}
 ,{ "SCANCODE_G","","",34,0,103,71}
 ,{ "SCANCODE_H","","",35,0,104,72}
 ,{ "SCANCODE_J","","",36,0,106,74}
 ,{ "SCANCODE_K","","",37,0,107,75}
 ,{ "SCANCODE_L","","",38,0,108,76}
 ,{ "SCANCODE_SEMICOLON","","",39,0,59,58}
 ,{ "SCANCODE_APOSTROPHE","","",40,0,39,34}
 ,{ "SCANCODE_GRAVE","","",41,0,96,126}
 ,{ "SCANCODE_LEFTSHIFT","","",42,5,42,SS_SHIFTKEY}
 ,{ "SCANCODE_BACKSLASH","","",43,0,92,124}
 ,{ "SCANCODE_Z","","",44,0,122,90}
 ,{ "SCANCODE_X","","",45,0,120,88}
 ,{ "SCANCODE_C","","",46,0,99,67}
 ,{ "SCANCODE_V","","",47,0,118,86}
 ,{ "SCANCODE_B","","",48,0,98,66}
 ,{ "SCANCODE_N","","",49,0,110,78}
 ,{ "SCANCODE_M","","",50,0,109,77}
 ,{ "SCANCODE_COMMA","","",51,0,44,60}
 ,{ "SCANCODE_PERIOD","","",52,0,46,62}
 ,{ "SCANCODE_SLASH","","",53,0,47,63}
 ,{ "SCANCODE_RIGHTSHIFT","","",54,5,54,SS_SHIFTKEY}
 ,{ "SCANCODE_KEYPADMULTIPLY","","",55,0,42,42}
 ,{ "SCANCODE_LEFTALT","","",56,5,56,SS_ALTKEY}
 ,{ "SCANCODE_SPACE","","",57,0,32,32}
 ,{ "SCANCODE_CAPSLOCK","","",58,5,58,SS_CAPSLOCK}
 ,{ "SCANCODE_F1","","",59,1,59,0}
 ,{ "SCANCODE_F2","","",60,1,60,0}
 ,{ "SCANCODE_F3","","",61,1,61,0}
 ,{ "SCANCODE_F4","","",62,1,62,0}
 ,{ "SCANCODE_F5","","",63,1,63,0}
 ,{ "SCANCODE_F6","","",64,1,64,0}
 ,{ "SCANCODE_F7","","",65,1,65,0}
 ,{ "SCANCODE_F8","","",66,1,66,0}
 ,{ "SCANCODE_F9","","",67,1,67,0}
 ,{ "SCANCODE_F10","","",68,1,68,0}
 ,{ "SCANCODE_NUMLOCK","","",69,5,69,SS_NUMLOCK}
 ,{ "SCANCODE_SCROLLLOCK","","",70,1,0,0}
 ,{ "SCANCODE_KEYPAD7","SCANCODE_CURSORUPLEFT","",71,2,71,55}
 ,{ "SCANCODE_KEYPAD8","SCANCODE_CURSORUP","",72,2,72,56}
 ,{ "SCANCODE_KEYPAD9","SCANCODE_CURSORUPRIGHT","",73,2,73,57}
 ,{ "SCANCODE_KEYPADMINUS","","",74,0,45,45}
 ,{ "SCANCODE_KEYPAD4","SCANCODE_CURSORLEFT","",75,2,75,52}
 ,{ "SCANCODE_KEYPAD5","","",76,2,76,53}
 ,{ "SCANCODE_KEYPAD6","SCANCODE_CURSORRIGHT","",77,2,77,54}
 ,{ "SCANCODE_KEYPADPLUS","","",78,0,43,43}
 ,{ "SCANCODE_KEYPAD1","SCANCODE_CURSORDOWNLEFT","",79,2,79,49}
 ,{ "SCANCODE_KEYPAD2","SCANCODE_CURSORDOWN","",80,2,80,50}
 ,{ "SCANCODE_KEYPAD3","SCANCODE_CURSORDOWNRIGHT","",81,2,81,51}
 ,{ "SCANCODE_KEYPAD0","","",82,2,82,48}
 ,{ "SCANCODE_KEYPADPERIOD","","",83,2,83,46}
 ,{ "","","",84,4,0,0}
 ,{ "","","",85,4,0,0}
 ,{ "SCANCODE_LESS","","",86,1,86,0}
 ,{ "SCANCODE_F11","","",87,1,87,0}
 ,{ "SCANCODE_F12","","",88,1,88,0}
 ,{ "","","",89,4,0,0}
 ,{ "","","",90,4,0,0}
 ,{ "","","",91,4,0,0}
 ,{ "","","",92,4,0,0}
 ,{ "","","",93,4,0,0}
 ,{ "","","",94,4,0,0}
 ,{ "","","",95,4,0,0}
 ,{ "SCANCODE_KEYPADENTER","","",96,0,13,13}
 ,{ "SCANCODE_RIGHTCONTROL","SCANCODE_CONTROL","",97,5,97,SS_CTRLKEY}
 ,{ "SCANCODE_KEYPADDIVIDE","","",98,0,47,47}
 ,{ "SCANCODE_PRINTSCREEN","","",99,1,99,0}
 ,{ "SCANCODE_RIGHTALT","","",100,5,100,SS_ALTKEY}
 ,{ "SCANCODE_BREAK","","",101,1,101,0}
 ,{ "SCANCODE_HOME","","",102,3,71,0}
 ,{ "SCANCODE_CURSORBLOCKUP","","",103,3,72,0}
 ,{ "SCANCODE_PAGEUP","","",104,3,73,0}
 ,{ "SCANCODE_CURSORBLOCKLEFT","","",105,3,75,0}
 ,{ "SCANCODE_CURSORBLOCKRIGHT","","",106,3,77,0}
 ,{ "SCANCODE_END","","",107,3,79,0}
 ,{ "SCANCODE_CURSORBLOCKDOWN","","",108,3,80,0}
 ,{ "SCANCODE_PAGEDOWN","","",109,3,81,0}
 ,{ "SCANCODE_INSERT","","",110,5,82,SS_INSERT}
 ,{ "SCANCODE_REMOVE","","",111,3,83,0}
 ,{ "","","",112,4,0,0}
 ,{ "","","",113,4,0,0}
 ,{ "","","",114,4,0,0}
 ,{ "","","",115,4,0,0}
 ,{ "","","",116,4,0,0}
 ,{ "","","",117,4,0,0}
 ,{ "","","",118,4,0,0}
 ,{ "SCANCODE_BREAK_ALTERNATIVE","","",119,1,0,0}
 ,{ "","","",120,4,0,0}
 ,{ "","","",121,4,0,0}
 ,{ "","","",122,4,0,0}
 ,{ "","","",123,4,0,0}
 ,{ "","","",124,4,0,0}
 ,{ "SCANCODE_LEFTWIN","","",125,1,125,0}
 ,{ "SCANCODE_RIGHTWIN","","",126,1,126,0}
 ,{ "","","",127,4,0,0}
 ,{ "","","",128,4,0,0}
 ,{ "","","",129,4,0,0}
 ,{ "","","",130,4,0,0}
 };// Termination of declarations



#endif
