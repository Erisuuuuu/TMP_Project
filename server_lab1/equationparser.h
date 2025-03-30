#ifndef EQUATIONPARSER_H
#define EQUATIONPARSER_H

#include <QString>
#include <QMap>

// Класс EquationParser содержит статическую функцию для разбора линейного уравнения,
// представленного строкой. Функция заполняет:
//  - QMap, где ключ – символ переменной, а значение – суммарный коэффициент.
//  - double для свободного члена (правая часть уравнения).
class EquationParser {
public:
    static bool parseLinearEquation(const QString &input, QMap<QChar, double>& varCoeffs, double &constantTerm);
};

#endif // EQUATIONPARSER_H
