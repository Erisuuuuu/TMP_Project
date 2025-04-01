#include "networkclient.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>
#include <QDebug>

// Реализация синглтона
NetworkClient* NetworkClient::instance()
{
    static NetworkClient instance;  // Синглтон: один экземпляр
    return &instance;
}

// Конструктор
NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent), networkManager(this)
{
}

// Метод для авторизации
bool NetworkClient::authenticate(const QString& login, const QString& password)
{
    QUrl url("http://example.com/api/authenticate");  // Замените на реальный URL
    QNetworkRequest request(url);

    // Создаем JSON с логином и паролем
    QJsonObject json;
    json["login"] = login;
    json["password"] = password;

    // Преобразуем JSON в QByteArray для отправки
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Отправляем POST-запрос
    QNetworkReply* reply = networkManager.post(request, data);

    // Ожидаем завершения запроса
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();  // Ожидаем ответа

    // Проверка ответа от сервера
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        // Обработка успешного ответа
        QJsonDocument responseDoc = QJsonDocument::fromJson(response);
        QJsonObject responseObj = responseDoc.object();

        return responseObj["status"] == "success";  // Проверка, что статус "success"
    } else {
        qDebug() << "Ошибка при авторизации: " << reply->errorString();
        return false;  // Ошибка при авторизации
    }
}

// Метод для регистрации
bool NetworkClient::registerUser(const QString& login, const QString& password, const QString& email)
{
    QUrl url("http://example.com/api/register");  // Замените на реальный URL
    QNetworkRequest request(url);

    // Создаем JSON с логином, паролем и email
    QJsonObject json;
    json["login"] = login;
    json["password"] = password;
    json["email"] = email;

    // Преобразуем JSON в QByteArray для отправки
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Отправляем POST-запрос
    QNetworkReply* reply = networkManager.post(request, data);

    // Ожидаем завершения запроса
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();  // Ожидаем ответа

    // Проверка ответа от сервера
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        // Обработка успешного ответа
        QJsonDocument responseDoc = QJsonDocument::fromJson(response);
        QJsonObject responseObj = responseDoc.object();

        return responseObj["status"] == "success";  // Проверка, что статус "success"
    } else {
        qDebug() << "Ошибка при регистрации: " << reply->errorString();
        return false;  // Ошибка при регистрации
    }
}
