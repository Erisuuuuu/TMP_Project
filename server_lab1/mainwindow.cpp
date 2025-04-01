#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Изначально отображаем только две строки ввода
    ui->labelUrav1->setVisible(true);
    ui->lineEditUrav1->setVisible(true);
    ui->labelUrav2->setVisible(true);
    ui->lineEditUrav2->setVisible(true);

    // Скрываем остальные строки ввода (изначально)
    ui->labelUrav3->setVisible(false);
    ui->labelUrav4->setVisible(false);
    ui->labelUrav5->setVisible(false);
    ui->lineEditUrav3->setVisible(false);
    ui->lineEditUrav4->setVisible(false);
    ui->lineEditUrav5->setVisible(false);

    // Устанавливаем начальное количество строк ввода
    uravCount = 2;



}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::slot_show(QString login)
{
    this->show();
}

void MainWindow::on_pushButtonAddUrav_clicked()
{
    // Проверяем, не превысили ли мы максимальное количество строк
    if (uravCount < 5) {
        // Отображаем следующую строку ввода в зависимости от текущего состояния uravCount
        if (uravCount == 2) {
            ui->labelUrav3->setVisible(true);
            ui->lineEditUrav3->setVisible(true);
        } else if (uravCount == 3) {
            ui->labelUrav4->setVisible(true);
            ui->lineEditUrav4->setVisible(true);
        } else if (uravCount == 4) {
            ui->labelUrav5->setVisible(true);
            ui->lineEditUrav5->setVisible(true);
        }

        // Увеличиваем счетчик после отображения строки
        uravCount++;
    } else {
        // Если достигли максимума, можно вывести сообщение об ошибке или просто ничего не делать.
        QMessageBox::warning(this, "Предупреждение", "Достигнуто максимальное количество уравнений.");
    }
}



void MainWindow::on_pushButtonSolveUrav_clicked()
{
    QString methodText = ui->lineEditMethod->text().trimmed(); // Получаем текст из поля ввода
    bool ok;
    int methodNumber = methodText.toInt(&ok);

    if (!ok || methodNumber < 1 || methodNumber > 5)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректный номер метода (от 1 до 5)!");
        return; // Прекращаем выполнение функции, если ввод некорректен
    }

    emit solution_ok(); // Если всё корректно, отправляем сигнал
}
