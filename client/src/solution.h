#ifndef SOLUTION_H
#define SOLUTION_H

#include <QMainWindow>  // Используем QMainWindow вместо QDialog

namespace Ui {
class Solution;
}

class Solution : public QMainWindow  // Наследуем от QMainWindow
{
    Q_OBJECT

public:
    explicit Solution(QWidget *parent = nullptr);
    ~Solution();

    void setSolutionText(const QString& text);  // Метод для установки текста решения


private:
    Ui::Solution *ui;
};

#endif // SOLUTION_H
