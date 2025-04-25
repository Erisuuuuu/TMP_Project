#include "equationparser.h"
#include <QMap>
#include <QRegularExpression>
#include <QDebug>

// Метод разбора системы линейных уравнений
bool EquationParser::parseLinearEquation(const QStringList &equations, QVector<QVector<double>>& matrix, QVector<double>& b) {
    // Очищаем матрицу коэффициентов и вектор свободных членов на случай повторного вызова
    matrix.clear();
    b.clear();

    // Обрабатываем каждое уравнение из входного списка
    for (const QString &equation : equations) {
        QMap<QChar, double> varCoeffs; // Ассоциативный массив: переменная -> её суммарный коэффициент
        double constantTerm = 0.0;    // Правая часть уравнения (свободный член)

        QString normalizedInput = equation;
        normalizedInput.remove(" ");    // Удаляем пробелы, чтобы было проще парсить

        // Разделяем уравнение на левую и правую часть по знаку "="
        QStringList sides = normalizedInput.split('=');
        if (sides.size() != 2) {
            qDebug() << "ERROR: Уравнение должно содержать ровно один знак '='.";
            return false; // Если знаков "=" больше или меньше, возвращаем ошибку
        }
        QString lhs = sides[0]; // Левая часть (с переменными)
        QString rhs = sides[1]; // Правая часть (свободный член)

        // Регулярное выражение для поиска слагаемых типа "3x", "-2.5*y", "+z" и т.д.
        QRegularExpression termRegex(R"(([+-]?)(\d*\.?\d*)\*?([A-Za-z]))");
        
        // Ищем все совпадения (слагаемые) в левой части уравнения
        QRegularExpressionMatchIterator iter = termRegex.globalMatch(lhs);

        bool foundTerm = false; // Флаг, нашли ли хотя бы одно слагаемое
        while (iter.hasNext()) {
            QRegularExpressionMatch match = iter.next();
            foundTerm = true;

            // Захватываем знак, числовую часть и саму переменную
            QString signStr = match.captured(1);    // + или -
            QString numStr = match.captured(2);    // Число перед переменной
            QString varStr = match.captured(3);    // Буква переменной (например, x)

            double coeff = 1.0; // Если число не указано (например, "x"), считаем коэффициент равным 1
            if (!numStr.isEmpty()) {
                coeff = numStr.toDouble(); // Преобразуем число в double
            }
            if (signStr == "-") {
                coeff = -coeff; // Учитываем знак
            }

            QChar var = varStr[0]; // Имя переменной — символ
            varCoeffs[var] += coeff; // Суммируем коэффициенты одинаковых переменных
        }

        if (!foundTerm) {
            qDebug() << "ERROR: Не найдено ни одного корректного слагаемого в левой части.";
            return false; // Если ни одно слагаемое не было найдено
        }

        // Обработка правой части: преобразуем в число
        bool ok = false;
        constantTerm = rhs.toDouble(&ok);
        if (!ok) {
            qDebug() << "ERROR: Правая часть уравнения не является корректным числом.";
            return false;
        }

        // Формируем строку для добавления в матрицу коэффициентов
        QVector<double> row;
        for (auto it = varCoeffs.constBegin(); it != varCoeffs.constEnd(); ++it) {
            row.append(it.value());  // Добавляем коэффициенты переменных в текущую строку матрицы
        }
        
        matrix.append(row);  // Добавляем строку в матрицу коэффициентов
        b.append(constantTerm);  // Добавляем свободный член в вектор правой части
    }
    // Если дошли до сюда — значит, все уравнения успешно разобраны
    return true;
}
