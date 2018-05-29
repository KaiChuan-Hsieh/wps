#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        argv++;
        qDebug("Set Interface %s", *argv);
        w.setInterface(*argv);
    } else
        w.setInterface("wlan0");

    w.setWindowTitle("WPS Test Application");
    w.show();

    return a.exec();
}
