TEMPLATE = app
CONFIG += CFLAGS:-static
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        scan.cpp

HEADERS += \
    scan.h
