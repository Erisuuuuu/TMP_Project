// Подключаем необходимые заголовочные файлы
#include "passwordrecoveryform.h"
#include "ui_passwordrecoveryform.h"  // Автогенерируемый Qt интерфейс формы
#include "networkclient.h"            // Для сетевых запросов
#include <QMessageBox>                // Для отображения всплывающих сообщений
#include <QRandomGenerator>           // Для генерации случайного кода

// Слот для отображения формы
void passwordrecoveryform::slot_show()
{
    this->show(); // Показываем текущую форму
}

// Конструктор формы
passwordrecoveryform::passwordrecoveryform(QWidget *parent) :
    QDialog(parent),                 // Инициализация базового класса QDialog
    ui(new Ui::passwordrecoveryform) // Инициализация пользовательского интерфейса
{
    ui->setupUi(this); // Настройка UI из файла .ui

    isCodeValid = false; // Флаг валидности кода (изначально false)

    // Скрываем поля для ввода нового пароля (покажутся после ввода кода)
    ui->labelCreateNewPassword->setVisible(false);
    ui->lineEditNewPassword->setVisible(false);
    ui->labelRepeatPassword->setVisible(false);
    ui->lineEditRepeatPassword->setVisible(false);
    ui->buttonConfirmNewPassword->setVisible(false);
}

// Деструктор формы
passwordrecoveryform::~passwordrecoveryform()
{
    delete ui; // Удаляем объект пользовательского интерфейса
}

// Обработчик кнопки "Отправить код"
void passwordrecoveryform::on_buttonSendCode_clicked() {
    // Получаем email из поля ввода
    QString email = ui->lineEditEmail->text();

    // Проверяем, что email не пустой
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ваш email!");
        return;
    }

    // Проверяем существование email через NetworkClient
    NetworkClient::instance().checkEmailExists(
        email,
        [this, email](bool exists, const QString& error) {
            Q_UNUSED(error); // Явно указываем, что параметр не используется

            if (exists) {
                // Генерируем 5-значный код восстановления
                QString recoveryCode = QString::number(QRandomGenerator::global()->bounded(10000, 100000));

                // Сохраняем код в переменной класса
                this->recoveryCode = recoveryCode;

                // Логируем код в консоль (для отладки)
                qDebug() << "Код восстановления:" << recoveryCode;

                // Показываем сообщение пользователю (в продакшене здесь будет отправка на email)
                QMessageBox::information(this, "Код отправлен",
                                         "Код для восстановления пароля был сгенерирован.\nДля тестирования: " + recoveryCode);

                // Показываем поля для ввода кода подтверждения
                ui->labelEnterCode->setVisible(true);
                ui->lineEditCode->setVisible(true);
                ui->buttonConfirmCode->setVisible(true);
            } else {
                QMessageBox::warning(this, "Ошибка", "Пользователь с такой почтой не найден.");
            }
        }
        );
}

// Обработчик кнопки подтверждения кода
void passwordrecoveryform::on_buttonConfirmCode_clicked()
{
    // Получаем введенный пользователем код
    QString code = ui->lineEditCode->text();

    // Сравниваем с сгенерированным кодом
    if (code == recoveryCode) {
        // Показываем поля для нового пароля
        ui->labelCreateNewPassword->setVisible(true);
        ui->lineEditNewPassword->setVisible(true);
        ui->labelRepeatPassword->setVisible(true);
        ui->lineEditRepeatPassword->setVisible(true);
        ui->buttonConfirmNewPassword->setVisible(true);

        isCodeValid = true; // Устанавливаем флаг валидности кода
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный код. Попробуйте снова.");
    }
}

// Обработчик кнопки подтверждения нового пароля
void passwordrecoveryform::on_buttonConfirmNewPassword_clicked()
{
    // Получаем введенные пароли
    QString newPassword = ui->lineEditNewPassword->text();
    QString repeatPassword = ui->lineEditRepeatPassword->text();

    // Проверяем совпадение паролей
    if (newPassword != repeatPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают. Попробуйте снова.");
        return;
    }

    // Здесь должна быть логика отправки нового пароля на сервер
    // Пока просто показываем сообщение об успехе
    QMessageBox::information(this, "Успех", "Пароль успешно изменен.");

    // Закрываем форму с результатом Accepted
    this->accept();
}
