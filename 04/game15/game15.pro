TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    board.cpp \
    helpers.cpp

HEADERS += \
    board.hh \
    helpers.hh

DISTFILES += \
    examples.txt
