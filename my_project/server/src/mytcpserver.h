#ifndef MYTCPSERVER_H  // Защита от повторного включения заголовочного файла
#define MYTCPSERVER_H  // Определение макроса для защиты

// Подключаем необходимые заголовочные файлы Qt
#include <QTcpServer>   // Базовый класс для TCP-сервера
#include <QTcpSocket>   // Класс для работы с TCP-сокетами
#include <QHash>        // Класс хэш-таблицы (словаря)
#include <QByteArray>   // Класс для работы с бинарными данными
#include "requesthandler.h" // Наш собственный класс для обработки запросов

// Наследуемся от QTcpServer для создания своего TCP-сервера
class MyTcpServer : public QTcpServer {
    Q_OBJECT  // Макрос для поддержки сигналов и слотов

public:
    // Конструктор с необязательным родительским объектом
    explicit MyTcpServer(QObject *parent = nullptr);

    // Метод для запуска сервера на указанном порту
    bool startServer(quint16 port);

protected:
    // Переопределяем метод обработки новых подключений
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    // Слот для чтения данных от клиента
    void onClientReadyRead();

    // Слот для обработки отключения клиента
    void onClientDisconnected();

private:
    // Метод для отправки JSON-ответа клиенту
    void sendJsonResponse(QTcpSocket* client, const QJsonObject& response);

    // Метод для отправки сообщения об ошибке
    void sendErrorResponse(QTcpSocket* client, const QString& message);

    // Список активных TCP-сокетов подключенных клиентов
    QList<QTcpSocket*> mTcpSockets;

    // Обработчик запросов (наш собственный класс)
    RequestHandler m_requestHandler;

    // Буфер для хранения данных от каждого клиента (ключ - сокет, значение - данные)
    // Используется для накопления данных, если они приходят частями
    QHash<QTcpSocket*, QByteArray> m_buffer;
};

#endif // MYTCPSERVER_H  // Конец защиты от повторного включения
