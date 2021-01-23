#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle(QStyleFactory::create("Fusion"));
    QCoreApplication::setOrganizationName("Oscar Vegener");
    QCoreApplication::setApplicationName("UnderManager");
    MainWindow w;
    w.show();
    return a.exec();
}
