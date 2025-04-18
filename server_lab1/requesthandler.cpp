#include "requesthandler.h"
#include "solver.h"
RequestHandler::RequestHandler() {}

QString RequestHandler::handleRequest(const QString& request) {
    QStringList parts = request.split(' ', Qt::SkipEmptyParts);

    if (parts.size() >= 3 && parts[0] == "REGISTER") {
        return handleRegister(parts); // Обработка регистрации
    }
    else if (parts.size() >= 3 && parts[0] == "LOGIN") {
        return handleLogin(parts); // Обработка авторизации
    }
    else if (parts.size() >= 2 && parts[0] == "SOLVE") {
        return handleSolve(request); // Обработка решения уравнений
    }
    else {
        return "Invalid command\r\n"; // Неверная команда
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

// QString RequestHandler::handleSolve(const QString& expression) {
//     return Solver::solver()+"\r\n";
// }
QString RequestHandler::handleSolve(const QString& expression) {
    // int boolka = Solver::solver();//debug
    // QString debug;
    // debug = Solver::solver();
    // return debug;
    return Solver::solver();
}
