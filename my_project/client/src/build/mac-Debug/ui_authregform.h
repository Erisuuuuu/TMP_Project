/********************************************************************************
** Form generated from reading UI file 'authregform.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHREGFORM_H
#define UI_AUTHREGFORM_H

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

class Ui_AuthRegForm
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAuth;
    QPushButton *pushButtonReg;
    QPushButton *pushButtonChange;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *labelLogin;
    QLineEdit *lineEditLogin;
    QLabel *labelPassword;
    QLineEdit *lineEditPassword;
    QLabel *labelPasswordCheck;
    QLineEdit *lineEditPasswordCheck;
    QLabel *labelMail;
    QLineEdit *lineEditMail;
    QPushButton *pushButtonForgotPassword;
    QLabel *labelHelp;

    void setupUi(QDialog *AuthRegForm)
    {
        if (AuthRegForm->objectName().isEmpty())
            AuthRegForm->setObjectName("AuthRegForm");
        AuthRegForm->resize(430, 324);
        horizontalLayoutWidget = new QWidget(AuthRegForm);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(50, 140, 347, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonAuth = new QPushButton(horizontalLayoutWidget);
        pushButtonAuth->setObjectName("pushButtonAuth");

        horizontalLayout->addWidget(pushButtonAuth);

        pushButtonReg = new QPushButton(horizontalLayoutWidget);
        pushButtonReg->setObjectName("pushButtonReg");

        horizontalLayout->addWidget(pushButtonReg);

        pushButtonChange = new QPushButton(horizontalLayoutWidget);
        pushButtonChange->setObjectName("pushButtonChange");

        horizontalLayout->addWidget(pushButtonChange);

        formLayoutWidget = new QWidget(AuthRegForm);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(10, 30, 409, 131));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        labelLogin = new QLabel(formLayoutWidget);
        labelLogin->setObjectName("labelLogin");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelLogin);

        lineEditLogin = new QLineEdit(formLayoutWidget);
        lineEditLogin->setObjectName("lineEditLogin");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditLogin);

        labelPassword = new QLabel(formLayoutWidget);
        labelPassword->setObjectName("labelPassword");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPassword);

        lineEditPassword = new QLineEdit(formLayoutWidget);
        lineEditPassword->setObjectName("lineEditPassword");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

        labelPasswordCheck = new QLabel(formLayoutWidget);
        labelPasswordCheck->setObjectName("labelPasswordCheck");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelPasswordCheck);

        lineEditPasswordCheck = new QLineEdit(formLayoutWidget);
        lineEditPasswordCheck->setObjectName("lineEditPasswordCheck");

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEditPasswordCheck);

        labelMail = new QLabel(formLayoutWidget);
        labelMail->setObjectName("labelMail");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelMail);

        lineEditMail = new QLineEdit(formLayoutWidget);
        lineEditMail->setObjectName("lineEditMail");

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditMail);

        pushButtonForgotPassword = new QPushButton(AuthRegForm);
        pushButtonForgotPassword->setObjectName("pushButtonForgotPassword");
        pushButtonForgotPassword->setGeometry(QRect(130, 210, 161, 41));
        labelHelp = new QLabel(AuthRegForm);
        labelHelp->setObjectName("labelHelp");
        labelHelp->setGeometry(QRect(40, 260, 391, 20));

        retranslateUi(AuthRegForm);

        QMetaObject::connectSlotsByName(AuthRegForm);
    } // setupUi

    void retranslateUi(QDialog *AuthRegForm)
    {
        AuthRegForm->setWindowTitle(QCoreApplication::translate("AuthRegForm", "Dialog", nullptr));
        pushButtonAuth->setText(QCoreApplication::translate("AuthRegForm", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        pushButtonReg->setText(QCoreApplication::translate("AuthRegForm", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        pushButtonChange->setText(QCoreApplication::translate("AuthRegForm", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214\321\201\321\217", nullptr));
        labelLogin->setText(QCoreApplication::translate("AuthRegForm", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        labelPassword->setText(QCoreApplication::translate("AuthRegForm", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        labelPasswordCheck->setText(QCoreApplication::translate("AuthRegForm", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        labelMail->setText(QCoreApplication::translate("AuthRegForm", "\320\237\320\276\321\207\321\202\320\260", nullptr));
        pushButtonForgotPassword->setText(QCoreApplication::translate("AuthRegForm", "\320\227\320\260\320\261\321\213\320\273\320\270 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        labelHelp->setText(QCoreApplication::translate("AuthRegForm", "<html><head/><body><p><span style=\" font-size:8pt;\">\320\225\321\201\320\273\320\270 \321\203 \320\262\320\260\321\201 \320\275\320\265 \320\277\320\276\320\273\321\203\321\207\320\260\320\265\321\202\321\201\321\217 \320\262\320\276\320\271\321\202\320\270,\320\262\321\213 \320\274\320\276\320\266\320\265\321\202\320\265 \320\262\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 \320\277\320\260\321\200\320\276\320\273\321\214</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AuthRegForm: public Ui_AuthRegForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHREGFORM_H
