// networkclient.cpp
#include "networkclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>

// Реализация шаблона Singleton
NetworkClient& NetworkClient::instance() {
    static NetworkClient instance;  // Создается при первом вызове
    return instance;  // Возвращаем ссылку на единственный экземпляр
}

// Приватный конструктор
NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent),
    networkManager(new QNetworkAccessManager(this))  // Инициализация менеджера запросов
{
    // Настройка параметров соединения:
    networkManager->setTransferTimeout(5000);  // Таймаут 5 секунд
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);  // Политика редиректов
}

// Метод аутентификации
void NetworkClient::authenticate(const QString& login,
                                 const QString& password,
                                 std::function<void(bool, const QString&)> callback) {
    // Формируем URL для запроса авторизации
    QUrl url("http://localhost:33337/api/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "login";  // Тип операции
    json["login"] = login;     // Логин пользователя
    json["password"] = password; // Пароль

    // Отправляем POST-запрос и сохраняем callback
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;

    // Подключаем обработчик завершения запроса
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply);
    });
}

// Метод регистрации пользователя
void NetworkClient::registerUser(const QString& login,
                                 const QString& password,
                                 const QString& email,
                                 std::function<void(bool, const QString&)> callback) {
    // Формируем URL для запроса регистрации
    QUrl url("http://localhost:33337/api/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными регистрации
    QJsonObject json;
    json["action"] = "register";  // Тип операции
    json["login"] = login;       // Логин
    json["password"] = password; // Пароль
    json["email"] = email;      // Email

    // Отправляем запрос и сохраняем callback
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;

    // Подключаем обработчик ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply);
    });
}

// Метод обработки всех ответов от сервера
void NetworkClient::handleReply(QNetworkReply* reply) {
    // Проверка на отмену запроса (например, по таймауту)
    if (reply->error() == QNetworkReply::OperationCanceledError) {
        emit networkErrorOccurred("Request timeout: no response from server");
        reply->deleteLater();
        return;
    }

    // Проверяем, есть ли callback для этого запроса
    if (!callbacks.contains(reply)) {
        reply->deleteLater();
        return;
    }

    // Получаем callback для этого запроса
    auto callback = callbacks.take(reply);
    QString message;
    bool success = false;

    // Обработка успешного ответа
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (doc.isNull()) {
            message = "Invalid JSON response";
        } else {
            QJsonObject response = doc.object();
            success = response["status"].toString() == "success";
            message = response["message"].toString();
        }
    } else {
        // Обработка ошибки сети
        message = reply->errorString();
        emit networkErrorOccurred(message);
    }

    // Вызываем callback с результатом
    callback(success, message);

    // Освобождаем ресурсы
    reply->deleteLater();
}

// Метод отправки системы уравнений
void NetworkClient::sendSolutionRequest(const QJsonObject& data,
                                        std::function<void(bool, const QString&)> callback) {
    // Формируем URL для запроса решения
    QUrl url("http://localhost:33337/api/solve");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Отправляем данные уравнений
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(data).toJson());
    callbacks[reply] = callback;

    // Подключаем обработчик ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply);
    });
}

// Метод проверки существования email
void NetworkClient::checkEmailExists(const QString& email,
                                     std::function<void(bool, const QString&)> callback) {
    // Формируем URL для проверки email
    QUrl url("http://localhost:33337/api/check_email");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON с email для проверки
    QJsonObject json;
    json["email"] = email;

    // Отправляем запрос
    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;

    // Подключаем обработчик ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply);
    });
}
