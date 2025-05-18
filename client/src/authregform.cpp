#include "authregform.h"
#include "ui_authregform.h"
#include "networkclient.h"
#include "passwordrecoveryform.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QKeyEvent>
AuthRegForm::AuthRegForm(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AuthRegForm)
{
    ui->setupUi(this);

    // Начальная настройка формы (режим авторизации)
    change_type_to_reg(false);

    // Кнопка восстановления пароля и подсказка всегда видимы
    ui->pushButtonForgotPassword->setVisible(true);
    ui->labelHelp->setVisible(true);

    // Убираем фокус с кнопки "Забыли пароль"
    ui->pushButtonForgotPassword->clearFocus();

    // Убираем фокус с кнопки "Забыли пароль" и всех других элементов
    ui->pushButtonForgotPassword->setDefault(false);
    ui->pushButtonAuth->setDefault(true);

    // Устанавливаем фокус на кнопку "Авторизация"
    ui->pushButtonAuth->setFocus();

    // Устанавливаем фокус на поле для ввода логина (если оно пустое)
    if (ui->lineEditLogin->text().isEmpty()) {
        ui->lineEditLogin->setFocus();
    } else if (ui->lineEditPassword->text().isEmpty()) {
        ui->lineEditPassword->setFocus();
    } else if (ui->lineEditPasswordCheck->isVisible() && ui->lineEditPasswordCheck->text().isEmpty()) {
        ui->lineEditPasswordCheck->setFocus();
    } else if (ui->lineEditMail->isVisible() && ui->lineEditMail->text().isEmpty()) {
        ui->lineEditMail->setFocus();
    }
}

void AuthRegForm::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);  // Вызов родительского showEvent

    // Устанавливаем фокус на поле для ввода логина
    ui->lineEditLogin->setFocus();
}
// Деструктор формы
AuthRegForm::~AuthRegForm()
{
    delete ui;
}

// Переключение между режимами авторизации и регистрации
void AuthRegForm::change_type_to_reg(bool is_reg)
{
    ui->labelPasswordCheck->setVisible(is_reg);
    ui->labelMail->setVisible(is_reg);
    ui->lineEditPasswordCheck->setVisible(is_reg);
    ui->lineEditMail->setVisible(is_reg);
    ui->pushButtonReg->setVisible(is_reg);
    ui->pushButtonAuth->setVisible(!is_reg);
    ui->pushButtonChange->setText(is_reg ? "Авторизоваться" : "Зарегистрироваться");
}

// Обработка нажатия кнопки авторизации
void AuthRegForm::on_pushButtonAuth_clicked()
{
    QString login = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();

    if (login.isEmpty() || password.isEmpty()) {
        QStringList missingFields;

        if (login.isEmpty()) missingFields << "логина";
        if (password.isEmpty()) missingFields << "пароля";

        QString message;
        if (missingFields.size() == 2) {
            message = "Не заполнены поля логина и пароля.\nЗаполните их и попробуйте снова.";
        } else {
            message = QString("Не заполнено поле %1.\nПожалуйста, введите его и попробуйте снова.")
                          .arg(missingFields.first());
        }

        QMessageBox::warning(this, "Ошибка", message);

        // Устанавливаем фокус в первое незаполненное поле
        if (login.isEmpty()) {
            ui->lineEditLogin->setFocus();
        } else if (password.isEmpty()) {
            ui->lineEditPassword->setFocus();
        }

        return;
    }

    ui->pushButtonAuth->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    NetworkClient::instance().authenticate(
        login,
        password,
        [this, login](bool success, const QString& errorMessage) {
            ui->pushButtonAuth->setEnabled(true);
            QApplication::restoreOverrideCursor();

            if(success) {
                emit auth_ok(login);
                this->close();
            } else {
                clear();
                QMessageBox::warning(this, "Ошибка", QString("Ошибка авторизации: %1").arg(errorMessage));
            }
        }
        );
}

// Обработка нажатия кнопки регистрации
void AuthRegForm::on_pushButtonReg_clicked()
{
    QString login = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();
    QString passwordCheck = ui->lineEditPasswordCheck->text().trimmed();
    QString email = ui->lineEditMail->text().trimmed();

    if(password != passwordCheck) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        clear();
        return;
    }

    if(login.isEmpty() || password.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    ui->pushButtonReg->setEnabled(false);
    ui->pushButtonReg->setText("Регистрация...");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    NetworkClient::instance().registerUser(
        login, password, email,
        [this](bool success, const QString& message) {
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
void AuthRegForm::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {  // Если нажата клавиша Enter
        if (ui->lineEditLogin->hasFocus()) {
            ui->lineEditPassword->setFocus();  // Перемещаем фокус на поле пароля
        } else if (ui->lineEditPassword->hasFocus()) {
            ui->lineEditPasswordCheck->setFocus();  // Перемещаем фокус на повтор пароля, если он есть
        } else if (ui->lineEditPasswordCheck->hasFocus()) {
            ui->lineEditMail->setFocus();  // Перемещаем фокус на поле Email
        } else if (ui->lineEditMail->hasFocus()) {
            ui->pushButtonReg->click();  // Если все поля заполнены, имитируем клик по кнопке регистрации
        }
    } else {
        QDialog::keyPressEvent(event);  // Обработка всех остальных событий
    }
}
// Обработка кнопки "Забыли пароль"
void AuthRegForm::on_pushButtonForgotPassword_clicked() {
    emit recovery_ok();
}

// Генерация 6-значного кода восстановления
QString AuthRegForm::generateRecoveryCode() {
    QString code;
    for (int i = 0; i < 6; ++i) {
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));
    }
    return code;
}

// Отправка кода восстановления на email (заглушка)
bool AuthRegForm::sendRecoveryCode(const QString& email, const QString& code) {
    qDebug() << "Отправка кода на почту: " << email;
    qDebug() << "Код: " << code;
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
    static bool isReg = false;
    isReg = !isReg;
    change_type_to_reg(isReg);
}
