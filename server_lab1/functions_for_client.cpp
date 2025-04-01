#include "functions_for_client.h"
#include<QString>
#include <QDebug>

//пока не зная как подключаться к сервиру провереям все локально:
// функция авторизации:
bool auth(QString login,QString password)
{       // в дальнейшим нужно будет тут делать запрос к серверу
    if (login == "user" && password == "123")
    {
        return true;
    }
    else
        return false;

}
// функция регестрации ( проверяет если такого логина и пароля еще нет в системе ,то можно зарегестрировать нового пользователя)
bool reg(QString login,QString password, QString mail)
{       // в дальнейшим нужно будет тут делать запрос к серверу на регистрацию и авторизацию
    // то есть если на сервер поступает запрос провести регистрацию,то он обязательно после регистрации должен провести авторизацию)
    if (login != "user" && password != "123")
    {
        return true;
    }
    else
        return false;

}
bool email(QString email)
{
    if (email == "user.com")
    {
        return true;
    }
    else
        return false;
}

bool sendRecoveryCode(QString email)
{
    // Здесь будет логика отправки почты на сервер
    qDebug() << "Отправка кода на почту: " << email;
    return true;
}
