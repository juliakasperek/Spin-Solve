TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += qt
QT += core gui widgets

SOURCES += \
        MainController.cpp \
        PhraseLibrary.cpp \
        game.cpp \
        main.cpp

HEADERS += \
    MainController.h \
    PhraseLibrary.h \
    Player.h \
    game.h
