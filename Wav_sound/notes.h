#ifndef NOTES_H
#define NOTES_H

#include "wav_sound.h"
#include "../fftAlgorithm/fft.h"

const int NUMBER_OF_NOTES = 84; //do I have 84 notes? Really???
enum TypeFrame {WITH_NULLS, WITHOUT_NULLS};

struct Note
{
    float freq;
    int nNote;
    int duration;
    float initTime;
};


struct NotesList
{
public:
    float notesList[NUMBER_OF_NOTES];
    float diffFreq;
    int numFirstNote;
    int numLastNote;
    float & operator [] (int n);
};

// const float & operator [] (int n) const;

class Notes
{
public:
    Notes();
    void generateMidView(const char* fileName);

    vector<Note> notesOut;
private:
    vector<float> FreqList; // initial list of notes
    vector<NotesList> partFirst;
    vector<NotesList> partSecond;
    int maxNote(NotesList& ampl);
    void executeBlock(unsigned int frameSize, int sampleRate,
                      int firstNote, int lastNote, TypeFrame typeFrame,
                      vector<float>& amplTime);
    void freqToNote(float * const outFft, int num, NotesList &amplNotes);
};

#endif // NOTES_H
