#ifndef FUNCTIONS_FOR_CLIENT_H
#define FUNCTIONS_FOR_CLIENT_H
#include<QString>
// просто файл(заголовочный) для функций
bool auth(QString,QString);

bool reg(QString,QString, QString);

bool email(QString);// функция которая будет проверять данный email пока что локально что он якобы есть в базе,чтобы далее востановить пароль

bool sendRecoveryCode(QString email);  // Новая функция для отправки кода на почту

#endif // FUNCTIONS_FOR_CLIENT_H
