QT -= gui
QT += core network sql

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    configloader.cpp \
    databasemanager.cpp \
    main.cpp \
    mytcpserver.cpp \
    equationparser.cpp\
    smtpclient.cpp \
    gaussianelimination.cpp\
    requesthandler.cpp \
    solver.cpp

HEADERS += \
    configloader.h \
    databasemanager.h \
    mytcpserver.h \
    equationparser.h\
    smtpclient.h \
    gaussianelimination.h\
    requesthandler.h \
    solver.h

# Деплой
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
CONFIG += deployment
DEPLOYMENT.path = $$OUT_PWD
DEPLOYMENT.files = config.json
INSTALLS += deployment
