QT -= gui
QT += core network sql

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    databasemanager.cpp \
    main.cpp \
    mytcpserver.cpp \
    equationparser.cpp\
    solver.cpp\
    gaussianelimination.cpp\
    requesthandler.cpp

HEADERS += \
    databasemanager.h \
    mytcpserver.h \
    equationparser.h\
    solver.h\
    gaussianelimination.h\
    requesthandler.h

# Деплой
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
CONFIG += deployment
DEPLOYMENT.path = $$OUT_PWD
DEPLOYMENT.files = config.json
INSTALLS += deployment
