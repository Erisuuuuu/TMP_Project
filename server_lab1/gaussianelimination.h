#ifndef GAUSSIANELIMINATION_H
#define GAUSSIANELIMINATION_H

#include <QVector>

// Класс GaussianElimination реализует метод Гаусса для решения систем линейных уравнений.
// Метод solve принимает на вход матрицу коэффициентов и вектор свободных членов и возвращает вектор решений системы.
class GaussianElimination {
public:
    static QVector<double> solve(const QVector<QVector<double>>& matrix, const QVector<double>& b);
    // Статический метод для решения системы линейных уравнений
    // matrix — матрица коэффициентов (NxN)
    // b — вектор свободных членов (Nx1)
    // Возвращает: QVector<double> — вектор решений (x, y, z, ...)
};

#endif // GAUSSIANELIMINATION_H
