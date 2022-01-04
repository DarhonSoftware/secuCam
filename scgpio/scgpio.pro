TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    custom.cext \
    command.c \
    pigpio.c

HEADERS += pigpio.h \
    command.h



