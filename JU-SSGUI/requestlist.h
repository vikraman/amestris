#ifndef REQUESTLIST_H
#define REQUESTLIST_H

#define INPUT_QUEUE_ID 7777
#define REQUEST_QUEUE_ID 9999
#define OUTPUT_ID 8888

#define RQ_STATE     10
#define RQ_CREATE_WINDOW 11
#define RQ_CREATE_EDITBOX 12
#define RQ_CREATE_CANVAS 13
#define RQ_CREATE_BUTTON 14
#define RQ_CREATE_CHECKBOX 15
#define RQ_CREATE_LABEL 16
#define RQ_CREATE_MENU 17
#define RQ_CREATE_CANVAS_SC 18
#define RQ_CREATE_TABLE 19
#define RQ_CREATE_PASSWORDBOX 20

#define RQ_DRAW_BAR  34
#define RQ_DRAW_BARS 80 
#define RQ_MENUBUTTONS 81 
#define RQ_WRITE_TEXTS 79 
#define RQ_DRAW_LINE 31
#define RQ_DRAW_PIXEL 32
#define RQ_DRAW_TEXT 33
#define RQ_DRAW_CIRCLE 35
#define RQ_CANVAS_SHIFT_UP 36
#define RQ_CANVAS_SHIFT_DOWN 37
#define RQ_CANVAS_SHIFT_LEFT 38
#define RQ_CANVAS_SHIFT_RIGHT 39
#define RQ_SET_CANVAS_KEY_STATE 30 

#define RQ_SAVE_TABLE 41

#define RQ_CHECKBOX_STATUS 52
#define RQ_SET_CHECKBOX_STATUS 56
#define RQ_SET_TEXT 54
#define RQ_GET_TEXT 55
#define RQ_DESTROY_WINDOW 51
#define RQ_SHOW_WINDOW 53
#define RQ_HIDE_WINDOW 59
#define RQ_GET_EDIT_TEXT 55
#define RQ_SET_CANVAS_TEXT_COLORS 57
#define RQ_SET_SCANVAS_TEXT_COLORS 58

#define RQ_SET_MENU_ITEM_STATE 60
#define RQ_SET_MENU_PAGE_STATE 61
#define RQ_DESTROY_COMPONENT 62

#define RQ_SET_FOCUS 63
#define RQ_GET_FOCUS 64

#define RQ_GET_NEXT_COMPONENT_INFO 65
#define RQ_SET_COLOR 66
//#define RQ_FREEZE_WINDOW 67
#define RQ_DEFREEZE_WINDOW 68 

#define RQ_PRINT_WINDOW 67

#define RQ_WRITE_TEXT 69

#define RQ_GET_COMPONENT_STATE 70

#define RQ_GUI_SHUTDOWN 0xFF

#define MAX_FONT_NO 30

#endif
