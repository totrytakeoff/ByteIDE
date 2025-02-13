#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("ByteIDE");

    MainWindow w;

    w.setWindowTitle(QString("ByteIDE"));
    w.setWindowIcon(QIcon(":/res/img/mainIcon.icon"));

    w.show();
    return a.exec();
}
