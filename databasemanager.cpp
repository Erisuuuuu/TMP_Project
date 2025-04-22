#include "databasemanager.h"
#include <QCryptographicHash>  // Для хеширования паролей
#include <QDateTime>          // Для работы с временем
#include <QDebug>             // Для отладочного вывода
#include <QSqlError>          // Для обработки ошибок SQL
#include <QFileInfo>          // Для проверки файла БД

// Реализация паттерна Singleton
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;  // Создается один раз при первом вызове
    return instance;                  // Все последующие вызовы получают тот же экземпляр
}

// Деструктор - закрывает соединение с БД
DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();  // Закрываем соединение при уничтожении
    }
}

// Подключение к базе данных SQLite
bool DatabaseManager::connectToDatabase(const QString& path) {
    qDebug() << "Connecting to database at:" << path;

    // Если соединение уже открыто, возвращаем true
    if (m_db.isOpen()) return true;

    // Настраиваем подключение к SQLite
    m_db = QSqlDatabase::addDatabase("QSQLITE", "SERVER_DB_CONN");  // Уникальное имя соединения
    m_db.setDatabaseName(path);  // Указываем путь к файлу БД

    // Пытаемся открыть соединение
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();  // Сохраняем текст ошибки
        qCritical() << "Database error:" << m_lastError;
        return false;
    }

    // Создаем таблицы, если они не существуют
    QSqlQuery query(m_db);
    QStringList createTables = {
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "login TEXT UNIQUE NOT NULL, "
        "password_hash TEXT NOT NULL, "
        "email TEXT UNIQUE NOT NULL)",

        "CREATE TABLE IF NOT EXISTS password_reset_codes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "code TEXT NOT NULL, "
        "expires_at DATETIME NOT NULL, "
        "FOREIGN KEY(user_id) REFERENCES users(id))"
    };

    // Выполняем все SQL-запросы на создание таблиц
    for (const QString& sql : createTables) {
        if (!query.exec(sql)) {
            m_lastError = query.lastError().text();
            return false;
        }
    }

    qDebug() << "Database connected successfully";
    return true;
}

// Хеширование пароля с SHA-256
QString DatabaseManager::hashPassword(const QString& password) {
    return QCryptographicHash::hash(
               password.toUtf8(),          // Преобразуем пароль в UTF-8
               QCryptographicHash::Sha256  // Алгоритм SHA-256
               ).toHex();                 // Результат в виде hex-строки
}

// Регистрация нового пользователя
bool DatabaseManager::registerUser(const QString& login,
                                   const QString& password,
                                   const QString& email) {
    QMutexLocker locker(&m_mutex);  // Блокировка для потокобезопасности

    // Подготавливаем SQL-запрос
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (login, password_hash, email) VALUES (?, ?, ?)");
    query.addBindValue(login);               // Логин
    query.addBindValue(hashPassword(password));  // Хеш пароля
    query.addBindValue(email);               // Email

    // Выполняем запрос и возвращаем результат
    return query.exec();
}

// Аутентификация пользователя
bool DatabaseManager::authenticateUser(const QString& login, const QString& password) {
    QMutexLocker locker(&m_mutex);  // Блокировка

    // Ищем пользователя по логину
    QSqlQuery query(m_db);
    query.prepare("SELECT password_hash FROM users WHERE login = ?");
    query.addBindValue(login);

    // Если пользователь не найден или ошибка запроса
    if (!query.exec() || !query.next()) {
        m_lastError = "Пользователь не найден";
        return false;
    }

    // Сравниваем хеши паролей
    QString storedHash = query.value(0).toString();
    return (storedHash == hashPassword(password));
}

// Получение email по логину
QString DatabaseManager::getEmailByLogin(const QString& login) {
    QMutexLocker locker(&m_mutex);

    QSqlQuery query(m_db);
    query.prepare("SELECT email FROM users WHERE login = ?");
    query.addBindValue(login);

    // Если запрос выполнен успешно и есть результаты
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";  // Возвращаем пустую строку, если не найдено
}

// Создание кода для сброса пароля
bool DatabaseManager::createPasswordResetCode(const QString& loginOrEmail, const QString& code) {
    QMutexLocker locker(&m_mutex);

    // Ищем пользователя по логину или email
    QSqlQuery findUser(m_db);
    findUser.prepare("SELECT id FROM users WHERE login = ? OR email = ?");
    findUser.addBindValue(loginOrEmail);
    findUser.addBindValue(loginOrEmail);

    if (!findUser.exec() || !findUser.next()) {
        m_lastError = "Пользователь не найден";
        return false;
    }

    // Генерируем код с временем жизни 5 минут
    int userId = findUser.value(0).toInt();
    QDateTime expiresAt = QDateTime::currentDateTime().addSecs(300);

    // Сохраняем код в БД
    QSqlQuery insertCode(m_db);
    insertCode.prepare("INSERT INTO password_reset_codes (user_id, code, expires_at) VALUES (?, ?, ?)");
    insertCode.addBindValue(userId);
    insertCode.addBindValue(code);
    insertCode.addBindValue(expiresAt.toString(Qt::ISODate));

    return insertCode.exec();
}

// Проверка кода сброса пароля
bool DatabaseManager::validateResetCode(const QString& loginOrEmail, const QString& code) {
    QMutexLocker locker(&m_mutex);

    // Проверяем код и его срок действия
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT COUNT(*) FROM password_reset_codes "
        "JOIN users ON users.id = password_reset_codes.user_id "
        "WHERE (users.login = ? OR users.email = ?) "
        "AND code = ? AND expires_at > datetime('now')"
        );
    query.addBindValue(loginOrEmail);
    query.addBindValue(loginOrEmail);
    query.addBindValue(code);

    // Если код найден и действителен
    if (query.exec() && query.next()) {
        return (query.value(0).toInt()) > 0;
    }
    return false;
}

// Обновление пароля пользователя
bool DatabaseManager::updatePassword(const QString& loginOrEmail, const QString& newPassword) {
    QMutexLocker locker(&m_mutex);

    // Обновляем хеш пароля
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE users SET password_hash = ? "
        "WHERE login = ? OR email = ?"
        );
    query.addBindValue(hashPassword(newPassword));  // Хешируем новый пароль
    query.addBindValue(loginOrEmail);
    query.addBindValue(loginOrEmail);

    return query.exec();
}

// Проверка существования email
bool DatabaseManager::emailExists(const QString& email) {
    QMutexLocker locker(&m_mutex);
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE email = ?");
    query.addBindValue(email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;  // Возвращаем true, если email существует
    }
    return false;
}

// Получение последней ошибки
QString DatabaseManager::lastError() const {
    return m_lastError;
}
