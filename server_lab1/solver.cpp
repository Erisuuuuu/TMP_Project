#include "solver.h"
#include <QDebug>
#include "equationparser.h"

QString Solver::solver(const QStringList &equations) {
    if (equations.size() > 5) {
        return "Ошибка: можно обработать не более 5 уравнений.\r\n";
    }

    QString response;
    int eqIndex = 1; // Индекс уравнения для наглядности

    for (const QString &equation : equations) {
        QMap<QChar, double> varCoeffs;
        double constantTerm = 0.0;

        response += QString("Уравнение %1: %2\r\n").arg(eqIndex++).arg(equation);

        if (EquationParser::parseLinearEquation(equation, varCoeffs, constantTerm)) {
            response += "Коэффициенты переменных:\r\n";
            for (auto it = varCoeffs.constBegin(); it != varCoeffs.constEnd(); ++it) {
                response += QString("%1 : %2\r\n").arg(it.key()).arg(it.value());
            }
            response += QString("Свободный член: %1\r\n\r\n").arg(constantTerm);
        } else {
            response += "Ошибка при разборе уравнения!\r\n\r\n";
        }
    }

    qDebug() << response; // Для отладки также выводим в консоль
    return response;
}
