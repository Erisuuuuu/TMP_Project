#главный файл проекта
#автоматически подключает графический интерфейс
QT       += core gui
QT += network #подключаем иодуль сети для работы с сетью : Этот параметр указывает Qt на необходимость включения модуля для работы с сетевыми запросами.
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#берет конфигурацию языка с++17
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authregform.cpp \
    functions_for_client.cpp \
    main.cpp \
    mainwindow.cpp \
    managerformss.cpp \
    networkclient.cpp \
    passwordrecoveryform.cpp \
    solution.cpp

HEADERS += \
    authregform.h \
    functions_for_client.h \
    mainwindow.h \
    managerformss.h \
    networkclient.h \
    passwordrecoveryform.h \
    solution.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    authregform.ui \
    mainwindow.ui \
    passwordrecoveryform.ui \
    solution.ui
