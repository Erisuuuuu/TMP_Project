#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <QString>
#include <QStringList>

class RequestHandler
{
public:
    RequestHandler();
    QString handleRequest(const QString& request); // Основной метод для обработки запросов
    QString handleSolve(const QString& expression); // Метод обработки команды решения системы уравнений
private:
    QString handleRegister(const QStringList& parts); // Заглушка для регистрации
    QString handleLogin(const QStringList& parts);    // Заглушка для авторизации
    //QString handleSolve(const QString& expression); // Приватным handleSolve можно сделать, если он вызывается только внутри handleRequest
};

#endif // REQUEST_HANDLER_H
