#include "myudp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyUdP Udp;

    return a.exec();
}
