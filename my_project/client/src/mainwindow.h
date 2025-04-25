#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

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

private slots:
    void on_pushButtonAddUrav_clicked();
    void on_pushButtonSolveUrav_clicked();

signals:
    void solution_ok();

private:
    Ui::MainWindow *ui;
    int uravCount;
};

#endif // MAINWINDOW_H
