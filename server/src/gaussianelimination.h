#ifndef GAUSSIANELIMINATION_H
#define GAUSSIANELIMINATION_H

#include <QStringList>
#include <QVector>

class GaussianElimination {
public:
    static bool solve(
        QVector<QVector<double>>& matrix,
        QVector<double>& constants,
        QVector<double>& solution,
        QStringList& steps,
        QString& error,
        const QStringList& variables
        );
};

#endif // GAUSSIANELIMINATION_H
