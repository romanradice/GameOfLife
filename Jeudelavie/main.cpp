#include "mylife.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mylife w;
    w.show();
    return a.exec();
}
