QT += testlib core network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += C:/Users/anast/Downloads/Проект-1-1/Проект-1-1/Проект-1/my_project/client/src

SOURCES += tst_networkclienttest.cpp \
    C:/Users/anast/Downloads/Проект-1-1/Проект-1-1/Проект-1/my_project/client/src/networkclient.cpp

HEADERS += C:/Users/anast/Downloads/Проект-1-1/Проект-1-1/Проект-1/my_project/client/src/networkclient.h
