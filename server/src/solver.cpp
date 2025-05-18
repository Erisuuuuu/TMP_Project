#include "solver.h"
#include "equationparser.h"
#include "gaussianelimination.h"
#include <QJsonArray>


QJsonObject Solver::solveEquations(const QStringList& equations) {
    QJsonObject response;

    QVector<QVector<double>> matrix;
    QVector<double> constants;
    QString parseError;

    // Логирование входящих уравнений
    qDebug() << "Парсим уравнения:";
    for (const QString& eq : equations) {
        qDebug() << eq;
    }

    // Парсим уравнения
    QStringList variables;
    if (!EquationParser::parseSystem(equations, matrix, constants, variables, parseError)) {
        response["status"] = "error";
        //response["message"] = "Ошибка разбора уравнений: " + parseError;
        response["message"] = parseError.isEmpty()
                                  ? "Не удалось разобрать систему уравнений."
                                  : "Ошибка разбора уравнений: " + parseError;

        return response;
    }

    // Проверка на русские символы в названиях переменных
    QRegularExpression nonLatinRegex("[^a-zA-Z0-9]");
    for (const QString& var : variables) {
        if (nonLatinRegex.match(var).hasMatch()) {
            response["status"] = "error";
            response["message"] = "Использование нелатинских символов в названиях переменных недопустимо: " + var;
            return response;
        }
    }


    qDebug() << "Парсинг прошел успешно, матрица и константы:" << matrix << constants;


    QVector<double> solution;
    QStringList steps;
    QString solveError;

    // Решаем методом Гаусса
    if (!GaussianElimination::solve(matrix, constants, solution, steps, solveError, variables)) {
        qDebug() << "Ошибка при решении:" << solveError;
        response["status"] = "error";
        //response["message"] = "Ошибка при решении: " + solveError;
        response["message"] = solveError.isEmpty()
                                  ? "Не удалось решить систему уравнений."
                                  : "Ошибка при решении: " + solveError;

        return response;
    }

    qDebug() << "Решение найдено:" << solution;

    // Успех: формируем JSON-ответ
    response["status"] = "success";

    QJsonArray solutionArray;
    for (int i = 0; i < solution.size(); ++i) {
        QJsonObject var;
        var["variable"] = variables.value(i, QString("x%1").arg(i + 1)); // используем имя переменной, если есть
        var["value"] = solution[i];
        solutionArray.append(var);
    }
    response["solution"] = solutionArray;

    QJsonArray stepsArray;
    for (const QString& step : steps) {
        stepsArray.append(step);
    }
    response["steps"] = stepsArray;

    return response;

}
