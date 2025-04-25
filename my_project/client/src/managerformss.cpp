// Подключаем необходимые заголовочные файлы
#include "managerformss.h"         // Заголовок текущего класса
#include "authregform.h"          // Форма авторизации/регистрации
#include "mainwindow.h"           // Главное окно приложения
#include "passwordrecoveryform.h" // Форма восстановления пароля
#include "solution.h"             // Форма с решением уравнений

// Конструктор класса ManagerFormss
ManagerFormss::ManagerFormss(QWidget *parent, MainWindow *mainWindow)
    : QMainWindow(parent)        // Инициализация базового класса QMainWindow
    , main_window(mainWindow)    // Сохраняем указатель на главное окно
{
    // 1. Создаем и показываем форму авторизации
    this->curr_auth = new AuthRegForm(); // Создаем экземпляр формы авторизации
    this->curr_auth->show();             // Делаем форму видимой

    // 2. Создаем (но не показываем) другие формы
    this->recovery_password = new passwordrecoveryform(); // Форма восстановления пароля
    this->curr_solution = new Solution();                // Форма отображения решения

    // 3. Настраиваем соединения сигналов и слотов:

    // Когда auth_ok сигнализирует об успешной авторизации,
    // вызываем slot_show главного окна
    connect(curr_auth, &AuthRegForm::auth_ok,
            main_window, &MainWindow::slot_show);

    // Когда recovery_ok сигнализирует о запросе восстановления,
    // вызываем slot_show формы восстановления пароля
    connect(curr_auth, &AuthRegForm::recovery_ok,
            recovery_password, &passwordrecoveryform::slot_show);

    // Когда solution_ok сигнализирует о готовности решения,
    // вызываем slot_show формы с решением
    connect(main_window, &MainWindow::solution_ok,
            curr_solution, &Solution::slot_show);
}

// Деструктор класса
ManagerFormss::~ManagerFormss()
{
    // Удаляем только форму авторизации, так как:
    // - main_window удаляется где-то в другом месте
    // - остальные формы будут удалены автоматически благодаря
    //   механизму parent-child в Qt (если установлены родители)
    delete curr_auth;
}
