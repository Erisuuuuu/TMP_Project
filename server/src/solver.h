#ifndef SOLVER_H
#define SOLVER_H

#include <QStringList>
#include <QJsonObject>

class Solver {
public:
    static QJsonObject solveEquations(const QStringList& equations);
};

#endif // SOLVER_H
