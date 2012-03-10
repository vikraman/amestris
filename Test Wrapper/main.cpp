#include <QtGui/QApplication>
#include "ss_gui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Create_Window(1, 0, 0, 100, 40);
    Show_Window(1, 0);
    Set_Window_Title(1, 0, 0, "TEST");
    Create_EditBox(0, 1, 0, 130, 40);
    Create_Button(0, 1, 0, 160, 80, 80, 35, "press");
    Create_EditBox(1, 1, 0, 130, 135);
    return a.exec();
}
