#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QJsonObject>
#include <QString>
#include "databasemanager.h"

class RequestHandler {
public:
    explicit RequestHandler(DatabaseManager& db);

    QJsonObject handleJsonRequest(const QJsonObject& request);
    QString handleTextRequest(const QString& request);

private:
    // Обработчики JSON-запросов
    QJsonObject processRegister(const QJsonObject& request);
    QJsonObject processLogin(const QJsonObject& request);
    QJsonObject processPasswordResetRequest(const QJsonObject& request);
    QJsonObject processPasswordResetConfirm(const QJsonObject& request);
    QJsonObject processPasswordResetUpdate(const QJsonObject& request);
    QJsonObject processCheckEmail(const QJsonObject& request);

    // Обработчики текстовых команд
    QString handleTextRegister(const QStringList& parts);
    QString handleTextLogin(const QStringList& parts);
    QString handleSolve(const QString& expression);

    // Вспомогательные методы
    bool validateRequest(const QJsonObject& request,
                         const QStringList& requiredFields,
                         QJsonObject& response);

    DatabaseManager& m_db;
};

#endif
