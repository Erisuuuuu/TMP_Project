#include "solver.h"

#include <QDebug>
#include "equationparser.h"

// QString Solver::solver() {
//     QString equation = "2x + 3y+52z + 2x= 5.0";
//     QString response = "";
//     // Для хранения коэффициентов переменных: ключ – символ переменной, значение – суммарный коэффициент.
//     QMap<QChar, double> varCoeffs;
//     // Для хранения свободного члена (правой части уравнения).
//     double constantTerm = 0.0;

//     if (EquationParser::parseLinearEquation(equation, varCoeffs, constantTerm)) {
//         response+= "Коэффициенты переменных:\n";
//         for (auto it = varCoeffs.constBegin(); it != varCoeffs.constEnd(); ++it) {
//             response+=  it.key() + ":" + it.value()+"\n";
//         }
//         response+= "Свободный член:" + constantTerm+"\n";
//     } else {
//         response+= "Ошибка при разборе уравнения!\n";
//     }

//     response;
// }
QString Solver::solver() {
    QString equation = "2x + 3y+52z + 2x= 5.0";

    QMap<QChar, double> varCoeffs;
    double constantTerm = 0.0;

    QString response; // Переменная для хранения результата

    if (EquationParser::parseLinearEquation(equation, varCoeffs, constantTerm)) {
        response += "Коэффициенты переменных:\r\n";
        for (auto it = varCoeffs.constBegin(); it != varCoeffs.constEnd(); ++it) {
            response += QString("%1 : %2\r\n").arg(it.key()).arg(it.value());
        }
        response += QString("Свободный член: %1\r\n").arg(constantTerm);
    } else {
        response = "Ошибка при разборе уравнения!\r\n";
    }

    qDebug() << response; // Для отладки также выводим в консоль
    return response;
}

