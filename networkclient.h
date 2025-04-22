#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <functional>

class NetworkClient : public QObject {
    Q_OBJECT
public:
    static NetworkClient& instance();

    void authenticate(const QString& login,
                      const QString& password,
                      std::function<void(bool, const QString&)> callback);

    void registerUser(const QString& login,
                      const QString& password,
                      const QString& email,
                      std::function<void(bool, const QString&)> callback);

    void sendSolutionRequest(const QJsonObject& data,
                             std::function<void(bool, const QString&)> callback);

    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;
    void checkEmailExists(const QString& email,
                          std::function<void(bool, const QString&)> callback);


signals:
    void networkErrorOccurred(const QString& error);

private slots:
    void handleReply(QNetworkReply* reply);

private:
    explicit NetworkClient(QObject *parent = nullptr);
    QNetworkAccessManager* networkManager;
    QMap<QNetworkReply*, std::function<void(bool, const QString&)>> callbacks;
};

#endif
