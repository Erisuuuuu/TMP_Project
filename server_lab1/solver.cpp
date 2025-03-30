#include "solver.h"

#include <QDebug>
#include "equationparser.h"

int Solver::solver() {
    QString equation = "2x + 3y+52z + 2x= 5.0";

    // Для хранения коэффициентов переменных: ключ – символ переменной, значение – суммарный коэффициент.
    QMap<QChar, double> varCoeffs;
    // Для хранения свободного члена (правой части уравнения).
    double constantTerm = 0.0;

    if (EquationParser::parseLinearEquation(equation, varCoeffs, constantTerm)) {
        qDebug() << "Коэффициенты переменных:";
        for (auto it = varCoeffs.constBegin(); it != varCoeffs.constEnd(); ++it) {
            qDebug() << it.key() << ":" << it.value();
        }
        qDebug() << "Свободный член:" << constantTerm;
    } else {
        qDebug() << "Ошибка при разборе уравнения!";
    }

    return 0;
}
