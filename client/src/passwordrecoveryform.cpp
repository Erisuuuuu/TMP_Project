// Подключаем необходимые заголовочные файлы
#include "passwordrecoveryform.h"
#include "ui_passwordrecoveryform.h"  // Автогенерируемый Qt интерфейс формы
#include "networkclient.h"            // Для сетевых запросов
#include <QMessageBox>                // Для отображения всплывающих сообщений
#include <QRandomGenerator>           // Для генерации случайного кода
#include <QKeyEvent>
void passwordrecoveryform::slot_show()
{
    this->show(); // Показываем форму

    // Устанавливаем фокус в первое незаполненное поле
    if (ui->lineEditLogin->text().isEmpty()) {
        ui->lineEditLogin->setFocus();
    } else if (ui->lineEditCode->isVisible() && ui->lineEditCode->text().isEmpty()) {
        ui->lineEditCode->setFocus();
    } else if (ui->lineEditNewPassword->isVisible() && ui->lineEditNewPassword->text().isEmpty()) {
        ui->lineEditNewPassword->setFocus();
    } else if (ui->lineEditRepeatPassword->isVisible() && ui->lineEditRepeatPassword->text().isEmpty()) {
        ui->lineEditRepeatPassword->setFocus();
    }
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
// В слоте on_buttonSendCode_clicked()
void passwordrecoveryform::on_buttonSendCode_clicked() {
    QString login = ui->lineEditLogin->text().trimmed(); // Убедитесь что имя виджета совпадает

    if (login.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ваш логин!");
        return;
    }

    NetworkClient::instance().sendPasswordResetRequest(
        login,
        [this](bool success, const QString& message) {
            if (success) {
                QMessageBox::information(this, "Успех", "Код отправлен на привязанную почту");

                // Показать следующие элементы формы
                ui->labelEnterCode->setVisible(true);
                ui->lineEditCode->setVisible(true);
                ui->buttonConfirmCode->setVisible(true);

                // Установить фокус в поле ввода кода
                ui->lineEditCode->setFocus();
            }
        }
        );
}

// Обработчик кнопки подтверждения кода
void passwordrecoveryform::on_buttonConfirmCode_clicked() {
    QString code = ui->lineEditCode->text().trimmed();
    QString login = ui->lineEditLogin->text().trimmed();

    if(code.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the verification code");
        return;
    }

    NetworkClient::instance().validateResetCode(
        login,
        code,
        [this](bool success, const QString& message) {
            if(success) {
                ui->labelCreateNewPassword->setVisible(true);
                ui->lineEditNewPassword->setVisible(true);
                ui->labelRepeatPassword->setVisible(true);
                ui->lineEditRepeatPassword->setVisible(true);
                ui->buttonConfirmNewPassword->setVisible(true);

                // Устанавливаем фокус на первое незаполненное поле
                setFocusToFirstEmptyField();
            } else {
                QMessageBox::warning(this, "Error", message);
            }
        }
        );
}

// Функция для установки фокуса на первое пустое поле
void passwordrecoveryform::setFocusToFirstEmptyField() {
    if (ui->lineEditNewPassword->text().isEmpty()) {
        ui->lineEditNewPassword->setFocus();
    } else if (ui->lineEditRepeatPassword->text().isEmpty()) {
        ui->lineEditRepeatPassword->setFocus();
    }
}

// Переопределение keyPressEvent для перехода между полями при нажатии Enter
void passwordrecoveryform::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        // Если нажали Enter, находим следующее пустое поле и устанавливаем на него фокус
        moveFocusToNextEmptyField();
    }
}

// Функция для перемещения фокуса на следующее пустое поле
void passwordrecoveryform::moveFocusToNextEmptyField() {
    if (ui->lineEditNewPassword->text().isEmpty()) {
        ui->lineEditNewPassword->setFocus();
    } else if (ui->lineEditRepeatPassword->text().isEmpty()) {
        ui->lineEditRepeatPassword->setFocus();
    }
}
// Обработчик кнопки подтверждения нового пароля
void passwordrecoveryform::on_buttonConfirmNewPassword_clicked()
{
    QString newPassword = ui->lineEditNewPassword->text();
    QString repeatPassword = ui->lineEditRepeatPassword->text();
    QString login = ui->lineEditLogin->text().trimmed();

    if(newPassword.isEmpty() || repeatPassword.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите новый пароль!");
        return;
    }

    if(newPassword != repeatPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        return;
    }

    NetworkClient::instance().validateResetCode(
        login,
        ui->lineEditCode->text().trimmed(),
        [this, login, newPassword](bool success, const QString& message) {
            if(success) {
                // Отправка нового пароля на сервер
                NetworkClient::instance().updatePassword(
                    login,
                    newPassword,
                    [this](bool success, const QString& message) {
                        if(success) {
                            QMessageBox::information(this, "Успех", "Пароль успешно изменен!");
                            this->accept();
                        } else {
                            QMessageBox::critical(this, "Ошибка", message);
                        }
                    }
                    );
            } else {
                QMessageBox::warning(this, "Ошибка", message);
            }
        }
        );
}


