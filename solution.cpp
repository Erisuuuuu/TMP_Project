
#include "solution.h"
#include "ui_solution.h"

// Конструктор класса Solution
Solution::Solution(QWidget *parent)          // Наследуем от QMainWindow
    : QMainWindow(parent)                   // Передаем родительский виджет в базовый класс
    , ui(new Ui::Solution)                  // Создаем новый экземпляр UI-класса
{
    ui->setupUi(this);  // Инициализируем пользовательский интерфейс из файла solution.ui
        // Этот метод автоматически создает все виджеты, указанные в дизайнере Qt
}

// Деструктор класса Solution
Solution::~Solution()
{
    delete ui;  // Удаляем объект пользовательского интерфейса
        // Это освобождает все связанные ресурсы и дочерние виджеты
}

// Слот для отображения окна
void Solution::slot_show()
{
    this->show();  // Делаем окно видимым
        // Метод show() наследуется от QWidget и отображает окно
}
