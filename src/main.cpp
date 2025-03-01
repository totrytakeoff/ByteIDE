#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("ByteIDE");
    a.setOrganizationName("2509软件工作室");
    MainWindow w;

    w.setWindowTitle(QString("ByteIDE"));
    w.setWindowIcon(QIcon(":img/mainIcon.icon"));

    w.show();
    return a.exec();
}
