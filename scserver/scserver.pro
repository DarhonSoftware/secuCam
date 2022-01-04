TEMPLATE = app

QT += network sql
QT -= gui

TARGET = scserver

SOURCES += main.cpp \
    dr_crypt.cpp \
    dr_sql.cpp \
    dr_tcpserver.cpp \
    globalobject.cpp

HEADERS += \
    dr_crypt.h \
    dr_sql.h \
    dr_tcpserver.h \
    globalobject.h

RESOURCES += \
    scserver.qrc



