#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMutex>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDebug>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    // Получение экземпляра синглтона
    static DatabaseManager& instance();

    // Основные методы работы с БД
    bool connectToDatabase(const QString& path);          // Подключение к БД
    bool registerUser(const QString& login,               // Регистрация пользователя
                      const QString& password,
                      const QString& email);
    bool authenticateUser(const QString& login,           // Аутентификация
                          const QString& password);
    QString getEmailByLogin(const QString& login);        // Получение email по логину
    bool createPasswordResetCode(const QString& loginOrEmail, // Генерация кода сброса
                                 const QString& code);
    bool validateResetCode(const QString& loginOrEmail,   // Проверка кода
                           const QString& code);
    bool updatePassword(const QString& loginOrEmail,      // Обновление пароля
                        const QString& newPassword);
    bool emailExists(const QString& email);
    QString lastError() const;                            // Получение последней ошибки

    // Запрет копирования
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    DatabaseManager() = default;  // Приватный конструктор
    ~DatabaseManager();
    QString hashPassword(const QString& password);  // Хеширование пароля

    QSqlDatabase m_db;         // Объект базы данных
    mutable QMutex m_mutex;    // Мьютекс для потокобезопасности
    QString m_lastError;       // Последнее сообщение об ошибке
};
#endif
