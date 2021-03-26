#include "Source/TestApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestApplication w;
    w.show();
    return a.exec();
}
