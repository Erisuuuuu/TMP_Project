#include "mytcpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// Конструктор TCP-сервера
MyTcpServer::MyTcpServer(QObject *parent)
    : QTcpServer(parent),
    m_requestHandler(DatabaseManager::instance())
{
    // Инициализация родительского класса QTcpServer
    // Дополнительная инициализация может быть добавлена здесь
}

// Запуск сервера на указанном порту
bool MyTcpServer::startServer(quint16 port) {
    // Пытаемся запустить сервер на всех сетевых интерфейсах (QHostAddress::Any)
    if (!listen(QHostAddress::Any, port)) {
        // В случае ошибки выводим сообщение в лог
        qCritical() << "Ошибка запуска сервера:" << errorString();
        return false;
    }
    // При успешном запуске выводим информационное сообщение
    qInfo() << "Сервер запущен на порту" << port;
    return true;
}

// Обработка нового входящего подключения
void MyTcpServer::incomingConnection(qintptr socketDescriptor) {
    // Создаем новый TCP-сокет для клиента
    QTcpSocket *client = new QTcpSocket(this);

    // Устанавливаем дескриптор сокета
    if (!client->setSocketDescriptor(socketDescriptor)) {
        // Если не удалось установить дескриптор
        qWarning() << "Ошибка установки дескриптора сокета";
        delete client;
        return;
    }

    // Подключаем сигналы сокета к слотам сервера
    connect(client, &QTcpSocket::readyRead,
            this, &MyTcpServer::onClientReadyRead);
    connect(client, &QTcpSocket::disconnected,
            this, &MyTcpServer::onClientDisconnected);

    // Добавляем клиента в список активных подключений
    mTcpSockets.append(client);
    qDebug() << "Новое подключение:" << client->peerAddress().toString();
}

// Обработка входящих данных от клиента
void MyTcpServer::onClientReadyRead() {
    // Получаем сокет, от которого пришли данные
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client || !client->bytesAvailable()) return;

    // Читаем все доступные данные
    QByteArray data = client->readAll();
    qDebug() << "Получены данные от" << client->peerAddress() << ":" << data;

    // Обработка HTTP-запроса (если есть заголовки)
    int headerEnd = data.indexOf("\r\n\r\n");
    QByteArray jsonData = (headerEnd >= 0) ?
                              data.mid(headerEnd + 4) :  // Берем тело после заголовков
                              data;                     // Или все данные как JSON

    // Парсим JSON данные
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

    if (error.error == QJsonParseError::NoError) {
        // Если JSON валиден, обрабатываем запрос
        QJsonObject response = m_requestHandler.handleJsonRequest(doc.object());
        sendJsonResponse(client, response);
    } else {
        // Ошибка парсинга JSON
        qWarning() << "Ошибка парсинга JSON:" << error.errorString();
        sendErrorResponse(client, "Неверный формат JSON: " + error.errorString());
    }
}

// Отправка JSON-ответа клиенту
void MyTcpServer::sendJsonResponse(QTcpSocket* client, const QJsonObject& response) {
    if (!client || !client->isWritable()) return;

    // Преобразуем JSON-объект в байтовый массив
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    // Формируем HTTP-заголовки
    QString headers = QString(
                          "HTTP/1.1 200 OK\r\n"
                          "Content-Type: application/json\r\n"
                          "Content-Length: %1\r\n"
                          "Connection: close\r\n\r\n"
                          ).arg(data.size());

    // Отправляем данные клиенту
    client->write(headers.toUtf8());
    client->write(data);
    client->flush();  // Принудительно отправляем буферизированные данные
    client->disconnectFromHost();  // Закрываем соединение после отправки
}

// Обработка отключения клиента
void MyTcpServer::onClientDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    qDebug() << "Клиент отключен:" << client->peerAddress().toString();

    // Удаляем клиента из списка активных подключений
    mTcpSockets.removeOne(client);

    // Очищаем буфер для этого клиента
    m_buffer.remove(client);

    // Удаляем объект сокета
    client->deleteLater();
}

// Отправка сообщения об ошибке
void MyTcpServer::sendErrorResponse(QTcpSocket* client, const QString& message) {
    if (!client || !client->isWritable()) return;

    // Формируем JSON-объект с ошибкой
    QJsonObject errorResponse;
    errorResponse["status"] = "error";
    errorResponse["message"] = message;

    // Отправляем стандартным методом
    sendJsonResponse(client, errorResponse);
}
