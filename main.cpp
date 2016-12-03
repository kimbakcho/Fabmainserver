#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load(":/lang/lang_ko.qm");
    a.installTranslator(&qtTranslator);
    MainWindow w;
    w.show();

    return a.exec();
}
