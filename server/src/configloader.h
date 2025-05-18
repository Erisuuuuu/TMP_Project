#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QString>
#include <QJsonObject>

class ConfigLoader {
public:
    ConfigLoader(const QString& configPath);

    bool load();
    QString getString(const QString& key) const;
    bool getBool(const QString& key) const;
    bool contains(const QString& key) const;
    QJsonValue get(const QString& key) const;
    int getInt(const QString& key, int defaultValue = 0) const;

private:
    QString m_configPath;
    QJsonObject m_config;
};

#endif // CONFIGLOADER_H
