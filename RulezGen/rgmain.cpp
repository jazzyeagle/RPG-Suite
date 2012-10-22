#include <QtGui/QApplication>
#include "rgmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RGMainWindow w;
    w.show();
    
    return a.exec();
}
