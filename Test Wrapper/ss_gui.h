#ifndef SS_GUI_H
#define SS_GUI_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QObject>
#include <utility>
#include <map>

using namespace std;

enum WidgetType
{
    PushButton,
    LineEdit
};

class Widget : public QObject
{
    Q_OBJECT
public:
    explicit Widget(QObject *parent) {};  //this is causing a hell lot of errors :(
    ~Widget() {};
    QWidget *widget;
    enum WidgetType type;
private slots:
   void onClicked();
};


/*
 * list of all widgets with unique widget id <widget_id: widget>
 */
typedef map <unsigned, Widget> WidgetList;

/*
 * list of all windgets under unique window id <window_id: (window_object, <widget_id>) >
 */
typedef map <unsigned, pair <QWidget *, WidgetList> > WindowList;

/*
 * list of all windows under unique process id <proc_id: <window_id>>
 */
typedef map <unsigned short, WindowList> ProcessList;



/*
 *  API declarations
 */

int Create_Window(unsigned id = 0, unsigned pid = 0,
                  unsigned short procid = 0,int x = 0,
                  int y = 0, int w = 400, int h = 400, int stat = 1);

void Show_Window(unsigned id, unsigned short procid);

void Hide_Window(unsigned id, unsigned short procid);

void Set_Window_Title(unsigned id,unsigned pid ,
                      unsigned short procid,char *text);

int Create_EditBox(unsigned id = 0,unsigned pid = 0,
                   unsigned short procid = 0,int x = 0,
                   int y = 0, int w = 140, int h = 20);

int Set_Text_EditBox(unsigned id,unsigned pid,
                     unsigned short procid, char *text);

int Get_Text(unsigned id, unsigned pid,
             unsigned short procid, char **buffer);

short Create_Button(unsigned id = 0,unsigned pid = 0,
                    unsigned short procid = 0,int x = 0,
                    int y = 0, int w = 80, int h = 35, char *text = 0);

int Delete_Window(unsigned id , unsigned short procid);

#endif // SS_GUI_H
