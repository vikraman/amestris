#ifndef SS_GUI_H
#define SS_GUI_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QObject>

typedef struct {
    QWidget *window;
    unsigned id;
} Window;

typedef struct {
    QLineEdit *editBox;
    unsigned id;
    QString text;
} EditBox;

class Button : public QWidget
{
    Q_OBJECT
public:
    explicit Button(QWidget *parent = 0) {};
    ~Button() {};
    QPushButton *getWidget() {return button;}

private slots:
   int onClicked();

private:
    QPushButton *button;
};



int Create_Window(unsigned id = 0, unsigned pid = 0,
                  unsigned short procid = 0,int x = 0,
                  int y = 0, int w = 400, int h = 400, int stat = 1);

void Show_Window(unsigned id, unsigned short procid);

void Set_Window_Title(unsigned id,unsigned pid ,
                      unsigned short procid,char *text);

int Delete_Window(unsigned id , unsigned short procid);

short Create_Button(unsigned id = 0,unsigned pid = 0,
                    unsigned short procid = 0,int x = 0,
                    int y = 0, int w = 80, int h = 35, char *text = 0);

int Create_EditBox(unsigned id = 0,unsigned pid = 0,
                   unsigned short procid = 0,int x = 0,
                   int y = 0, int w = 140, int h = 20);

int Set_Text_EditBox(unsigned id,unsigned pid,
                     unsigned short procid, char *text);

int Get_Text(unsigned id, unsigned pid,
             unsigned short procid, char *buffer);

#endif // SS_GUI_H
