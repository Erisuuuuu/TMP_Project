#include "gaussianelimination.h"
#include <QDebug>

// Метод Гаусса для решения системы линейных уравнений
// matrix — исходная матрица коэффициентов
// b — вектор свободных членов
// Возвращает вектор решений
QVector<double> GaussianElimination::solve(const QVector<QVector<double>>& matrix, const QVector<double>& b) {
    int n = matrix.size(); // Определяем размерность системы (кол-во уравнений)
  
    QVector<QVector<double>> augmentedMatrix = matrix;  // Копируем матрицу для преобразований
    QVector<double> solution(n); // Вектор для хранения решения

    // Добавляем в каждый ряд матрицы соответствующий свободный член (получаем расширенную матрицу)
    for (int i = 0; i < n; ++i) {
        augmentedMatrix[i].append(b[i]);
    }

    // Прямой ход метода Гаусса (обнуляем элементы под главной диагональю)
    for (int i = 0; i < n; ++i) {
        // Поиск строки с максимальным элементом в текущем столбце (частичный выбор главного элемента)
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (qAbs(augmentedMatrix[k][i]) > qAbs(augmentedMatrix[maxRow][i])) {
                maxRow = k;
            }
        }

        // Меняем текущую строку и строку с максимальным элементом
        augmentedMatrix[i].swap(augmentedMatrix[maxRow]);

        // Преобразуем строки ниже текущей, чтобы обнулить элементы под диагональю
        for (int j = i + 1; j < n; ++j) {
            double ratio = augmentedMatrix[j][i] / augmentedMatrix[i][i]; // коэффициент домножения
            for (int k = i; k <= n; ++k) {
                augmentedMatrix[j][k] -= ratio * augmentedMatrix[i][k]; // вычитание строки
            }
        }
    }

    // Обратный ход метода Гаусса (находим значения переменных начиная с последней)
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = augmentedMatrix[i][n];  // начинаем с правой части
        for (int j = i + 1; j < n; ++j) {
            solution[i] -= augmentedMatrix[i][j] * solution[j]; // учитываем уже найденные переменные
        }
        solution[i] /= augmentedMatrix[i][i]; // делим на коэффициент перед переменной
    }

    return solution; // возвращаем вектор решений
}
