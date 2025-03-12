#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    // Закрываем все сокеты
    for (QTcpSocket* socket : mTcpSockets) {
        socket->close();
        socket->deleteLater();
    }
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
    // Получаем сокет для нового подключения
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    mTcpSockets.append(socket); // Добавляем сокет в список

    // Отправляем приветственное сообщение
    socket->write("Hello, World!!! I am equation solver server!\r\n");

    // Подключаем сигналы для нового сокета
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    qDebug() << "New client connected. Total clients:" << mTcpSockets.size();
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qDebug() << "Invalid socket";
        return;
    }
    while (socket->canReadLine()) { // Читаем данные построчно
        QByteArray array = socket->readLine(); // Читаем одну строку
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
        socket->write(response.toUtf8());
        qDebug() << "Sent response to client:" << response;
    }
}

void MyTcpServer::slotClientDisconnected(){
    // Определяем, какой сокет отключился
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qDebug() << "Invalid socket";
        return;
    }

    // Удаляем сокет из списка
    mTcpSockets.removeOne(socket);

    // Закрываем и удаляем сокет
    socket->close();
    socket->deleteLater();

    qDebug() << "Client disconnected. Total clients:" << mTcpSockets.size();
}
