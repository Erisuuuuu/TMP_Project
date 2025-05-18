#include "managerformss.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    MainWindow *mainWindow = new MainWindow();
    new ManagerFormss(nullptr, mainWindow); // Не сохраняем в переменную
    return a.exec();
}
