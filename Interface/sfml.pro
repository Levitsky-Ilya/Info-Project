TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    background.cpp

LIBS += -LE:/Programs/SFML/SFML-2.2-windows-gcc-4.9.2-mingw-32-bit/SFML-2.2/lib
LIBS += -lsfml-graphics -lsfml-window -lsfml-system
INCLUDEPATH = E:/Programs/SFML/SFML-2.2-windows-gcc-4.9.2-mingw-32-bit/SFML-2.2/include

HEADERS += \
    background.h
