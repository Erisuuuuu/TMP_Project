#include "equationparser.h"
#include <QRegularExpression>
#include <QDebug>

bool EquationParser::parseLinearEquation(const QString &input, QMap<QChar, double>& varCoeffs, double &constantTerm) {
    // Удаляем пробелы для упрощения обработки
    QString normalizedInput = input;
    normalizedInput.remove(" ");

    // Разбиваем строку по символу '='
    QStringList sides = normalizedInput.split('=');
    if (sides.size() != 2) {
        qDebug() << "ERROR: Уравнение должно содержать ровно один знак '='.";
        return false;
    }
    QString lhs = sides[0];
    QString rhs = sides[1];

    // Проверяем, что левая часть полностью соответствует допустимому формату:
    // Формат: [+-]?(\d*\.?\d+)?\*?[A-Za-z] ([+-](\d*\.?\d+)?\*?[A-Za-z])*
    // То есть одна или более правильных слагаемых без посторонних символов.
    QRegularExpression fullRegex(R"(^[+-]?(\d*\.?\d+)?\*?[A-Za-z](?:[+-](\d*\.?\d+)?\*?[A-Za-z])*$)");
    QRegularExpressionMatch fullMatch = fullRegex.match(lhs);
    if (!fullMatch.hasMatch() || fullMatch.captured(0) != lhs) {
        qDebug() << "ERROR: Левая часть уравнения содержит недопустимые символы или формат.";
        return false;
    }

    // Теперь извлекаем отдельные слагаемые с помощью регулярного выражения.
    QRegularExpression termRegex(R"(([+-]?)(\d*\.?\d*)\*?([A-Za-z]))");
    QRegularExpressionMatchIterator iter = termRegex.globalMatch(lhs);

    varCoeffs.clear();
    bool foundTerm = false;
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        foundTerm = true;
        QString signStr = match.captured(1);
        QString numStr = match.captured(2);
        QString varStr = match.captured(3);

        // Если число не задано, предполагаем коэффициент равный 1
        double coeff = 1.0;
        if (!numStr.isEmpty())
            coeff = numStr.toDouble();

        // Обработка знака перед слагаемым
        if (signStr == "-")
            coeff = -coeff;

        QChar var = varStr[0];
        varCoeffs[var] += coeff;
    }

    if (!foundTerm) {
        qDebug() << "ERROR: Не найдено ни одного корректного слагаемого в левой части.";
        return false;
    }

    // Разбор правой части уравнения как свободного члена
    bool ok = false;
    constantTerm = rhs.toDouble(&ok);
    if (!ok) {
        qDebug() << "ERROR: Правая часть уравнения не является корректным числом.";
        return false;
    }

    return true;
}
