#include "solver.h"  // Заголовочный файл класса Solver

#include <QDebug>  // Для вывода отладочной информации в консоль
#include "equationparser.h"  // Модуль для разбора строк уравнений в матрицу
#include "gaussianelimination.h"  // Метод Гаусса для решения

// Основной метод solver() — демонстрационная функция для тестирования
QString Solver::solver() {
    // Пример системы линейных уравнений, заданной в виде строк
    QStringList equations = {
        "2x + 3y + 52z + 2x = 5.0",  // Первое уравнение (обратите внимание: 2x встречается дважды)
        "3x + y - z = 4.0"           // Второе уравнение
    };

    QVector<QVector<double>> matrix; // Матрица коэффициентов
    QVector<double> b;               // Вектор свободных членов

    QString response; // Строка-ответ, которая будет возвращена и выведена

    // Парсинг входных строк в матрицу коэффициентов и вектор b
    if (EquationParser::parseLinearEquation(equations, matrix, b)) {
        // Если парсинг успешен, выводим коэффициенты
        response += "Коэффициенты переменных:\r\n";
        for (const QVector<double>& row : matrix) {
            for (double coeff : row) {
                response += QString("%1 ").arg(coeff);
            }
            response += "\r\n";
        }

        // Выводим вектор свободных членов
        response += "Свободные члены:\r\n";
        for (double val : b) {
            response += QString("%1 ").arg(val);
        }
        response += "\r\n";

        // Решение системы методом Гаусса
        QVector<double> solution = GaussianElimination::solve(matrix, b);

        // Выводим найденные значения переменных
        response += "Решение системы:\r\n";
        for (double val : solution) {
            response += QString("%1 ").arg(val);
        }
    } else {
        // Если произошла ошибка при парсинге — сообщаем об этом
        response = "Ошибка при разборе системы уравнений!\r\n";
    }

    qDebug() << response; // Отладочный вывод в консоль (удобно при тестировании)
    return response;      // Возвращаем итоговый результат как строку
}
