#include "requesthandler.h"
#include <QRandomGenerator>  // Для генерации случайных чисел (кодов подтверждения)
#include <QDebug>           // Для отладочного вывода

// Основной метод обработки входящих JSON-запросов
QJsonObject RequestHandler::handleJsonRequest(const QJsonObject& request) {
    // Извлекаем действие из запроса и приводим к нижнему регистру
    QString action = request["action"].toString().toLower();

    // Роутинг запросов в зависимости от типа действия
    if (action == "register") {
        return processRegister(request);  // Обработка регистрации
    } else if (action == "login") {
        return processLogin(request);     // Обработка входа
    } else if (action == "password_reset_request") {
        return processPasswordResetRequest(request);  // Запрос сброса пароля
    } else if (action == "password_reset_confirm") {
        return processPasswordResetConfirm(request);  // Подтверждение сброса
    } else if (action == "check_email") {
        return processCheckEmail(request);  // Проверка email
    }

    // Если действие не распознано, возвращаем ошибку
    QJsonObject response;
    response["status"] = "error";
    response["message"] = "Неизвестное действие";
    return response;
}

// Обработка проверки существования email
QJsonObject RequestHandler::processCheckEmail(const QJsonObject& request) {
    QJsonObject response;

    // Проверяем наличие обязательного поля email
    if (!request.contains("email")) {
        response["status"] = "error";
        response["message"] = "Missing email field";
        return response;
    }

    // Извлекаем email из запроса
    QString email = request["email"].toString();
    // Проверяем существование email в базе данных
    bool exists = m_db.emailExists(email);

    // Формируем ответ
    response["status"] = "success";
    response["exists"] = exists;  // true - если email существует, false - если нет
    return response;
}

// Обработка регистрации нового пользователя
QJsonObject RequestHandler::processRegister(const QJsonObject& request) {
    QJsonObject response;

    // Проверка наличия обязательных полей
    if (!request.contains("login") || !request.contains("password") || !request.contains("email")) {
        response["status"] = "error";
        response["message"] = "Missing required fields";
        return response;
    }

    // Извлекаем данные из запроса
    QString login = request["login"].toString();
    QString password = request["password"].toString();
    QString email = request["email"].toString();

    // Логируем попытку регистрации
    qDebug() << "Register request:" << login << email;

    // Пытаемся зарегистрировать пользователя
    if (m_db.registerUser(login, password, email)) {
        response["status"] = "success";
        response["message"] = "User registered successfully";
    } else {
        // Если регистрация не удалась, возвращаем ошибку из БД
        response["status"] = "error";
        response["message"] = m_db.lastError();
    }

    return response;
}

// Обработка входа пользователя
QJsonObject RequestHandler::processLogin(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login", "password"};  // Обязательные поля

    // Валидация запроса
    if (!validateRequest(request, required, response)) {
        return response;  // Возвращаем ошибку, если валидация не пройдена
    }

    // Извлекаем и очищаем данные
    QString login = request["login"].toString().trimmed();
    QString password = request["password"].toString().trimmed();

    // Аутентификация пользователя
    if (m_db.authenticateUser(login, password)) {
        response["status"] = "success";
        // Добавляем email в ответ при успешной аутентификации
        response["email"] = m_db.getEmailByLogin(login);
    } else {
        response["status"] = "error";
        response["message"] = "Неверные учетные данные";
    }
    return response;
}

// Обработка запроса на сброс пароля
QJsonObject RequestHandler::processPasswordResetRequest(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login_or_email"};  // Обязательное поле

    // Валидация запроса
    if (!validateRequest(request, required, response)) {
        return response;
    }

    // Извлекаем и очищаем данные
    QString loginOrEmail = request["login_or_email"].toString().trimmed();
    // Генерируем 6-значный код подтверждения
    QString code = QString::number(QRandomGenerator::global()->bounded(100000, 999999));

    // Создаем код сброса в базе данных
    if (m_db.createPasswordResetCode(loginOrEmail, code)) {
        response["status"] = "success";
        response["message"] = "Код отправлен на почту";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError();  // Ошибка из БД
    }
    return response;
}

// Обработка подтверждения сброса пароля
QJsonObject RequestHandler::processPasswordResetConfirm(const QJsonObject& request) {
    QJsonObject response;
    // Обязательные поля для этого запроса
    QStringList required = {"login_or_email", "code", "new_password"};

    // Валидация запроса
    if (!validateRequest(request, required, response)) {
        return response;
    }

    // Извлекаем и очищаем данные
    QString loginOrEmail = request["login_or_email"].toString().trimmed();
    QString code = request["code"].toString().trimmed();
    QString newPassword = request["new_password"].toString().trimmed();

    // Проверяем код сброса
    if (!m_db.validateResetCode(loginOrEmail, code)) {
        response["status"] = "error";
        response["message"] = "Неверный код или время действия истекло";
        return response;
    }

    // Обновляем пароль
    if (m_db.updatePassword(loginOrEmail, newPassword)) {
        response["status"] = "success";
        response["message"] = "Пароль успешно изменен";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError();  // Ошибка из БД
    }
    return response;
}

// Метод валидации обязательных полей в запросе
bool RequestHandler::validateRequest(const QJsonObject& request,
                                     const QStringList& requiredFields,
                                     QJsonObject& response) {
    // Проверяем каждое обязательное поле
    for (const QString& field : requiredFields) {
        if (!request.contains(field)) {
            // Если поле отсутствует, формируем ошибку
            response["status"] = "error";
            response["message"] = "Отсутствует обязательное поле: " + field;
            return false;
        }
    }
    return true;  // Все обязательные поля присутствуют
}
