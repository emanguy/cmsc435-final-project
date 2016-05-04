#include "nielsenvis.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NielsenVis w;
    w.show();

    return a.exec();
}
