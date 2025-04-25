#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Инициализация БД
    if (!DatabaseManager::instance().connectToDatabase("server.db")) {
        qCritical() << "Не удалось подключиться к базе данных";
        return -1;
    }

    // Запуск сервера
    MyTcpServer server;
    if (!server.startServer(33337)) {
        qCritical() << "Не удалось запустить сервер";
        return -1;
    }

    qInfo() << "Сервер успешно запущен";
    return a.exec();
} // Добавить закрывающую скобку
