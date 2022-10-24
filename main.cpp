#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("SPNG (Simple Perline Noise Generator) by Sergey B.");
    w.show();

    return a.exec();
}
