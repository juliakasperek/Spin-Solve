TEMPLATE = app
CONFIG += testcase
QT += widgets
QT += testlib
QT += core gui widgets

TARGET = tests

SOURCES += \
    ../Difficulty.cpp \
    TestGame.cpp \
    ../GameController.cpp \
    ../Wheel.cpp \
    ../PhraseHandler.cpp \
    ../PhraseLibrary.cpp \
    ../MainController.cpp \
    ../Help.cpp \
    ../Instructions.cpp \
    ../Timer.cpp \
    TestPhrase.cpp \
    TestWheel.cpp

HEADERS += \
    ../Difficulty.h \
    ../GameController.h \
    ../PlayerGems.h \
    ../Instructions.h \
    ../Wheel.h \
    MockWheel.h \
    ../PhraseHandler.h \
    ../PhraseLibrary.h \
    ../MainController.h \
    ../Help.h \
    ../Timer.h \

INCLUDEPATH += ../
INCLUDEPATH += /opt/homebrew/include

LIBS += -L/opt/homebrew/lib -lgtest -lgtest_main -lgmock -lgmock_main
