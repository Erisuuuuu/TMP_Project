#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    mTcpSocket = mTcpServer->nextPendingConnection();
    mTcpSocket->write("Hello, World!!! I am equation solver server!\r\n");
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead(){
    while (mTcpSocket->canReadLine()) { // Читаем данные построчно
        QByteArray array = mTcpSocket->readLine(); // Читаем одну строку
        QString request = QString::fromUtf8(array).trimmed(); // Убираем лишние пробелы и символы новой строки

        qDebug() << "Received data:" << request;

        // Игнорируем пустые строки
        if (request.isEmpty()) {
            qDebug() << "Received empty data, ignoring...";
            continue;
        }

        // Передаем запрос в RequestHandler для обработки
        QString response = requestHandler.handleRequest(request);

        // Отправляем ответ клиенту
        mTcpSocket->write(response.toUtf8());
        qDebug() << "Sent response:" << response;
    }
}

void MyTcpServer::slotClientDisconnected(){
    qDebug() << "Client disconnected";
    mTcpSocket->close();
}
