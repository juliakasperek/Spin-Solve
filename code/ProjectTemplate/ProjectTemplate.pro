TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += qt
CONFIG += testcase
QT -= gui
QT += core gui widgets

SOURCES += \
    Difficulty.cpp \
        GameController.cpp \
    Help.cpp \
    Instructions.cpp \
        MainController.cpp \
    PhraseHandler.cpp \
        PhraseLibrary.cpp \
    Timer.cpp \
        Wheel.cpp \
        main.cpp

HEADERS += \
    Difficulty.h \
    GameController.h \
    Help.h \
    Instructions.h \
    MainController.h \
    PhraseHandler.h \
    PhraseLibrary.h \
    PlayerGems.h \
    Timer.h \
    Wheel.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/lib -lgtest -lgtest_main
