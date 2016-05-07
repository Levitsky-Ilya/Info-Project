#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T14:39:26
#
#-------------------------------------------------

QT       += core gui

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_ui
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    complex.cpp \
    fft.cpp \
    freq_names.cpp \
    global.cpp \
    note_durations.cpp \
    note_pauses.cpp \
    notes.cpp \
    queue.cpp \
    wav_sound.cpp

HEADERS  += widget.h \
    complex.h \
    exception.h \
    exceptions.h \
    fft.h \
    freq_names.h \
    frequencies_for_notes.h \
    global.h \
    mytread.h \
    note_durations.h \
    note_pauses.h \
    notes.h \
    queue.h \
    wav_sound.h

FORMS    += widget.ui

DISTFILES += \
    test_ui.pro.user
