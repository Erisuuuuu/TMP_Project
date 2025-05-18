#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QTimer>

class SMTPClient : public QObject {
    Q_OBJECT
public:
    explicit SMTPClient(QObject *parent = nullptr);
    void sendEmail(const QString& to, const QString& subject, const QString& body);
    void configure(const QString& host, int port, const QString& user, const QString& password, bool useSsl);

signals:
    void finished(bool success, const QString& error);

private slots:
    void onConnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void onTimeout();

private:
    void sendCommand(const QString& command);
    void processResponse(const QString& response);

    QString m_host;
    int m_port;
    QString m_user;
    QString m_password;
    bool m_useSsl;

    enum State {
        Disconnected, Connected, EhloSent,
        AuthSent, UserSent, PassSent,
        MailFromSent, RcptToSent, DataSent,
        SendingBody, QuitSent
    };

    QSslSocket socket;
    QTimer timeoutTimer;
    State state = Disconnected;
    QString recipient;
    QString emailData;
};

#endif // SMTPCLIENT_H
