#ifndef MANAGERFORMSS_H
#define MANAGERFORMSS_H

#include <QMainWindow>
#include "authregform.h"
#include "mainwindow.h"
#include "passwordrecoveryform.h"
#include "solution.h"

class ManagerFormss : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerFormss(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~ManagerFormss();

private:
    AuthRegForm *curr_auth;
    passwordrecoveryform *recovery_password;
    Solution *curr_solution;
    MainWindow *main_window;
};

#endif // MANAGERFORMSS_H
