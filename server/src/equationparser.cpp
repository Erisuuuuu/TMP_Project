#include "equationparser.h"
#include <QRegularExpression>
#include <QSet>
#include <QDebug>
#include <QMap>
#include <algorithm>

namespace {

// Проверка на наличие недопустимых символов (например, русских букв)
bool containsInvalidCharacters(const QStringList& equations) {
    QRegularExpression invalidCharRegex("[^a-zA-Z0-9=+\\-*/().\\s]");
    for (const QString& eq : equations) {
        if (invalidCharRegex.match(eq).hasMatch()) {
            return true;
        }
    }
    return false;
}


// Вспомогательная функция для разбора одного уравнения
bool parseEquation(
    const QString& equation,
    const QStringList& variables,
    QVector<double>& coefficients,
    double& constant,
    QString& error
    ) {
    // Проверка количества знаков равенства
    QStringList parts = equation.split('=');
    if (parts.size() != 2) {
        error = "Уравнение должно содержать ровно один знак равенства: " + equation;
        return false;
    }

    QString left = parts[0].trimmed();
    QString right = parts[1].trimmed();

    if (left.isEmpty() || right.isEmpty()) {
        error = "Уравнение содержит пустую левую или правую часть: " + equation;
        return false;
    }

    //QRegularExpression termRegex("([+-]?\\s*\\d*\\.?\\d*)\\s*([a-zA-Z]+)");
    QRegularExpression termRegex("([+-]?\\s*\\d*\\.?\\d*)\\s*([a-zA-Z][a-zA-Z0-9]*)");

    QMap<QString, double> varMap;

    auto matches = termRegex.globalMatch(left);
    bool foundVariable = false;
    while (matches.hasNext()) {
        auto match = matches.next();
        foundVariable = true;

        QString coeffStr = match.captured(1).remove(' ').trimmed();
        QString var = match.captured(2).trimmed();

        double coeff = 1.0;
        if (!coeffStr.isEmpty() && coeffStr != "+" && coeffStr != "-") {
            bool ok = false;
            coeff = coeffStr.toDouble(&ok);
            if (!ok) {
                error = "Некорректный коэффициент в уравнении: " + coeffStr;
                return false;
            }
        } else if (coeffStr == "-") {
            coeff = -1.0;
        }

        varMap[var] += coeff;
    }

    if (!foundVariable) {
        error = "В уравнении не найдены переменные: " + equation;
        return false;
    }

    // Инициализировать вектор коэффициентов
    coefficients.resize(variables.size());
    for (int i = 0; i < variables.size(); ++i) {
        coefficients[i] = varMap.value(variables[i], 0.0);
    }

    // Разбор правой части (константы)
    bool ok = false;
    constant = right.toDouble(&ok);
    if (!ok) {
        error = "Правая часть уравнения не является числом: " + right;
        return false;
    }

    return true;
}

} // namespace

// Основной метод разбора всей системы
bool EquationParser::parseSystem(
    const QStringList& equations,
    QVector<QVector<double>>& matrix,
    QVector<double>& constants,
    QStringList& variables,
    QString& error
    ) {

    // Проверка на недопустимые символы
    if (containsInvalidCharacters(equations)) {
        error = "Ошибка: используйте только латинские буквы и допустимые математические символы.";
        return false;
    }

    QSet<QString> varSet;
    //QRegularExpression varRegex("([a-zA-Z]+)");
    QRegularExpression varRegex("([a-zA-Z][a-zA-Z0-9]*)");


    // Сначала соберём все переменные
    for (const QString& eq : equations) {
        auto matches = varRegex.globalMatch(eq);
        while (matches.hasNext()) {
            QString var = matches.next().captured(1);
            varSet.insert(var);
        }
    }

    if (varSet.isEmpty()) {
        error = "Не удалось определить переменные в системе.";
        return false;
    }

    // Упорядочим переменные по алфавиту
    variables = QStringList(varSet.begin(), varSet.end());
    std::sort(variables.begin(), variables.end());

    // Разбор каждого уравнения
    matrix.clear();
    constants.clear();

    for (const QString& eq : equations) {
        QVector<double> row;
        double b = 0.0;

        if (!parseEquation(eq, variables, row, b, error)) {
            return false;
        }

        matrix.append(row);
        constants.append(b);
    }

    if (matrix.size() != constants.size()) {
        error = "Внутренняя ошибка: количество строк в матрице не совпадает с количеством констант.";
        return false;
    }

    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[i].size() != variables.size()) {
            error = "Внутренняя ошибка: некорректное число коэффициентов в уравнении.";
            return false;
        }
    }

    return true;
}



