#ifndef SOLUTION_H
#define SOLUTION_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class Solution;
}

class Solution : public QMainWindow
{
    Q_OBJECT

public:
    explicit Solution(QWidget *parent = nullptr);
    ~Solution();

public slots:
    // Слот для отображения решения
    void displaySolution(const QString& solution);

    // Слот для показа окна решения
    void slot_show();

private:
    Ui::Solution *ui;
};

#endif // SOLUTION_H
