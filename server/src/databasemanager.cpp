// Подключение заголовочного файла класса
#include "databasemanager.h"

// Необходимые заголовки Qt
#include <QDateTime>          // Для работы с датой/временем
#include <QSqlError>          // Для обработки ошибок SQL
#include <QCryptographicHash> // Для хеширования паролей

// Реализация синглтона (единственного экземпляра класса)
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance; // Создается при первом вызове
    return instance;                 // Гарантирует единственный экземпляр
}

// Деструктор
DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {  // Если соединение открыто
        m_db.close();     // Закрываем соединение с БД
    }
}

// Подключение к базе данных
bool DatabaseManager::connectToDatabase(const QString& path) {
    if (path.isEmpty()) {                 // Проверка пути
        m_lastError = "Database path is empty";
        return false;
    }

    // Создаем соединение с именем "SERVER_DB_CONN"
    m_db = QSqlDatabase::addDatabase("QSQLITE", "SERVER_DB_CONN");
    m_db.setDatabaseName(path);           // Устанавливаем путь к БД

    if (!m_db.open()) {                   // Пытаемся открыть БД
        m_lastError = m_db.lastError().text(); // Сохраняем ошибку
        return false;
    }

    // Проблема: код ниже никогда не выполнится из-за return true выше!
    // Исправление: нужно переместить этот код перед return true

    // Создание таблиц (не выполняется в текущей версии)
    QSqlQuery query(m_db);
    return query.exec(
               "CREATE TABLE IF NOT EXISTS users ("          // Таблица пользователей
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "      // Автоинкрементный ID
               "login TEXT UNIQUE NOT NULL, "                // Уникальный логин
               "password_hash TEXT NOT NULL, "               // Хеш пароля
               "email TEXT UNIQUE NOT NULL)"                 // Уникальный email
               ) && query.exec(
               "CREATE TABLE IF NOT EXISTS password_reset_codes (" // Таблица кодов сброса
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "user_id INTEGER NOT NULL, "                   // Ссылка на пользователя
               "code TEXT NOT NULL, "                         // Код для сброса
               "expires_at DATETIME NOT NULL, "               // Срок действия
               "FOREIGN KEY(user_id) REFERENCES users(id))"   // Внешний ключ
               );
}

// Регистрация пользователя
bool DatabaseManager::registerUser(const QString& login,
                                   const QString& password,
                                   const QString& email) {
    QMutexLocker locker(&m_mutex); // Блокировка мьютекса для потокобезопасности

    QSqlQuery query(m_db); // Создание SQL-запроса
    query.prepare(         // Подготовка запроса с плейсхолдерами
        "INSERT INTO users (login, password_hash, email) "
        "VALUES (?, ?, ?)" // Плейсхолдеры для защиты от SQL-инъекций
        );
    // Привязка значений
    query.addBindValue(login);
    query.addBindValue(hashPassword(password)); // Хеширование пароля
    query.addBindValue(email);

    if (!query.exec()) {                  // Выполнение запроса
        m_lastError = query.lastError().text(); // Сохранение ошибки
        return false;
    }
    return true; // Успешная регистрация
}

// Аутентификация пользователя
bool DatabaseManager::authenticateUser(const QString& login,
                                       const QString& password) {
    QMutexLocker locker(&m_mutex); // Блокировка мьютекса

    QSqlQuery query(m_db);
    query.prepare(                  // Запрос хеша пароля
        "SELECT password_hash FROM users "
        "WHERE login = ?"
        );
    query.addBindValue(login);

    if (!query.exec() || !query.next()) { // Выполнение и проверка результата
        m_lastError = "User not found";
        return false;
    }

    // Сравнение хешей
    return query.value(0).toString() == hashPassword(password);
}

// Получение email по логину или email
QString DatabaseManager::getEmailByLogin(const QString& loginOrEmail) {
    QMutexLocker locker(&m_mutex);

    QSqlQuery query(m_db);
    query.prepare(  // Поиск по логину или email
        "SELECT email FROM users "
        "WHERE login = ? OR email = ?"
        );
    query.addBindValue(loginOrEmail);
    query.addBindValue(loginOrEmail);

    if (query.exec() && query.next()) { // Если найден результат
        return query.value(0).toString();
    }
    return ""; // Пустая строка если не найден
}

// Создание кода сброса пароля
bool DatabaseManager::createPasswordResetCode(const QString& loginOrEmail, const QString& code) {
    QMutexLocker locker(&m_mutex);
    qDebug() << "\n=== Creating password reset code ==="; // Логирование

    if (!m_db.transaction()) { // Начало транзакции
        qCritical() << "Failed to start transaction:" << m_db.lastError().text();
        return false;
    }

    // Шаг 1: Поиск пользователя
    QSqlQuery userQuery(m_db);
    userQuery.prepare(
        "SELECT id FROM users "
        "WHERE login = ? OR email = ?"
        );
    userQuery.addBindValue(loginOrEmail.trimmed());
    userQuery.addBindValue(loginOrEmail.trimmed());

    if (!userQuery.exec()) {
        qCritical() << "User search error:" << userQuery.lastError().text();
        m_db.rollback(); // Откат транзакции
        return false;
    }

    if (!userQuery.next()) { // Если пользователь не найден
        qWarning() << "User not found:" << loginOrEmail;
        m_db.rollback();
        return false;
    }

    int userId = userQuery.value(0).toInt(); // Получение ID пользователя

    // Шаг 2: Удаление старых кодов
    QSqlQuery deleteQuery(m_db);
    deleteQuery.prepare("DELETE FROM password_reset_codes WHERE user_id = ?");
    deleteQuery.addBindValue(userId);
    if (!deleteQuery.exec()) {
        m_lastError = deleteQuery.lastError().text();
        return false;
    }

    // Шаг 3: Вставка нового кода
    QDateTime expiresAt = QDateTime::currentDateTime().addSecs(300).toUTC(); // Время жизни 5 минут
    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(
        "INSERT INTO password_reset_codes " // Опечатка в имени таблицы (должно быть password_reset_codes?)
        "(user_id, code, expires_at) "
        "VALUES (?, ?, ?)"
        );

    insertQuery.addBindValue(userId);
    insertQuery.addBindValue(code.trimmed());
    insertQuery.addBindValue(expiresAt.toString(Qt::ISODateWithMs)); // Формат времени ISO

    if (!insertQuery.exec()) {
        qCritical() << "Insert error:" << insertQuery.lastError().text();
        m_db.rollback();
        return false;
    }

    if (!m_db.commit()) { // Фиксация транзакции
        qCritical() << "Commit error:" << m_db.lastError().text();
        m_db.rollback();
        return false;
    }

    return true;
}

// Валидация кода сброса
bool DatabaseManager::validateResetCode(const QString& loginOrEmail, const QString& code) {
    QMutexLocker locker(&m_mutex);

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT COUNT(*) as cnt FROM password_reset_codes " // Подсчет валидных кодов
        "WHERE code = :code "
        "AND user_id IN (SELECT id FROM users WHERE login = :loginOrEmail OR email = :loginOrEmail) "
        "AND expires_at > datetime('now')" // Проверка срока действия
        );
    query.bindValue(":code", code.trimmed());
    query.bindValue(":loginOrEmail", loginOrEmail);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0; // Если есть совпадения
    }
    return false;
}

// Обновление пароля
bool DatabaseManager::updatePassword(const QString& loginOrEmail, const QString& newPassword) {
    QMutexLocker locker(&m_mutex);

    QString hashed = hashPassword(newPassword); // Хеширование нового пароля

    QSqlQuery query(m_db);
    query.prepare(  // Обновление хеша пароля
        "UPDATE users SET password_hash = ? "
        "WHERE login = ? OR email = ?"
        );
    query.addBindValue(hashed);
    query.addBindValue(loginOrEmail);
    query.addBindValue(loginOrEmail);

    if(!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0; // Проверка что пароль обновлен
}

// Проверка существования email
bool DatabaseManager::emailExists(const QString& email) {
    QMutexLocker locker(&m_mutex);

    QSqlQuery query(m_db);
    query.prepare(  // Подсчет записей с email
        "SELECT COUNT(*) FROM users "
        "WHERE email = ?"
        );
    query.addBindValue(email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0; // Если есть совпадения
    }
    return false;
}

// Хеширование пароля SHA-256
QString DatabaseManager::hashPassword(const QString& password) {
    return QCryptographicHash::hash(
               password.toUtf8(),          // Конвертация в UTF-8
               QCryptographicHash::Sha256  // Алгоритм SHA-256
               ).toHex();                   // Конвертация в hex-строку
}

// Получение последней ошибки
QString DatabaseManager::lastError() const {
    return m_lastError; // Возврат сохраненной ошибки
}

// Удаление кода сброса
bool DatabaseManager::deleteResetCode(const QString& loginOrEmail, const QString& code) {
    QMutexLocker locker(&m_mutex);

    QSqlQuery query(m_db);
    query.prepare(  // Удаление кода
        "DELETE FROM password_reset_codes "
        "WHERE code = ? AND user_id IN ("
        "SELECT id FROM users WHERE login = ? OR email = ?)"
        );
    query.addBindValue(code);
    query.addBindValue(loginOrEmail);
    query.addBindValue(loginOrEmail);

    if(!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true; // Не проверяем количество удаленных строк
}
