#include "networkclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>

NetworkClient& NetworkClient::instance() {
    static NetworkClient instance;
    return instance;
}

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent),
    networkManager(new QNetworkAccessManager(this))
{
    networkManager->setTransferTimeout(5000);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

// Основные методы аутентификации
void NetworkClient::authenticate(const QString& login,
                                 const QString& password,
                                 std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{
        {"action", "login"},
        {"login", login},
        {"password", password}
    };

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

void NetworkClient::registerUser(const QString& login,
                                 const QString& password,
                                 const QString& email,
                                 std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{
        {"action", "register"},
        {"login", login},
        {"password", password},
        {"email", email}
    };

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

// Методы восстановления пароля
void NetworkClient::sendPasswordResetRequest(const QString& loginOrEmail,
                                             std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/password_reset_request");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{
        {"action", "password_reset_request"},
        {"login_or_email", loginOrEmail}
    };

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

void NetworkClient::validateResetCode(const QString& loginOrEmail,
                                      const QString& code,
                                      std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/password_reset_confirm");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{
        {"action", "password_reset_confirm"},
        {"login_or_email", loginOrEmail},
        {"code", code}
    };

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

void NetworkClient::updatePassword(const QString& loginOrEmail,
                                   const QString& newPassword,
                                   std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/password_reset_update");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{
        {"action", "password_reset_update"},
        {"login_or_email", loginOrEmail},
        {"new_password", newPassword}
    };

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

// Методы проверки данных
void NetworkClient::checkEmailExists(const QString& email,
                                     std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/check_email");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json{{"email", email}};

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}


// Метод решения уравнений
void NetworkClient::sendSolutionRequest(const QJsonObject& data,
                                        std::function<void(bool, const QString&)> callback)
{
    QUrl url("http://localhost:33337/api/solve");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(data).toJson());
    callbacks[reply] = callback;
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });
}

// Обработчик всех ответов
void NetworkClient::handleReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::OperationCanceledError) {
        emit networkErrorOccurred("Request timeout: no response from server");
        reply->deleteLater();
        return;
    }

    if (!callbacks.contains(reply)) {
        reply->deleteLater();
        return;
    }

    auto callback = callbacks.take(reply);
    QString message;
    bool success = false;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        // 🐞 ОТЛАДКА: печатаем ответ сервера
        qDebug() << "Сырой ответ от сервера:" << responseData;

        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if (!doc.isNull()) {
            QJsonObject response = doc.object();
            success = response["status"].toString() == "success";

            // Если есть поле message, используем его
            message = response["message"].toString();

            // Если нет message, но успех, то сериализуем весь объект
            if (message.trimmed().isEmpty() && success) {
                message = QString::fromUtf8(responseData);
            }
        } else {
            message = "Invalid JSON response";
        }
    } else {
        message = reply->errorString();
        emit networkErrorOccurred(message);
    }

    qDebug() << "Ответ от сервера:" << message; // Логируем сообщение от сервера


    callback(success, message);
    reply->deleteLater();
}

