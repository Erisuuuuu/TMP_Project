#ifndef REQUESTHANDLER_H  // Защита от повторного включения заголовка
#define REQUESTHANDLER_H  // Уникальный идентификатор для этого файла

#include <QJsonObject>    // Для работы с JSON-объектами
#include "databasemanager.h"  // Наш класс для работы с базой данных

class RequestHandler {
public:
    // Конструктор по умолчанию (явно указанный)
    RequestHandler() = default;

    // Основной публичный метод для обработки входящих JSON-запросов
    // Принимает: const QJsonObject& - JSON-запрос
    // Возвращает: QJsonObject - JSON-ответ
    QJsonObject handleJsonRequest(const QJsonObject& request);

private:
    // Приватные методы для обработки конкретных типов запросов:

    // Обработка регистрации пользователя
    QJsonObject processRegister(const QJsonObject& request);

    // Обработка входа пользователя
    QJsonObject processLogin(const QJsonObject& request);

    // Запрос на сброс пароля
    QJsonObject processPasswordResetRequest(const QJsonObject& request);

    // Подтверждение сброса пароля
    QJsonObject processPasswordResetConfirm(const QJsonObject& request);

    // Проверка существования email
    QJsonObject processCheckEmail(const QJsonObject& request);

    // Метод валидации входящих запросов
    // Принимает:
    // - request: входящий JSON-запрос
    // - requiredFields: список обязательных полей
    // - response: ссылка на объект ответа (для заполнения ошибок)
    // Возвращает: bool - true если валидация пройдена
    bool validateRequest(const QJsonObject& request,
                         const QStringList& requiredFields,
                         QJsonObject& response);

    // Ссылка на единственный экземпляр DatabaseManager
    // Использует паттерн Singleton через instance()
    DatabaseManager& m_db = DatabaseManager::instance();
};

#endif // Конец защиты от повторного включения
