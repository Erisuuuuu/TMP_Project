#include "managerformss.h"
#include "authregform.h"
#include "mainwindow.h"
#include "passwordrecoveryform.h"
#include "solution.h"

ManagerFormss::ManagerFormss(QWidget *parent, MainWindow *mainWindow)
    : QMainWindow(parent), main_window(mainWindow)
{
    this->curr_auth = new AuthRegForm();// создаем первую форму
    this->curr_auth->show();// показываем первую форму
    this->recovery_password = new passwordrecoveryform();
    this->curr_solution = new Solution;
    // Подключение сигнала авторизации к слоту отображения в главном окне
    connect(curr_auth, &AuthRegForm::auth_ok, main_window, &MainWindow::slot_show);
    connect(curr_auth, &AuthRegForm::recovery_ok, recovery_password, &passwordrecoveryform::slot_show);
    connect(main_window, &MainWindow::solution_ok,curr_solution,&Solution::slot_show);

    // источник сигнала -наша форма авторизации ведь на ней кнопка забылт пароль,
    //далее идет название сигнала
    //
}

ManagerFormss::~ManagerFormss()
{
    delete curr_auth;
}
