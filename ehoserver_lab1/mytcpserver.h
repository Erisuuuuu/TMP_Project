#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "requesthandler.h" // Подключаем класс RequestHandler

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer* mTcpServer;
    QTcpSocket* mTcpSocket;
    RequestHandler requestHandler; // Объект для обработки запросов
};

#endif // MYTCPSERVER_H



