// Заголовочные файлы
#include "smtpclient.h"          // Клиент для отправки email
#include "configloader.h"        // Загрузчик конфигураций
#include <QObject>               // Базовый класс Qt
#include "requesthandler.h"      // Заголовок текущего класса
#include "solver.h"              // Добавим в начало файла, если еще не подключен
#include <QJsonDocument>         // Работа с JSON
#include <QJsonObject>           // JSON-объекты
#include <QJsonArray>            // JSON-массивы
#include <QRandomGenerator>      // Генерация случайных чисел
#include <QDebug>                // Отладочный вывод
#include <QSqlError>             // Ошибки SQL

// Конструктор с инициализацией ссылки на менеджер БД
RequestHandler::RequestHandler(DatabaseManager& db, QObject* parent)
    : QObject(parent),   // Инициализация родительского класса
    m_db(db)           // Сохраняем ссылку на менеджер БД
{
}

// Основной обработчик JSON-запросов
QJsonObject RequestHandler::handleJsonRequest(const QJsonObject& request) {
    QString action = request["action"].toString().toLower(); // Извлекаем действие

    // Роутинг запросов
    if(action == "register") return processRegister(request);
    else if(action == "login") return processLogin(request);
    else if(action == "solve") return processSolveEquations(request); // Обработка уравнений
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

// Обработчик текстовых команд (legacy-интерфейс)
QString RequestHandler::handleTextRequest(const QString& request) {
    QString trimmed = request.trimmed(); // Удаляем пробелы
    QStringList parts = trimmed.split(' ', Qt::SkipEmptyParts); // Разбиваем на токены

    // Определение типа команды
    if(parts.size() >= 3 && parts[0] == "REGISTER") return handleTextRegister(parts);
    else if(parts.size() >= 3 && parts[0] == "LOGIN") return handleTextLogin(parts);
    else if(trimmed.startsWith("SOLVE ")) return handleSolve(trimmed.mid(6).trimmed());

    return "Invalid command\r\n"; // Ответ по умолчанию
}

// Регистрация пользователя (JSON)
QJsonObject RequestHandler::processRegister(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login", "password", "email"}; // Обязательные поля

    // Проверка наличия полей
    if(!validateRequest(request, required, response)) {
        return response;
    }

    // Извлечение и нормализация данных
    QString login = request["login"].toString().trimmed();
    QString password = request["password"].toString().trimmed();
    QString email = request["email"].toString().trimmed();

    // Регистрация через менеджер БД
    if(m_db.registerUser(login, password, email)) {
        response["status"] = "success";
        response["message"] = "User registered";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError(); // Сообщение об ошибке из БД
    }
    return response;
}

// Аутентификация пользователя (JSON)
QJsonObject RequestHandler::processLogin(const QJsonObject& request) {
    QJsonObject response;
    QString login = request["login"].toString();
    QString password = request["password"].toString();

    // Проверка учетных данных
    if(m_db.authenticateUser(login, password)) {
        response["status"] = "success";
        response["message"] = "auth_ok";
        response["email"] = m_db.getEmailByLogin(login); // Добавляем email в ответ
    } else {
        response["status"] = "error";
        response["message"] = "auth_failed";
    }
    return response;
}

// Запрос сброса пароля (JSON)
QJsonObject RequestHandler::processPasswordResetRequest(const QJsonObject& request) {
    QJsonObject response;

    // Проверка наличия обязательного поля
    if (!request.contains("login_or_email")) {
        response["status"] = "error";
        response["message"] = "Поле 'login_or_email' обязательно";
        return response;
    }

    QString loginOrEmail = request["login_or_email"].toString().trimmed();
    QString email = m_db.getEmailByLogin(loginOrEmail); // Поиск email

    if (email.isEmpty()) { // Проверка существования пользователя
        response["status"] = "error";
        response["message"] = "Пользователь не найден";
        return response;
    }

    // Генерация 6-значного кода
    QString code = QString::number(QRandomGenerator::global()->bounded(100000, 999999));

    // Сохранение кода в БД
    if (!m_db.createPasswordResetCode(loginOrEmail, code)) {
        response["status"] = "error";
        response["message"] = "Ошибка сохранения кода";
        return response;
    }

    // Загрузка конфигурации SMTP
    ConfigLoader config("config.json");
    if (!config.load()) {
        response["status"] = "error";
        response["message"] = "Ошибка загрузки конфигурации";
        return response;
    }

    // Настройка SMTP-клиента
    SMTPClient *smtp = new SMTPClient(this);
    smtp->configure(
        config.getString("smtp/host"),
        config.getInt("smtp/port"),
        config.getString("smtp/user"),
        config.getString("smtp/password"),
        config.getBool("smtp/ssl")
        );

    // Формирование письма
    QString subject = "Код восстановления пароля";
    QString body = QString("Ваш код: %1").arg(code);

    // Обработчик результата отправки
    connect(smtp, &SMTPClient::finished, [=](bool success, const QString& error) {
        if (!success) { // В случае ошибки
            qCritical() << "[SMTP] Ошибка отправки:" << error;
            m_db.deleteResetCode(loginOrEmail, code); // Удаление неиспользуемого кода
        }
        smtp->deleteLater(); // Освобождение памяти
    });

    smtp->sendEmail(email, subject, body); // Отправка

    response["status"] = "success";
    response["message"] = "Код отправлен на почту";
    return response;
}

// Проверка кода сброса пароля (JSON)
QJsonObject RequestHandler::processPasswordResetConfirm(const QJsonObject& request) {
    QJsonObject response;
    QStringList requiredFields = {"login_or_email", "code"};

    // Валидация входных данных
    if (!validateRequest(request, requiredFields, response)) {
        return response;
    }

    QString loginOrEmail = request["login_or_email"].toString().trimmed();
    QString code = request["code"].toString().trimmed();

    // Проверка кода в БД
    bool isValid = m_db.validateResetCode(loginOrEmail, code);

    if (isValid) {
        response["status"] = "success";
        response["message"] = "Код подтвержден";
        m_db.deleteResetCode(loginOrEmail, code); // Удаление использованного кода
    } else {
        response["status"] = "error";
        response["message"] = "Неверный код или время действия истекло";
    }
    return response;
}

// Обновление пароля (JSON)
QJsonObject RequestHandler::processPasswordResetUpdate(const QJsonObject& request) {
    QJsonObject response;
    QStringList required = {"login_or_email", "new_password"};

    if(!validateRequest(request, required, response)) {
        return response;
    }

    QString loginOrEmail = request["login_or_email"].toString().trimmed();
    QString newPassword = request["new_password"].toString().trimmed();

    // Проверка существования пользователя
    QString email = m_db.getEmailByLogin(loginOrEmail);
    if(email.isEmpty()) {
        response["status"] = "error";
        response["message"] = "User not found";
        return response;
    }

    // Обновление пароля в БД
    if(m_db.updatePassword(loginOrEmail, newPassword)) {
        response["status"] = "success";
        response["message"] = "Password updated successfully";
    } else {
        response["status"] = "error";
        response["message"] = m_db.lastError();
    }
    return response;
}

// Проверка существования email (JSON)
QJsonObject RequestHandler::processCheckEmail(const QJsonObject& request) {
    QJsonObject response;
    if(!request.contains("email")) {
        response["status"] = "error";
        response["message"] = "Missing email field";
        return response;
    }

    bool exists = m_db.emailExists(request["email"].toString());
    response["status"] = "success";
    response["exists"] = exists;
    return response;
}

// Регистрация через текстовый интерфейс
QString RequestHandler::handleTextRegister(const QStringList& parts) {
    if(parts.size() < 3) return "Invalid format: REGISTER username password\r\n";

    // Конвертация в JSON-формат
    QJsonObject request;
    request["action"] = "register";
    request["login"] = parts[1];
    request["password"] = parts[2];
    request["email"] = parts.size() > 3 ? parts[3] : "";

    QJsonObject response = processRegister(request);
    return QJsonDocument(response).toJson() + "\r\n";
}

// Аутентификация через текстовый интерфейс
QString RequestHandler::handleTextLogin(const QStringList& parts) {
    if(parts.size() < 3) return "Invalid format: LOGIN username password\r\n";

    QJsonObject request;
    request["action"] = "login";
    request["login"] = parts[1];
    request["password"] = parts[2];

    QJsonObject response = processLogin(request);
    return QJsonDocument(response).toJson() + "\r\n";
}

// Валидатор обязательных полей
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
    return true;
}


// Решение уравнений через текстовый интерфейс
QString RequestHandler::handleSolve(const QString& rawInput) {
    // Разделяем уравнения по точке с запятой
    QStringList equations = rawInput.split(';', Qt::SkipEmptyParts);
    for (QString& eq : equations) {
        eq = eq.trimmed();
    }

    // Вызываем общий решатель
    QJsonObject response = Solver::solveEquations(equations);

    if (response["status"] != "success") {
        return "Ошибка: " + response["message"].toString() + "\r\n";
    }

    QString result = "Решение:\r\n";
    QJsonArray solution = response["solution"].toArray();
    for (const QJsonValue& val : solution) {
        QJsonObject var = val.toObject();
        result += QString("%1 = %2\r\n")
                      .arg(var["variable"].toString())
                      .arg(var["value"].toDouble());
    }

    QJsonArray steps = response["steps"].toArray();
    result += "\r\nШаги:\r\n";
    for (int i = 0; i < steps.size(); ++i) {
        result += QString("%1. %2\r\n").arg(i + 1).arg(steps[i].toString());
    }

    return result;
}


QJsonObject RequestHandler::processSolveEquations(const QJsonObject& request) {
    QJsonObject response;

    if (!request.contains("equations") || !request["equations"].isArray()) {
        response["status"] = "error";
        response["message"] = "Некорректный формат запроса: отсутствует список уравнений.";
        return response;
    }

    QJsonArray eqArray = request["equations"].toArray();
    if (eqArray.isEmpty() || eqArray.size() > 5) {
        response["status"] = "error";
        response["message"] = "Количество уравнений должно быть от 1 до 5.";
        return response;
    }

    QStringList equations;
    for (const QJsonValue& val : eqArray) {
        if (!val.isString()) {
            response["status"] = "error";
            response["message"] = "Уравнения должны быть строками.";
            return response;
        }
        equations << val.toString();
    }

    // Логирование входящих данных
    qDebug() << "Полученные уравнения:" << equations;

    qDebug() << "Ответ от сервера: " << QJsonDocument(response).toJson(); // Логирование ответа

    // Вызываем решение уравнений
    return Solver::solveEquations(equations);
}
