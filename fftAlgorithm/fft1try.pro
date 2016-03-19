TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    complex.cpp \
    fft.cpp

INCLUDEPATH += C:/Qt/prject1/infproject/fft1try/complex/include
INCLUDEPATH += C:/Qt/prject1/infproject/fft1try/fft/include

LIBS += -LC:/Qt/prject1/infproject/fft1try/complex/lib
LIBS += -LC:/Qt/prject1/infproject/fft1try/fft/lib

HEADERS += \
    complex.h \
    fft.h




