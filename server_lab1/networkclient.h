#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class NetworkClient : public QObject
{
    Q_OBJECT

public:
    static NetworkClient* instance();  // Получение единственного экземпляра синглтона

    bool authenticate(const QString& login, const QString& password);  // Авторизация
    bool registerUser(const QString& login, const QString& password, const QString& email);  // Регистрация

private:
    explicit NetworkClient(QObject *parent = nullptr);  // Конструктор для синглтона
    QNetworkAccessManager networkManager;  // Менеджер для отправки сетевых запросов
};

#endif // NETWORKCLIENT_H

