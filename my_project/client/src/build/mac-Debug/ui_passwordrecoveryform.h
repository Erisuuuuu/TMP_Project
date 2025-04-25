/********************************************************************************
** Form generated from reading UI file 'passwordrecoveryform.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDRECOVERYFORM_H
#define UI_PASSWORDRECOVERYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_passwordrecoveryform
{
public:
    QLabel *labelEmailExplanation;
    QPushButton *buttonSendCode;
    QPushButton *buttonConfirmCode;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *labelEnterEmail;
    QLineEdit *lineEditEmail;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelEnterCode;
    QLineEdit *lineEditCode;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLineEdit *lineEditNewPassword;
    QLabel *labelRepeatPassword;
    QLineEdit *lineEditRepeatPassword;
    QLabel *labelCreateNewPassword;
    QPushButton *buttonConfirmNewPassword;
    QLabel *label;

    void setupUi(QDialog *passwordrecoveryform)
    {
        if (passwordrecoveryform->objectName().isEmpty())
            passwordrecoveryform->setObjectName("passwordrecoveryform");
        passwordrecoveryform->resize(586, 399);
        labelEmailExplanation = new QLabel(passwordrecoveryform);
        labelEmailExplanation->setObjectName("labelEmailExplanation");
        labelEmailExplanation->setGeometry(QRect(100, 100, 341, 20));
        buttonSendCode = new QPushButton(passwordrecoveryform);
        buttonSendCode->setObjectName("buttonSendCode");
        buttonSendCode->setGeometry(QRect(210, 130, 171, 29));
        buttonConfirmCode = new QPushButton(passwordrecoveryform);
        buttonConfirmCode->setObjectName("buttonConfirmCode");
        buttonConfirmCode->setGeometry(QRect(210, 220, 171, 31));
        horizontalLayoutWidget = new QWidget(passwordrecoveryform);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(100, 60, 401, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelEnterEmail = new QLabel(horizontalLayoutWidget);
        labelEnterEmail->setObjectName("labelEnterEmail");

        horizontalLayout->addWidget(labelEnterEmail);

        lineEditEmail = new QLineEdit(horizontalLayoutWidget);
        lineEditEmail->setObjectName("lineEditEmail");

        horizontalLayout->addWidget(lineEditEmail);

        horizontalLayoutWidget_2 = new QWidget(passwordrecoveryform);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(100, 160, 401, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelEnterCode = new QLabel(horizontalLayoutWidget_2);
        labelEnterCode->setObjectName("labelEnterCode");

        horizontalLayout_2->addWidget(labelEnterCode);

        lineEditCode = new QLineEdit(horizontalLayoutWidget_2);
        lineEditCode->setObjectName("lineEditCode");

        horizontalLayout_2->addWidget(lineEditCode);

        formLayoutWidget = new QWidget(passwordrecoveryform);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(100, 260, 401, 71));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        lineEditNewPassword = new QLineEdit(formLayoutWidget);
        lineEditNewPassword->setObjectName("lineEditNewPassword");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditNewPassword);

        labelRepeatPassword = new QLabel(formLayoutWidget);
        labelRepeatPassword->setObjectName("labelRepeatPassword");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelRepeatPassword);

        lineEditRepeatPassword = new QLineEdit(formLayoutWidget);
        lineEditRepeatPassword->setObjectName("lineEditRepeatPassword");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditRepeatPassword);

        labelCreateNewPassword = new QLabel(formLayoutWidget);
        labelCreateNewPassword->setObjectName("labelCreateNewPassword");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelCreateNewPassword);

        buttonConfirmNewPassword = new QPushButton(passwordrecoveryform);
        buttonConfirmNewPassword->setObjectName("buttonConfirmNewPassword");
        buttonConfirmNewPassword->setGeometry(QRect(210, 350, 201, 31));
        label = new QLabel(passwordrecoveryform);
        label->setObjectName("label");
        label->setGeometry(QRect(220, 0, 251, 71));

        retranslateUi(passwordrecoveryform);

        QMetaObject::connectSlotsByName(passwordrecoveryform);
    } // setupUi

    void retranslateUi(QDialog *passwordrecoveryform)
    {
        passwordrecoveryform->setWindowTitle(QCoreApplication::translate("passwordrecoveryform", "Dialog", nullptr));
        labelEmailExplanation->setText(QCoreApplication::translate("passwordrecoveryform", "(\320\235\320\260 \321\215\321\202\321\203 \320\277\320\276\321\207\321\202\321\203 \320\261\321\203\320\264\320\265\321\202 \320\276\321\202\320\277\321\200\320\260\320\262\320\273\320\265\320\275 \321\203\320\275\320\270\320\272\320\260\320\273\321\214\320\275\321\213\320\271 \320\272\320\276\320\264)", nullptr));
        buttonSendCode->setText(QCoreApplication::translate("passwordrecoveryform", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \320\272\320\276\320\264", nullptr));
        buttonConfirmCode->setText(QCoreApplication::translate("passwordrecoveryform", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\264\320\270\321\202\321\214 \320\272\320\276\320\264", nullptr));
        labelEnterEmail->setText(QCoreApplication::translate("passwordrecoveryform", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\276\321\207\321\202\321\203              ", nullptr));
        labelEnterCode->setText(QCoreApplication::translate("passwordrecoveryform", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\272\320\276\320\264 \320\270\320\267 \320\277\320\270\321\201\321\214\320\274\320\260", nullptr));
        labelRepeatPassword->setText(QCoreApplication::translate("passwordrecoveryform", "\320\237\320\276\320\262\321\202\320\276\321\200\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        labelCreateNewPassword->setText(QCoreApplication::translate("passwordrecoveryform", "\320\237\321\200\320\270\320\264\321\203\320\274\320\260\320\271\321\202\320\265 \320\275\320\276\320\262\321\213\320\271 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        buttonConfirmNewPassword->setText(QCoreApplication::translate("passwordrecoveryform", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\264\320\270\321\202\321\214 \320\275\320\276\320\262\321\213\320\271 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        label->setText(QCoreApplication::translate("passwordrecoveryform", "<html><head/><body><p><span style=\" font-size:11pt;\">\320\222\320\276\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\270\320\265 \320\277\320\260\321\200\320\276\320\273\321\217</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class passwordrecoveryform: public Ui_passwordrecoveryform {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDRECOVERYFORM_H
