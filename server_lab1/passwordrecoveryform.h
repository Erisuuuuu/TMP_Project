#ifndef PASSWORDRECOVERYFORM_H
#define PASSWORDRECOVERYFORM_H

#include "ui_passwordrecoveryform.h"
#include <QDialog>

namespace Ui {
class passwordrecoveryform;
}

class passwordrecoveryform : public QDialog
{
    Q_OBJECT

public:
    explicit passwordrecoveryform(QWidget *parent = nullptr);
    ~passwordrecoveryform();
public slots:
    void slot_show();
private slots:

    void on_buttonSendCode_clicked();  // Кнопка для отправки кода
    void on_buttonConfirmCode_clicked();  // Кнопка для подтверждения кода
    void on_buttonConfirmNewPassword_clicked();  // Кнопка для подтверждения нового пароля

private:
    Ui::passwordrecoveryform *ui;

    bool isCodeValid;  // Для отслеживания, был ли код введен правильно
    QString recoveryCode;  // Сохранение отправленного кода
};

#endif // PASSWORDRECOVERYFORM_H

