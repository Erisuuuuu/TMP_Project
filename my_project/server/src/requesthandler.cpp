// Включение необходимых заголовочных файлов
#include "requesthandler.h"       // Заголовок текущего класса
#include "equationparser.h"       // Парсер уравнений
#include "gaussianelimination.h"  // Решение СЛАУ методом Гаусса
#include <QJsonDocument>          // Работа с JSON документами
#include <QJsonObject>            // JSON объекты
#include <QJsonArray>             // JSON массивы
#include <QRandomGenerator>       // Генерация случайных чисел
#include <QDebug>                 // Вывод отладочной информации

// Конструктор, принимает ссылку на менеджер БД
RequestHandler::RequestHandler(DatabaseManager& db)
    : m_db(db)  // Инициализация ссылки на БД
{
}

// Обработчик JSON запросов
QJsonObject RequestHandler::handleJsonRequest(const QJsonObject& request) {
    // Извлечение действия из запроса (в нижнем регистре)
    QString action = request["action"].toString().toLower();

    // Роутинг запросов по действиям
    if(action == "register") return processRegister(request);
    else if(action == "login") return processLogin(request);
    else if(action == "password_reset_request") return processPasswordResetRequest(request);
    else if(action == "password_reset_confirm") return processPasswordResetConfirm(request);
    else if(action == "password_reset_update") return processPasswordResetUpdate(request);
    else if(action == "check_email") return processCheckEmail(request);

    // Ответ для неизвестных действий
    QJsonObject response;
    response["status"] = "error";
    response["message"] = "Unknown action";
    return response;
}

// Обработчик текстовых запросов
QString RequestHandler::handleTextRequest(const QString& request) {
    // Предварительная обработка запроса
    QString trimmed = request.trimmed();                 // Удаление пробелов по краям
    QStringList parts = trimmed.split(' ', Qt::SkipEmptyParts); // Разбивка на части

    // Определение типа команды
    if(parts.size() >= 3 && parts[0] == "REGISTER") return handleTextRegister(parts);
    else if(parts.size() >= 3 && parts[0] == "LOGIN") return handleTextLogin(parts);
    else if(trimmed.startsWith("SOLVE ")) return handleSolve(trimmed.mid(6).trimmed());

    return "Invalid command\r\n"; // Ответ по умолчанию
}

// JSON: Регистрация пользователя
QJsonObject RequestHandler::processRegister(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login", "password", "email"}; // Обязательные поля

    // Валидация наличия полей
    if(!validateRequest(request, required, response)) {
        return response;
    }

    // Извлечение данных из запроса
    QString login = request["login"].toString().trimmed();
    QString password = request["password"].toString().trimmed();
    QString email = request["email"].toString().trimmed();

    // Попытка регистрации через БД
    if(m_db.registerUser(login, password, email)) {
        response["status"] = "success";
        response["message"] = "User registered";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError(); // Получение ошибки из БД
    }
    return response;
}

// JSON: Аутентификация пользователя
QJsonObject RequestHandler::processLogin(const QJsonObject& request) {
    QJsonObject response;
    // Извлечение учетных данных
    QString login = request["login"].toString();
    QString password = request["password"].toString();

    // Проверка учетных данных в БД
    if(m_db.authenticateUser(login, password)) {
        response["status"] = "success";
        response["message"] = "auth_ok";  // Специфичный код успеха
        response["email"] = m_db.getEmailByLogin(login); // Добавление почты
    } else {
        response["status"] = "error";
        response["message"] = "auth_failed"; // Код ошибки
    }
    return response;
}

// JSON: Запрос сброса пароля
QJsonObject RequestHandler::processPasswordResetRequest(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login_or_email"};

    if(!validateRequest(request, required, response)) {
        return response;
    }

    // Генерация 6-значного кода
    QString code = QString::number(QRandomGenerator::global()->bounded(100000, 999999));
    QString loginOrEmail = request["login_or_email"].toString().trimmed();

    // Сохранение кода в БД
    if(m_db.createPasswordResetCode(loginOrEmail, code)) {
        response["status"] = "success";
        response["message"] = "Reset code sent";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError();
    }
    return response;
}

// JSON: Подтверждение кода сброса
QJsonObject RequestHandler::processPasswordResetConfirm(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login_or_email", "code"};

    if(!validateRequest(request, required, response)) {
        return response;
    }

    // Валидация кода через БД
    if(m_db.validateResetCode(
            request["login_or_email"].toString().trimmed(),
            request["code"].toString().trimmed()
            )) {
        response["status"] = "success";
    } else {
        response["status"] = "error";
        response["message"] = "Invalid code";
    }
    return response;
}

// JSON: Обновление пароля
QJsonObject RequestHandler::processPasswordResetUpdate(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login_or_email", "new_password"};

    if(!validateRequest(request, required, response)) {
        return response;
    }

    // Обновление пароля в БД
    if(m_db.updatePassword(
            request["login_or_email"].toString().trimmed(),
            request["new_password"].toString().trimmed()
            )) {
        response["status"] = "success";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError();
    }
    return response;
}

// JSON: Проверка существования почты
QJsonObject RequestHandler::processCheckEmail(const QJsonObject& request) {
    QJsonObject response;
    if(!request.contains("email")) {
        response["status"] = "error";
        response["message"] = "Missing email field";
        return response;
    }

    // Проверка через БД
    bool exists = m_db.emailExists(request["email"].toString());
    response["status"] = "success";
    response["exists"] = exists;
    return response;
}

// Текстовая команда: Регистрация
QString RequestHandler::handleTextRegister(const QStringList& parts) {
    if(parts.size() < 3) return "Invalid format: REGISTER username password\r\n";

    // Формирование JSON запроса из текстовых данных
    QJsonObject request;
    request["action"] = "register";
    request["login"] = parts[1];
    request["password"] = parts[2];
    request["email"] = parts.size() > 3 ? parts[3] : "";

    // Использование JSON обработчика
    QJsonObject response = processRegister(request);
    return QJsonDocument(response).toJson() + "\r\n"; // Конвертация в строку
}

// Текстовая команда: Аутентификация
QString RequestHandler::handleTextLogin(const QStringList& parts) {
    if(parts.size() < 3) return "Invalid format: LOGIN username password\r\n";

    QJsonObject request;
    request["action"] = "login";
    request["login"] = parts[1];
    request["password"] = parts[2];

    QJsonObject response = processLogin(request);
    return QJsonDocument(response).toJson() + "\r\n";
}

// Текстовая команда: Решение уравнений
QString RequestHandler::handleSolve(const QString& expression) {
    QVector<QVector<double>> matrix; // Матрица коэффициентов
    QVector<double> b;               // Вектор свободных членов

    // Парсинг уравнений
    if(!EquationParser::parseLinearEquation(
            expression.split(';', Qt::SkipEmptyParts), // Разделение уравнений
            matrix,
            b
            )) {
        return "Error parsing equations\r\n";
    }

    // Решение СЛАУ
    QVector<double> solution = GaussianElimination::solve(matrix, b);
    if(solution.isEmpty()) return "No solution found\r\n";

    // Форматирование ответа
    QString response = "Solution:\r\n";
    QStringList vars = {"x", "y", "z", "w", "v"}; // Имена переменных
    for(int i = 0; i < solution.size(); ++i) {
        QString var = (i < vars.size()) ? vars[i] : QString("x%1").arg(i+1);
        response += QString("%1 = %2\r\n").arg(var).arg(solution[i], 0, 'f', 2);
    }
    return response;
}

// Валидация обязательных полей в JSON запросе
bool RequestHandler::validateRequest(
    const QJsonObject& request,
    const QStringList& requiredFields,
    QJsonObject& response
    ) {
    for(const QString& field : requiredFields) {
        if(!request.contains(field)) {
            response["status"] = "error";
            response["message"] = "Missing field: " + field;
            return false;
        }
    }
    return true; // Все обязательные поля присутствуют
}
