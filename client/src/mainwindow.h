#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "solution.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void slot_show(QString);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void on_pushButtonAddUrav_clicked();
    void on_pushButtonSolveUrav_clicked();

signals:
    void solution_ok();

private:
    //void displaySolution(const QJsonArray& solution);
    void displaySolution(const QJsonObject& response);
    Ui::MainWindow *ui;
    int uravCount;
    void setFocusToFirstEmptyField();
    Solution* solutionWindow = nullptr; // окно для отображения решения
};



#endif // MAINWINDOW_H
