#include "authregform.h"
#include "ui_authregform.h"
#include "networkclient.h"        // Для работы с сетевыми запросами
#include "passwordrecoveryform.h" // Форма восстановления пароля
#include <QMessageBox>            // Для показа сообщений
#include <QInputDialog>           // Для диалогов ввода
#include <QRandomGenerator>       // Для генерации случайных кодов

// Конструктор формы
AuthRegForm::AuthRegForm(QWidget *parent)
    : QDialog(parent),            // Инициализация базового класса QDialog
    ui(new Ui::AuthRegForm),    // Создание UI формы
    authAttempts(0)             // Инициализация счетчика попыток авторизации
{
    ui->setupUi(this);  // Настройка UI компонентов

    // Начальная настройка формы (режим авторизации)
    change_type_to_reg(false);

    // Скрываем кнопку восстановления пароля и подсказку
    ui->pushButtonForgotPassword->setVisible(false);
    ui->labelHelp->setVisible(false);
}

// Деструктор формы
AuthRegForm::~AuthRegForm()
{
    delete ui;  // Удаление UI компонентов
}

// Переключение между режимами авторизации и регистрации
void AuthRegForm::change_type_to_reg(bool is_reg)
{
    // Показываем/скрываем элементы в зависимости от режима
    ui->labelPasswordCheck->setVisible(is_reg);   // Подтверждение пароля
    ui->labelMail->setVisible(is_reg);            // Поле email
    ui->lineEditPasswordCheck->setVisible(is_reg); // Поле для подтверждения пароля
    ui->lineEditMail->setVisible(is_reg);         // Поле для email
    ui->pushButtonReg->setVisible(is_reg);        // Кнопка регистрации
    ui->pushButtonAuth->setVisible(!is_reg);      // Кнопка авторизации

    // Меняем текст кнопки переключения режима
    ui->pushButtonChange->setText(is_reg ? "Авторизоваться" : "Зарегистрироваться");
}

// Обработка нажатия кнопки авторизации
void AuthRegForm::on_pushButtonAuth_clicked()
{
    // Проверка количества попыток
    if (authAttempts >= 3) {
        QMessageBox::warning(this, "Ошибка", "Вы превысили количество попыток авторизации.");
        return;
    }

    // Получаем и очищаем введенные данные
    QString login = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();

    // Проверка заполненности полей
    if(login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    // Блокируем кнопку и показываем курсор ожидания
    ui->pushButtonAuth->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Отправляем запрос на авторизацию через NetworkClient
    NetworkClient::instance().authenticate(
        login,
        password,
        [this, login](bool success, const QString& errorMessage) {
            // Обработка ответа от сервера (лямбда-функция)

            // Восстанавливаем UI
            ui->pushButtonAuth->setEnabled(true);
            QApplication::restoreOverrideCursor();

            if(success) {
                // При успешной авторизации
                emit auth_ok(login);  // Отправляем сигнал об успехе
                this->close();        // Закрываем форму
            } else {
                // При ошибке авторизации
                authAttempts++;       // Увеличиваем счетчик попыток
                clear();              // Очищаем поля

                // После первой неудачной попытки показываем кнопку восстановления
                if (authAttempts == 1) {
                    ui->pushButtonForgotPassword->setVisible(true);
                    ui->labelHelp->setVisible(true);
                }

                // Формируем сообщение об ошибке
                QString message = QString("Ошибка авторизации: %1\nОсталось попыток: %2")
                                      .arg(errorMessage)
                                      .arg(3 - authAttempts);
                QMessageBox::warning(this, "Ошибка", message);
            }
        }
        );
}

// Обработка нажатия кнопки регистрации
void AuthRegForm::on_pushButtonReg_clicked()
{
    // Получаем и очищаем введенные данные
    QString login = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();
    QString passwordCheck = ui->lineEditPasswordCheck->text().trimmed();
    QString email = ui->lineEditMail->text().trimmed();

    // Валидация данных
    if(password != passwordCheck) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        clear();
        return;
    }

    if(login.isEmpty() || password.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    // Блокируем кнопку и меняем ее текст
    ui->pushButtonReg->setEnabled(false);
    ui->pushButtonReg->setText("Регистрация...");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents(); // Обновляем UI

    // Отправляем запрос на регистрацию
    NetworkClient::instance().registerUser(
        login, password, email,
        [this](bool success, const QString& message) {
            // Обработка ответа от сервера

            // Восстанавливаем UI
            ui->pushButtonReg->setEnabled(true);
            ui->pushButtonReg->setText("Регистрация");
            QApplication::restoreOverrideCursor();

            if (success) {
                QMessageBox::information(this, "Успех", "Регистрация завершена");
            } else {
                QMessageBox::critical(this, "Ошибка", message);
            }
        }
        );
}

// Обработка кнопки "Забыли пароль"
void AuthRegForm::on_pushButtonForgotPassword_clicked() {
    emit recovery_ok();  // Отправляем сигнал о переходе к восстановлению пароля
}

// Генерация 6-значного кода восстановления
QString AuthRegForm::generateRecoveryCode() {
    QString code;
    for (int i = 0; i < 6; ++i) {
        // Генерация случайной цифры от 0 до 9
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));
    }
    return code;
}

// Отправка кода восстановления на email (заглушка)
bool AuthRegForm::sendRecoveryCode(const QString& email, const QString& code) {
    qDebug() << "Отправка кода на почту: " << email;
    qDebug() << "Код: " << code;

    // В реальной реализации здесь должна быть отправка email
    // Сейчас просто возвращаем true для тестирования
    return true;
}

// Очистка всех полей формы
void AuthRegForm::clear() {
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
    ui->lineEditPasswordCheck->clear();
    ui->lineEditMail->clear();
}

// Обработка переключения между режимами авторизации/регистрации
void AuthRegForm::on_pushButtonChange_clicked() {
    static bool isReg = false;  // Статическая переменная для хранения состояния
    isReg = !isReg;            // Переключаем состояние
    change_type_to_reg(isReg); // Обновляем форму
}
