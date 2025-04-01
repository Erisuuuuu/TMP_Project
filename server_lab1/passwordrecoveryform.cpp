#include "passwordrecoveryform.h"
#include "ui_passwordrecoveryform.h"  // Подключение файла интерфейса
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>
void passwordrecoveryform::slot_show()
{
    this->show();
}
passwordrecoveryform::passwordrecoveryform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwordrecoveryform)
{
    ui->setupUi(this);
    isCodeValid = false;

    // Скрываем поля для нового пароля до ввода правильного кода
    ui->labelCreateNewPassword->setVisible(false);
    ui->lineEditNewPassword->setVisible(false);
    ui->labelRepeatPassword->setVisible(false);
    ui->lineEditRepeatPassword->setVisible(false);
    ui->buttonConfirmNewPassword->setVisible(false);
}

passwordrecoveryform::~passwordrecoveryform()
{
    delete ui;
}

void passwordrecoveryform::on_buttonSendCode_clicked()
{
    QString email = ui->lineEditEmail->text();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ваш email!");
        return;
    }

    // Локальная имитация поиска пользователя с таким email в базе данных
    // (позже можно будет заменить на реальную проверку)
    bool userExists = (email == "user.com");  // Локально задали почту: user@example.com

    if (userExists) {
        // Генерация и отправка кода на почту
        recoveryCode = QString::number(QRandomGenerator::global()->bounded(10000, 100000));  // Генерация 5-значного кода

        // Выводим сгенерированный код в консоль
        qDebug() << "Сгенерированный код восстановления:" << recoveryCode;

        // Реальная отправка email с recoveryCode будет реализована позже
        QMessageBox::information(this, "Код отправлен", "Код для восстановления пароля отправлен на вашу почту.");

        // Показываем поля для ввода кода
        ui->labelEnterCode->setVisible(true);
        ui->lineEditCode->setVisible(true);
        ui->buttonConfirmCode->setVisible(true);
    } else {
        QMessageBox::warning(this, "Ошибка", "Пользователь с такой почтой не найден.");
    }
}

void passwordrecoveryform::on_buttonConfirmCode_clicked()
{
    QString code = ui->lineEditCode->text();  // Код, введенный пользователем

    // Проверка кода
    if (code == recoveryCode) {
        ui->labelCreateNewPassword->setVisible(true);  // Показываем поле для нового пароля
        ui->lineEditNewPassword->setVisible(true);
        ui->labelRepeatPassword->setVisible(true);  // Показываем поле для повторного пароля
        ui->lineEditRepeatPassword->setVisible(true);
        ui->buttonConfirmNewPassword->setVisible(true);
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный код. Попробуйте снова.");
    }
}

void passwordrecoveryform::on_buttonConfirmNewPassword_clicked()
{
    QString newPassword = ui->lineEditNewPassword->text();
    QString repeatPassword = ui->lineEditRepeatPassword->text();

    if (newPassword != repeatPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают. Попробуйте снова.");
        return;
    }

    // Логика для изменения пароля
    // Например, сохраняем новый пароль (в будущем может быть реализована передача на сервер)
    QMessageBox::information(this, "Успех", "Пароль успешно изменен.");
    this->accept();  // Закрываем окно
}

