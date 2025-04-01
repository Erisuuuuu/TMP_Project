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

void Solution::slot_show()
{
    this->show();
}
