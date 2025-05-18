#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QJsonObject>
#include <QString>
#include "databasemanager.h"
#include <QObject>

class RequestHandler : public QObject {
    Q_OBJECT
public:
    // Конструктор с параметром parent для поддержки сигналов и слотов
    explicit RequestHandler(DatabaseManager& db, QObject* parent = nullptr);

    // Методы для обработки запросов
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
    QJsonObject processSolveEquations(const QJsonObject& request);  // Обработка запроса на решение уравнений

    // Обработчики текстовых команд
    QString handleTextRegister(const QStringList& parts);
    QString handleTextLogin(const QStringList& parts);
    QString handleSolve(const QString& expression);

    // Вспомогательные методы
    bool validateRequest(const QJsonObject& request,
                         const QStringList& requiredFields,
                         QJsonObject& response);

    DatabaseManager& m_db;  // Ссылка на объект базы данных
};

#endif // REQUESTHANDLER_H
