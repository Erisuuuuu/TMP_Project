#ifndef EQUATIONPARSER_H
#define EQUATIONPARSER_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

class EquationParser {
public:
    static bool parseSystem(
        const QStringList& equations,
        QVector<QVector<double>>& matrix,
        QVector<double>& constants,
        QStringList& variables,
        QString& error
        );
};

#endif // EQUATIONPARSER_H
