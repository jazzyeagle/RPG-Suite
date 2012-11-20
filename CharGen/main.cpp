#include <QtGui/QApplication>
#include "cgmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CGMainWindow w;
    w.show();
    
    return a.exec();
}
