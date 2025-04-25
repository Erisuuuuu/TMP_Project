#include "solution.h"
#include "ui_solution.h"

Solution::Solution(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Solution)
{
    ui->setupUi(this);
}

Solution::~Solution()
{
    delete ui;
}

// Слот для отображения решения
void Solution::displaySolution(const QString &solution)
{
    // Обновляем содержимое QTextBrowser
    ui->textBrowser->setPlainText(solution);  // предполагаем, что у нас есть QTextBrowser с именем textBrowserSolution
}

// Слот для показа окна решения
void Solution::slot_show()
{
    this->show();
}
