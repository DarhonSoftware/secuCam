TEMPLATE = app

QT += network sql
QT -= gui

TARGET = scnotifier

SOURCES += main.cpp \
    dr_smtpclient.cpp

HEADERS += \
    dr_smtpclient.h

RESOURCES += \
    scnotifier.qrc



