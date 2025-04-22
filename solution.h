#ifndef SOLUTION_H
#define SOLUTION_H

#include <QMainWindow>

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
    void slot_show();

private:
    Ui::Solution *ui;
};

#endif // SOLUTION_H
