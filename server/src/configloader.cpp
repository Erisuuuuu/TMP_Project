// Подключаем заголовочный файл нашего класса ConfigLoader
#include "ConfigLoader.h"

// Подключаем необходимые Qt-классы
#include <QFile>        // Для работы с файлами
#include <QJsonDocument> // Для работы с JSON-документами
#include <QDebug>       // Для вывода отладочной информации

// Конструктор класса, принимает путь к конфигурационному файлу
ConfigLoader::ConfigLoader(const QString& configPath)
    : m_configPath(configPath) {} // Инициализируем член класса m_configPath переданным значением

// Метод для загрузки и парсинга конфигурационного файла
bool ConfigLoader::load() {
    QFile configFile(m_configPath); // Создаем объект файла по указанному пути

    // Проверка существования файла
    if (!configFile.exists()) {
        qCritical() << "Config file not found:" << m_configPath; // Выводим критическую ошибку
        return false;
    }

    // Пытаемся открыть файл только для чтения
    if (!configFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open config file:" << configFile.errorString(); // Ошибка открытия
        return false;
    }

    // Читаем все содержимое файла в QByteArray
    QByteArray data = configFile.readAll();
    qDebug() << "Config content:\n" << data; // Выводим содержимое для отладки

    // Парсим JSON-данные
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error); // Преобразуем данные в JSON-документ

    // Проверка ошибок парсинга
    if (error.error != QJsonParseError::NoError) {
        qCritical() << "JSON parse error at offset" << error.offset
                    << ":" << error.errorString(); // Выводим позицию и описание ошибки
        return false;
    }

    m_config = doc.object(); // Сохраняем корневой JSON-объект
    return true; // Успешное завершение
}

// Метод для получения произвольного JSON-значения по ключу
QJsonValue ConfigLoader::get(const QString& key) const {
    return m_config.value(key); // Используем встроенный метод QJsonObject
}

// Метод для получения строки из конфига (поддержка вложенных ключей через '/')
QString ConfigLoader::getString(const QString& key) const {
    QStringList keys = key.split('/'); // Разбиваем ключ на части по разделителю
    QJsonObject current = m_config;    // Начинаем с корневого объекта

    // Проходим по всем частям ключа, кроме последней
    for(int i = 0; i < keys.size() - 1; ++i) {
        if(!current.contains(keys[i])) return QString(); // Если ключ отсутствует - возвращаем пустую строку
        current = current[keys[i]].toObject(); // Переходим во вложенный объект
    }

    return current.value(keys.last()).toString(); // Возвращаем значение последнего ключа как строку
}

// Метод для получения целого числа с значением по умолчанию
int ConfigLoader::getInt(const QString& key, int defaultValue) const {
    QStringList keys = key.split('/');
    QJsonObject current = m_config;

    for(int i = 0; i < keys.size() - 1; ++i) {
        if(!current.contains(keys[i])) return defaultValue; // Если ключ отсутствует - возвращаем дефолт
        current = current[keys[i]].toObject();
    }

    // Преобразуем значение в int, используя defaultValue при ошибке
    return current.value(keys.last()).toInt(defaultValue);
}

// Метод для получения логического значения
bool ConfigLoader::getBool(const QString& key) const {
    return m_config.value(key).toBool(); // Прямое преобразование в bool
}
