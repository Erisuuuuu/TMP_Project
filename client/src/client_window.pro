# Клиентская часть проекта
QT       += core gui network widgets
CONFIG   += c++17

# Отключение deprecated API (раскомментируйте при необходимости)
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    authregform.cpp \
    main.cpp \
    mainwindow.cpp \
    managerformss.cpp \
    networkclient.cpp \
    passwordrecoveryform.cpp \
    solution.cpp

HEADERS += \
    authregform.h \
    mainwindow.h \
    managerformss.h \
    networkclient.h \
    passwordrecoveryform.h \
    solution.h

FORMS += \
    ../ui/authregform.ui \
    ../ui/mainwindow.ui \
    ../ui/passwordrecoveryform.ui \
    ../ui/solution.ui

# Правила деплоя
unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
