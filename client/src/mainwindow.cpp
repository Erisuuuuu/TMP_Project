// Подключаем необходимые заголовочные файлы
#include "mainwindow.h"       // Заголовок текущего класса
#include "ui_mainwindow.h"    // Автогенерируемый UI-класс из .ui файла
#include <QMessageBox>        // Для показа сообщений пользователю
#include <QJsonObject>        // Работа с JSON-объектами
#include <QJsonDocument>      // Сериализация/десериализация JSON
#include "networkclient.h"
#include <QJsonArray>    // Для сетевых запросов
#include <QKeyEvent>

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

    // Устанавливаем фокус на первое пустое поле при открытии окна
    setFocusToFirstEmptyField();
}

// Метод для установки фокуса на первое пустое поле
void MainWindow::setFocusToFirstEmptyField() {
    if (ui->lineEditUrav1->text().isEmpty()) {
        ui->lineEditUrav1->setFocus();
    } else if (ui->lineEditUrav2->text().isEmpty()) {
        ui->lineEditUrav2->setFocus();
    } else if (uravCount >= 3 && ui->lineEditUrav3->text().isEmpty()) {
        ui->lineEditUrav3->setFocus();
    } else if (uravCount >= 4 && ui->lineEditUrav4->text().isEmpty()) {
        ui->lineEditUrav4->setFocus();
    } else if (uravCount >= 5 && ui->lineEditUrav5->text().isEmpty()) {
        ui->lineEditUrav5->setFocus();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QList<QLineEdit*> fields = {
            ui->lineEditUrav1,
            ui->lineEditUrav2,
            ui->lineEditUrav3,
            ui->lineEditUrav4,
            ui->lineEditUrav5
        };

        for (int i = 0; i < fields.size(); ++i) {
            if (fields[i]->hasFocus()) {
                // Ищем следующее видимое поле
                for (int j = i + 1; j < fields.size(); ++j) {
                    if (fields[j]->isVisible()) {
                        fields[j]->setFocus();
                        return;
                    }
                }
                // Если нет следующего видимого поля — показать предупреждение
                QMessageBox::information(this, "Добавление уравнения",
                                         "Нажмите кнопку \"Добавить уравнение\", чтобы ввести следующее.");
                return;
            }
        }
    }

    QMainWindow::keyPressEvent(event);  // передаём остальные события базовому классу
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

    // Массив всех полей уравнений
    QLineEdit* uravEdits[] = {
        ui->lineEditUrav1,
        ui->lineEditUrav2,
        ui->lineEditUrav3,
        ui->lineEditUrav4,
        ui->lineEditUrav5
    };

    // Проверяем все предыдущие поля — если есть пустое, ставим туда фокус и выходим
    for (int i = 0; i < uravCount; ++i) {
        if (uravEdits[i]->text().trimmed().isEmpty()) {
            uravEdits[i]->setFocus();
            QMessageBox::warning(this, "Пустое поле",
                                 "Сначала заполните предыдущее уравнение.");
            return;
        }
    }

    // Включаем следующее поле и даем фокус
    switch(uravCount) {
    case 2:
        ui->labelUrav3->setVisible(true);
        ui->lineEditUrav3->setVisible(true);
        ui->lineEditUrav3->setFocus();
        break;
    case 3:
        ui->labelUrav4->setVisible(true);
        ui->lineEditUrav4->setVisible(true);
        ui->lineEditUrav4->setFocus();
        break;
    case 4:
        ui->labelUrav5->setVisible(true);
        ui->lineEditUrav5->setVisible(true);
        ui->lineEditUrav5->setFocus();
        break;
    }

    uravCount++; // Увеличиваем счетчик
}


void MainWindow::displaySolution(const QJsonObject& response) {
    QString solutionText;

    if (response.contains("steps")) {
        QJsonArray steps = response["steps"].toArray();
        for (const auto& step : steps)
            solutionText += step.toString() + "\n";
    }

    if (!solutionWindow)
        solutionWindow = new Solution(this);

    solutionWindow->setSolutionText(solutionText);
    solutionWindow->show();
    solutionWindow->raise();
    solutionWindow->activateWindow();
}


void MainWindow::on_pushButtonSolveUrav_clicked() {
    QJsonObject request;
    request["action"] = "solve";

    QJsonArray equations;
    equations.append(ui->lineEditUrav1->text());
    equations.append(ui->lineEditUrav2->text());

    if (uravCount >= 3) equations.append(ui->lineEditUrav3->text());
    if (uravCount >= 4) equations.append(ui->lineEditUrav4->text());
    if (uravCount >= 5) equations.append(ui->lineEditUrav5->text());

    request["equations"] = equations;

    NetworkClient::instance().sendSolutionRequest(
        request,
        [this](bool success, const QString& result) {

            qDebug() << "Ответ от сервера:" << result; // Логируем результат

            if (success) {
                QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8());
                QJsonObject response = doc.object();

                if (response["status"] == "success") {
                    //displaySolution(response["solution"].toString());
                    //QString solutionText = response["solution"].toString();  // <- здесь исправлено
                    displaySolution(response);
                } else {
                    // 🔽 Вот сюда вставляется улучшенный вывод ошибки:
                    QString errorText = response["message"].toString();
                    if (errorText.trimmed().isEmpty())
                        errorText = "Произошла неизвестная ошибка.";

                    QMessageBox::critical(this, "Error", errorText);
                }
            } else {
                // Это сообщение появляется, если ошибка произошла на сетевом уровне
                QString errorText = result.trimmed();
                if (errorText.isEmpty())
                    errorText = "Не удалось получить ответ от сервера.";
                QMessageBox::critical(this, "Error", errorText);
            }
        });
}

