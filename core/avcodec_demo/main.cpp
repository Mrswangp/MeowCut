#include <QtWidgets/QApplication>

#include "avcodecdemo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AVCodecDemo w;
    w.show();
    return a.exec();
}
