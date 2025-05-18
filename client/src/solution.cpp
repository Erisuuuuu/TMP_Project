#include "solution.h"
#include "ui_solution.h"

Solution::Solution(QWidget *parent) :
    QMainWindow(parent),  // Наследуем от QMainWindow
    ui(new Ui::Solution)
{
    ui->setupUi(this);  // Настроить интерфейс
    setWindowTitle("Решение системы уравнений");

}

Solution::~Solution()
{
    delete ui;
}

void Solution::setSolutionText(const QString& text)
{
    ui->textBrowserSolution->setText(text);  // Передаем решение в QTextBrowser

}
