// Подключаем необходимые заголовочные файлы
#include "mainwindow.h"       // Заголовок текущего класса
#include "ui_mainwindow.h"    // Автогенерируемый UI-класс из .ui файла
#include <QMessageBox>        // Для показа сообщений пользователю
#include <QJsonObject>        // Работа с JSON-объектами
#include <QJsonDocument>      // Сериализация/десериализация JSON
#include "networkclient.h"    // Для сетевых запросов

// Конструктор главного окна
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)     // Инициализация базового класса
    , ui(new Ui::MainWindow)  // Создание UI-интерфейса
    , uravCount(2)            // Инициализация счетчика уравнений (стартовое значение)
{
    ui->setupUi(this); // Настройка UI из файла mainwindow.ui

    // Скрываем дополнительные поля уравнений (изначально видны только 2)
    ui->labelUrav3->setVisible(false);
    ui->labelUrav4->setVisible(false);
    ui->labelUrav5->setVisible(false);
    ui->lineEditUrav3->setVisible(false);
    ui->lineEditUrav4->setVisible(false);
    ui->lineEditUrav5->setVisible(false);
}

// Деструктор
MainWindow::~MainWindow()
{
    delete ui; // Освобождаем память от UI
}

// Слот для отображения окна (вызывается после успешной авторизации)
void MainWindow::slot_show(QString login)
{
    Q_UNUSED(login); // Явно указываем, что параметр не используется (чтобы избежать warning)
    this->show();    // Показываем главное окно
}

// Обработчик клика по кнопке добавления уравнения
void MainWindow::on_pushButtonAddUrav_clicked()
{
    // Проверяем, не достигли ли максимума (5 уравнений)
    if (uravCount >= 5) {
        QMessageBox::warning(this, "Достигнут максимум",
                             "Можно добавить не более 5 уравнений");
        return;
    }

    // Включаем видимость следующего поля ввода в зависимости от текущего количества
    switch(uravCount) {
    case 2: // Показываем третье уравнение
        ui->labelUrav3->setVisible(true);
        ui->lineEditUrav3->setVisible(true);
        break;
    case 3: // Показываем четвертое уравнение
        ui->labelUrav4->setVisible(true);
        ui->lineEditUrav4->setVisible(true);
        break;
    case 4: // Показываем пятое уравнение
        ui->labelUrav5->setVisible(true);
        ui->lineEditUrav5->setVisible(true);
        break;
    }

    uravCount++; // Увеличиваем счетчик уравнений
}

// Обработчик кнопки решения системы уравнений
void MainWindow::on_pushButtonSolveUrav_clicked()
{
    // Получаем и обрабатываем номер метода
    QString method = ui->lineEditMethod->text().trimmed(); // Удаляем пробелы
    bool ok;
    int methodNumber = method.toInt(&ok); // Пробуем преобразовать в число

    // Проверяем корректность ввода (должно быть число 1-5)
    if (!ok || methodNumber < 1 || methodNumber > 5) {
        QMessageBox::warning(this, "Ошибка ввода",
                             "Введите число от 1 до 5");
        return;
    }

    // Формируем JSON-объект с данными для сервера
    QJsonObject equations;
    equations["method"] = methodNumber; // Номер метода решения
    equations["eq1"] = ui->lineEditUrav1->text(); // Уравнение 1
    equations["eq2"] = ui->lineEditUrav2->text(); // Уравнение 2

    // Добавляем остальные уравнения, если они включены
    if (uravCount >= 3) equations["eq3"] = ui->lineEditUrav3->text();
    if (uravCount >= 4) equations["eq4"] = ui->lineEditUrav4->text();
    if (uravCount >= 5) equations["eq5"] = ui->lineEditUrav5->text();

    // Отправляем запрос через NetworkClient
    NetworkClient::instance().sendSolutionRequest(
        equations, // Передаем JSON с уравнениями
        [this](bool success, const QString& result) { // Лямбда-обработчик ответа
            if (success) {
                QMessageBox::information(this, "Результат", result); // Показываем результат
            } else {
                QMessageBox::critical(this, "Ошибка", result); // Показываем ошибку
            }
        }
        );
}
