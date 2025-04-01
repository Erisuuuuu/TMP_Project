#include "managerformss.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow *mainWindow = new MainWindow();
    ManagerFormss *manager = new ManagerFormss(nullptr, mainWindow);

    manager->show();
    return a.exec();
}
