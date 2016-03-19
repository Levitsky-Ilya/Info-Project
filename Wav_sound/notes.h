#ifndef NOTES_H
#define NOTES_H

#include "wav_sound.h"
#include "../fftAlgorithm/fft.h"

const int NUMBER_OF_NOTES = 84; //do I have 84 notes? Really???

struct Note
{
    double freq;
    int nNote;
    int duration;
    double initTime;
};


struct NotesList
{
    double notesList[NUMBER_OF_NOTES];
    double diffFreq;
    int numFirstNote;
    int numLastNote;
};

class Notes
{
public:
    Notes();
    void generateMidView(const char* fileName);

    vector<Note> notesOut;
private:
    vector<double> notesFreqList; // initial list of notes
    vector<NotesList> partOne;
    void freqToNote(double*const outFft, int num, NotesList &amplNotes);
};

#endif // NOTES_H
