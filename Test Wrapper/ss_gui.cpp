#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <string>
#include <cstdlib>
#include <iostream>
#include <QObject>
#include "ss_gui.h"

using namespace std;

void Widget::onClicked() {
    
    char *text;
    Get_Text(0, 0, 0, &text);
    std::cout << text << std::endl;
    Set_Text_EditBox(0, 0, 2, text);
}

/*
 *  global process list declaration
 */

ProcessList processList;

int Create_Window(unsigned id, unsigned pid,
                  unsigned short procid,int x,
                  int y, int w, int h, int stat)
{
    if (!processList.count(procid))
        processList[procid] = *(new WindowList);

    if (processList[procid].count(id))
        return -1;    // window already exists

    QWidget *parent;
    if (pid) {
        if (!processList[procid].count(pid))
            return -1;    // parent doesn't exist
        else
            parent = processList[procid][pid].first;
    }

    QWidget *window;
    window = pid ? new QWidget(parent) : new QWidget();
    window->resize(w, h);
    window->move(x, y);
    window->setWindowTitle("window");
    WidgetList newWidgetList = *(new WidgetList);
    processList[procid][id] = make_pair(window, newWidgetList);
    return 0;
}

void Show_Window(unsigned id, unsigned short procid)
{
    processList[procid][id].first->show();
    return;
}

void Hide_Window(unsigned id, unsigned short procid)
{
    processList[procid][id].first->hide();
    return;
}

void Set_Window_Title(unsigned id, unsigned pid,
                      unsigned short procid, char *text)
{
    processList[procid][id].first->setWindowTitle(text);
    return;
}

int Create_EditBox(unsigned id,unsigned pid,
                   unsigned short procid,int x,
                   int y,int w,int h)
{
    if (!processList.count(procid))
        return -1;    // process doesn't exist

    if (!processList[procid].count(pid))
        return -1;    // parent window doesn't exist

    if (processList[procid][pid].second.count(id))
        return -1;    // widget id is already in use

    Widget widget = *(new Widget(NULL));
    widget.type = LineEdit;
    widget.widget = (QWidget *) new QLineEdit(processList[procid][pid].first);
    widget.widget->resize(w, h);
    widget.widget->move(x, y);
    widget.widget->show();

    processList[procid][pid].second[id] = widget;
    return 0;
}

int Set_Text_EditBox(unsigned id,unsigned pid,
                     unsigned short procid, char *text)
{
    if (!processList.count(procid))
        return -1;    // process doesn't exist

    if (!processList[procid].count(pid))
        return -1;    // parent window doesn't exist

    if (!processList[procid][pid].second.count(id))
        return -1;    // widget doesn't exist

    ((QLineEdit *)(processList[procid][pid].second[id].widget))->setText(text);
    return 0;
}

int Get_Text(unsigned id, unsigned pid,
             unsigned short procid, char **buffer)
{
    if (!processList.count(procid))
        return -1;    // process doesn't exist

    if (!processList[procid].count(pid))
        return -1;    // parent window doesn't exist

    if (!processList[procid][pid].second.count(id))
        return -1;    // widget doesn't exist

    QString str = ((QLineEdit *)(processList[procid][pid].second[id].widget))->text();
    *buffer = new char[str.length() + 1];
    strcpy(*buffer, str.toStdString().c_str());
    return 0;
}

short Create_Button(unsigned id,unsigned pid,
                    unsigned short procid,int x,
                    int y, int w, int h, char *text)
{
    if (!processList.count(procid))
        return -1;    // process doesn't exist

    if (!processList[procid].count(pid))
        return -1;    // parent window doesn't exist

    if (processList[procid][pid].second.count(id))
        return -1;    // widget id is already in use

    Widget *widget = new Widget(NULL);
    widget->type = PushButton;
    widget->widget = (QWidget *) new QPushButton(processList[procid][pid].first);
    widget->widget->resize(w, h);
    widget->widget->move(x, y);
    ((QPushButton *)(widget->widget))->setText(text);
    widget->widget->show();

    QObject::connect((QObject *)widget->widget, SIGNAL(clicked()), (QObject *)widget, SLOT(onClicked()), Qt::DirectConnection);

    processList[procid][pid].second[id] = *widget;
    return 0;
}
