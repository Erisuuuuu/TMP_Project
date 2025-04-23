#include "requesthandler.h"         // Заголовочный файл класса RequestHandler
#include "equationparser.h"         // Подключаем парсер уравнений
#include "gaussianelimination.h"     // Подключаем реализацию метода Гаусса
//#include "solver.h"

RequestHandler::RequestHandler() {}

// Основной метод обработки входящего текстового запроса
QString RequestHandler::handleRequest(const QString& request) {
    QString trimmedRequest = request.trimmed();  // Удаляем лишние пробелы в начале и конце строки

    // Разбиваем строку на части по пробелам
    QStringList parts = trimmedRequest.split(' ', Qt::SkipEmptyParts);

    if (parts.size() >= 3 && parts[0] == "REGISTER") {
        return handleRegister(parts); // Обработка регистрации
    }
    else if (parts.size() >= 3 && parts[0] == "LOGIN") {
        return handleLogin(parts); // Обработка авторизации
    }
    // Проверка на команду решения уравнений: SOLVE <уравнения>
    else if (trimmedRequest.startsWith("SOLVE ")) {
        // Отрезаем "SOLVE " и передаем оставшееся выражение
        QString expression = trimmedRequest.mid(6).trimmed();
        return handleSolve(expression);
    }
    // Неизвестная команда — возвращаем сообщение об ошибке   
    else {
        return "Invalid command\r\n";
    }
}

QString RequestHandler::handleRegister(const QStringList& parts) {
    QString username = parts[1];
    QString password = parts[2];
    return "User registered (stub): " + username + "\r\n";
}

QString RequestHandler::handleLogin(const QStringList& parts) {
    QString username = parts[1];
    QString password = parts[2];
    return "User logged in (stub): " + username + "\r\n";
}

    // Метод для обработки команды решения уравнений
QString RequestHandler::handleSolve(const QString& expression) {
    // Разделяем строку выражения на отдельные уравнения (по символу ';')
    QStringList equations = expression.split(';', Qt::SkipEmptyParts);

    QVector<QVector<double>> matrix;  // Матрица коэффициентов
    QVector<double> b;                // Вектор свободных членов

    // Пытаемся распарсить уравнения в матрицу и вектор
    bool success = EquationParser::parseLinearEquation(equations, matrix, b);
    if (!success) {
        return "Ошибка при разборе уравнений!\r\n";
    }

    // Решаем систему уравнений методом Гаусса
    QVector<double> solution = GaussianElimination::solve(matrix, b);

    // Формируем строку-ответ с решением
    QString response = "Решение системы:\r\n";
    QStringList variableNames = {"x", "y", "z", "w", "v"}; // Имена переменных на случай до 5 переменных

    // Подставляем полученные значения в ответ
    for (int i = 0; i < solution.size(); ++i) {
        QString varName = (i < variableNames.size()) ? variableNames[i] : QString("x%1").arg(i+1);
        response += QString("%1 = %2\r\n").arg(varName).arg(solution[i]);
    }

    return response;  // Возвращаем итоговое решение
}
