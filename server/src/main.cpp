#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include "mytcpserver.h"
#include "configloader.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Для диагностики
    qDebug() << "Текущая директория:" << QDir::currentPath();

    ConfigLoader config("config.json");
    if (!config.load()) {
        qCritical() << "Ошибка загрузки конфига:" << QDir().absoluteFilePath("config.json");
        return -1;
    }

    // Проверка файла БД
    QString dbPath = config.getString("database/path");
    QFile dbFile(dbPath);
    if (!dbFile.exists()) {
        if (!dbFile.open(QIODevice::WriteOnly)) {
            qCritical() << "Ошибка создания БД:" << dbFile.errorString();
            return -1;
        }
        dbFile.close();
    }

    if (!DatabaseManager::instance().connectToDatabase(dbPath)) {
        qCritical() << "Ошибка БД:" << DatabaseManager::instance().lastError();
        return -1;
    }

    int port = config.getInt("server/port");
    MyTcpServer server;
    if (!server.startServer(port)) {
        qCritical() << "Ошибка запуска сервера";
        return -1;
    }

    qInfo() << "Сервер успешно запущен на порту" << port;
    return a.exec();
}
