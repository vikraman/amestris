#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <string>
#include <iostream>
#include <QObject>
#include "ss_gui.h"

Window *win = new Window; //here we're dealing with only 1 parent window for testing purpose.
			  // so instead of using the id (unique window id) we're using this global var

EditBox *box= new EditBox[2];  //1 for input & 1 for output

int Button::onClicked() {
    
    char *text;
    Get_Text(0, 1, 0, text);
    std::cout << "hello" << std::endl;
    std::cout << text << std::endl;
    Set_Text_EditBox(1, 1, 0, text);
    return 1;
}

int Create_Window(unsigned id, unsigned pid,
                  unsigned short procid,int x,
                  int y, int w, int h, int stat)
{
    win->window = new QWidget();
    win->window->resize(w, h);
    win->window->move(x, y);
    win->id = id;
    return 0;
}

void Show_Window(unsigned id, unsigned short procid)
{
    win->window->show();
}

void Set_Window_Title(unsigned id, unsigned pid,
                      unsigned short procid, char *text)
{
    win->window->setWindowTitle(text);
}

int Create_EditBox(unsigned id,unsigned pid,
                   unsigned short procid,int x,
                   int y,int w,int h)
{
    box[id].editBox = new QLineEdit(win->window); //here the searching will be done based on the param "pid"
    box[id].editBox->resize(w, h);
    box[id].editBox->move(x, y);
    box[id].editBox->show();
    box[id].id = id;
}

short Create_Button(unsigned id,unsigned pid,
                    unsigned short procid,int x,
                    int y, int w, int h, char *text)
{
    Button *button = new Button();
    QPushButton *widget = button->getWidget();
    widget = new QPushButton(win->window); //here the searching will be done based on the param "pid"

    /* 
     There're issues with this  ... the callback (i.e onClicked()) is being called only once.
     Any further call is blocked. Fix this.
    */
    QObject::connect((QObject *)widget, SIGNAL(clicked()), (QObject *)button, SLOT(onClicked()), 			      Qt::DirectConnection);

    widget->resize(w, h);
    widget->move(x, y);
    widget->setText(text);
    widget->show();
}

int Get_Text(unsigned id, unsigned pid,
             unsigned short procid, char *buffer)
{
    QString str = box[id].editBox->text();  //every time the trio <procid>:<pid>:<id> will give a unique widget
    buffer = new char[str.length() + 1];
    strcpy(buffer, str.toStdString().c_str());
    std::cout << buffer << std::endl;
    return 0;
}

int Set_Text_EditBox(unsigned id,unsigned pid,
                     unsigned short procid, char *text)
{
    std::cout << text << std::endl;
    box[id].editBox->setText(text);
    return 0;
}
