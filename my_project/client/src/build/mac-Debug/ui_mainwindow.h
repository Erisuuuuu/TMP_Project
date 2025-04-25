/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButtonSolveUrav;
    QLabel *labelVvod;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLineEdit *lineEditUrav1;
    QLabel *labelUrav2;
    QLineEdit *lineEditUrav2;
    QLabel *labelUrav3;
    QLineEdit *lineEditUrav3;
    QLabel *labelUrav4;
    QLineEdit *lineEditUrav4;
    QLabel *labelUrav5;
    QLineEdit *lineEditUrav5;
    QLabel *labelUrav1;
    QTextBrowser *textBrowser;
    QLineEdit *lineEditMethod;
    QLabel *label;
    QPushButton *pushButtonAddUrav;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButtonSolveUrav = new QPushButton(centralwidget);
        pushButtonSolveUrav->setObjectName("pushButtonSolveUrav");
        pushButtonSolveUrav->setGeometry(QRect(270, 500, 271, 41));
        labelVvod = new QLabel(centralwidget);
        labelVvod->setObjectName("labelVvod");
        labelVvod->setGeometry(QRect(100, 30, 591, 51));
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(60, 90, 651, 197));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        lineEditUrav1 = new QLineEdit(formLayoutWidget);
        lineEditUrav1->setObjectName("lineEditUrav1");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUrav1);

        labelUrav2 = new QLabel(formLayoutWidget);
        labelUrav2->setObjectName("labelUrav2");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelUrav2);

        lineEditUrav2 = new QLineEdit(formLayoutWidget);
        lineEditUrav2->setObjectName("lineEditUrav2");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditUrav2);

        labelUrav3 = new QLabel(formLayoutWidget);
        labelUrav3->setObjectName("labelUrav3");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelUrav3);

        lineEditUrav3 = new QLineEdit(formLayoutWidget);
        lineEditUrav3->setObjectName("lineEditUrav3");

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEditUrav3);

        labelUrav4 = new QLabel(formLayoutWidget);
        labelUrav4->setObjectName("labelUrav4");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelUrav4);

        lineEditUrav4 = new QLineEdit(formLayoutWidget);
        lineEditUrav4->setObjectName("lineEditUrav4");

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditUrav4);

        labelUrav5 = new QLabel(formLayoutWidget);
        labelUrav5->setObjectName("labelUrav5");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelUrav5);

        lineEditUrav5 = new QLineEdit(formLayoutWidget);
        lineEditUrav5->setObjectName("lineEditUrav5");

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEditUrav5);

        labelUrav1 = new QLabel(formLayoutWidget);
        labelUrav1->setObjectName("labelUrav1");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelUrav1);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(60, 270, 661, 161));
        lineEditMethod = new QLineEdit(centralwidget);
        lineEditMethod->setObjectName("lineEditMethod");
        lineEditMethod->setGeometry(QRect(370, 440, 101, 28));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 440, 291, 31));
        pushButtonAddUrav = new QPushButton(centralwidget);
        pushButtonAddUrav->setObjectName("pushButtonAddUrav");
        pushButtonAddUrav->setGeometry(QRect(520, 440, 221, 29));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonSolveUrav->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\321\210\320\270\321\202\321\214  \321\201\320\270\321\201\321\202\320\265\320\274\321\203 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\271!", nullptr));
        labelVvod->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\201\320\270\321\201\321\202\320\265\320\274\321\203 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\271( \320\264\320\276\320\277\321\203\321\201\321\202\320\270\320\274\320\276 \320\274\320\260\320\272\321\201\320\270\320\274\321\203\320\274 5  \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\271 \320\262 \321\201\320\270\321\201\321\202\320\265\320\274\320\265):", nullptr));
        labelUrav2->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \342\204\2262:", nullptr));
        labelUrav3->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \342\204\2263:", nullptr));
        labelUrav4->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \342\204\2264:", nullptr));
        labelUrav5->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \342\204\2265:", nullptr));
        labelUrav1->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \342\204\2261:", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\274\320\265\321\202\320\276\320\264 \321\200\320\265\321\210\320\265\320\275\320\270\321\217:</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" fon"
                        "t-size:11pt; font-style:italic;\">1.\320\234\320\265\321\202\320\276\320\264 1</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">2.\320\234\320\265\321\202\320\276\320\264 2</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">3.\320\234\320\265\321\202\320\276\320\264 3</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">4.\320\234\320\265\321\202\320\276\320\264 4</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:i"
                        "talic;\">5.\320\234\320\265\321\202\320\276\320\264 5</span></p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:700; font-style:italic;\">\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\275\320\276\320\274\320\265\321\200 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\263\320\276  \320\274\320\265\321\202\320\276\320\264\320\260:</span></p></body></html>", nullptr));
        pushButtonAddUrav->setText(QCoreApplication::translate("MainWindow", "+\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
