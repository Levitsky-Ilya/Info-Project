TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += E:/Programs/SFML/SFML-2.3-windows-gcc-4.9.2-mingw-32-bit/SFML-2.3.2/include
INCLUDEPATH += E:/Programs/SFGUI/SFGUI-0.3.0/install/include

LIBS += -LE:/Programs/SFML/SFML-2.3-windows-gcc-4.9.2-mingw-32-bit/SFML-2.3.2/lib
LIBS += -lsfml-graphics -lsfml-window -lsfml-system

LIBS += -LE:/Programs/SFGUI/SFGUI-0.3.0/install/lib
LIBS += -lsfgui

