#ifndef NOTES_H
#define NOTES_H

#include "fft_transform.h"
#include "wav_sound.h"

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
};

class Notes
{
public:
    Notes();
    int generateMidView(const char* fileName);

    vector<Note> notesOut;
private:
    vector<double> notesFreqList;
    vector<NotesList> partOne;
};

struct AmplFreqArray //use if it's necessary
{
    std::vector<double> amplFreq;
    double diffFreq;
    double diffTime;
    int ntime;
};

#endif // NOTES_H
