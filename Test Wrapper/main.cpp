#include <QtGui/QApplication>
#include "ss_gui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Create_Window(0, 0, 0, 100, 40);
    Show_Window(0, 0);
    Set_Window_Title(0, 0, 0, "TEST");
    Create_EditBox(0, 0, 0, 130, 40);
    Create_Button(1, 0, 0, 160, 80, 80, 35, "press");
    Create_EditBox(2, 0, 0, 130, 135);
    return a.exec();
}
