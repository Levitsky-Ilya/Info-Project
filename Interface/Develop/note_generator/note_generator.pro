TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../additional_files/complex.cpp \
    ../additional_files/fft.cpp \
    ../additional_files/notes.cpp \
    ../additional_files/wav_sound.cpp \
    global.cpp \
    queue.cpp \
    note_durations.cpp \
    note_pauses.cpp \
    freq_names.cpp

HEADERS += \
    ../additional_files/complex.h \
    ../additional_files/fft.h \
    ../additional_files/frequencies_for_notes.h \
    ../additional_files/notes.h \
    ../additional_files/wav_sound.h \
    ../additional_files/exception.h \
    freq_names.h \
    note_pauses.h \
    global.h \
    queue.h \
    note_durations.h

