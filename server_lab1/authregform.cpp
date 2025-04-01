#include "authregform.h"
#include "ui_authregform.h"
#include "passwordrecoveryform.h"
#include <QMessageBox>
#include <QInputDialog>  // Подключаем для использования QInputDialog
#include <QRandomGenerator>  // Для генерации случайного кода
#include "functions_for_client.h"
#include "networkclient.h"// подключаем заголовочный файл нашего нового класса- взаимодествие с серером
// КОНСТРУКТОР
AuthRegForm::AuthRegForm(QWidget *parent)
    : QDialog(parent), ui(new Ui::AuthRegForm), authAttempts(0)
{
    ui->setupUi(this);
    change_type_to_reg(false);  // По умолчанию форма авторизации
    ui->pushButtonForgotPassword->setVisible(false);  // Кнопка "Забыли пароль" изначально скрыта
    ui->labelHelp->setVisible(false);
}
//ДЕСТРУКТОР
AuthRegForm::~AuthRegForm()
{
    delete ui;
}

// Смена типа формы (регистрация/авторизация)
void AuthRegForm::change_type_to_reg(bool is_reg)// еслли тру - регистрация- поля открыты ,если фолс - авторизация и поля тоже фолс-скрыты
{
    ui->labelPasswordCheck->setVisible(is_reg);
    ui->labelMail->setVisible(is_reg);
    ui->lineEditPasswordCheck->setVisible(is_reg);
    ui->lineEditMail->setVisible(is_reg);
    ui->pushButtonReg->setVisible(is_reg);
    ui->pushButtonAuth->setVisible(!is_reg);// здесь противоположная логика
    //при нажатии меняем текст кнопки
    ui->pushButtonChange->setText(is_reg ? "Авторизоваться" : "Зарегистрироваться");
}

// Обработка кнопки "Авторизоваться"
void AuthRegForm::on_pushButtonAuth_clicked()
{
    // Если количество попыток превышает 3, выводим сообщение и не даем вход
    if (authAttempts >= 3) {
        QMessageBox::warning(this, "Ошибка", "Вы превысили количество попыток авторизации.");
        return; // Не даем продолжить авторизацию
    }

    // Получаем данные для авторизации
    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    // Используем функцию авторизации
    if (auth(login, password)) {
        emit auth_ok(login);  // Успешная авторизация
        this->close();
    } else {
        authAttempts++;  // Увеличиваем счетчик попыток при неудачном входе
        clear();  // Очищаем поля

        // Показываем кнопку "Забыли пароль" только после первой неудачной попытки
        if (authAttempts == 1) {
            ui->pushButtonForgotPassword->setVisible(true);
            ui->labelHelp->setVisible(true);
        }

        // Выводим сообщение о неверных данных и количестве оставшихся попыток
        int remainingAttempts = 3 - authAttempts;
        QString message = QString("Логин и пароль неверны. Количество оставшихся попыток: %1 .").arg(remainingAttempts);
        QMessageBox::warning(this, "Ошибка авторизации", message);
    }
}


// Обработка кнопки "Зарегистрироваться"
void AuthRegForm::on_pushButtonReg_clicked()
{
    // Если текст введенный в поля пароль и повтор пароля не одинаков, то очищаем все поля
    if (ui->lineEditPassword->text() != ui->lineEditPasswordCheck->text()) {
        this->clear();
    } else {
        // Получаем данные для регистрации
        QString login = ui->lineEditLogin->text();
        QString password = ui->lineEditPassword->text();
        QString email = ui->lineEditMail->text();

        // Используем функцию регистрации
        if (reg(login, password, email)) {
            emit auth_ok(login);  // Успешная регистрация
            this->close();
        } else {
            this->clear();
        }
    }
}
// Слот для кнопки "Забыли пароль"
void AuthRegForm::on_pushButtonForgotPassword_clicked() {
    emit recovery_ok();


}

// Генерация кода восстановления
QString AuthRegForm::generateRecoveryCode() {
    QString code;
    for (int i = 0; i < 6; ++i) {
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));  // Генерация случайной цифры от 0 до 9
    }
    return code;
}

// Отправка кода на email
bool AuthRegForm::sendRecoveryCode(const QString& email, const QString& code) {
    qDebug() << "Отправка кода на почту: " << email;
    qDebug() << "Код: " << code;

    // Реальная логика отправки кода на почту будет здесь.
    // Например, можно интегрировать с SMTP-сервером для отправки почты.
    // Для примера, просто вернем true, чтобы симулировать успешную отправку.
    return true; // Замените эту строку на реальную логику отправки.
}

// Очистка полей ввода
void AuthRegForm::clear() {
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
    ui->lineEditPasswordCheck->clear();
    ui->lineEditMail->clear();
}

// Обработка кнопки для смены режима (авторизация/регистрация)
void AuthRegForm::on_pushButtonChange_clicked() {
    static bool isReg = false; // Переключатель режима
    isReg = !isReg; // Переключаем режим
    change_type_to_reg(isReg);
}

