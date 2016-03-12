#ifndef NOTES_H
#define NOTES_H
#include "fft_transform.h"
#include "wav_sound.h"

class Notes
{
public:
    int generateMidView(const char* fileName);
    //std::vector<Note> octaveGreat;
    //std::vector<Note> octaveSmall;
    std::vector<Note> octave1;
    std::vector<Note> octave2;
    std::vector<Note> octave3;
};

struct Note
{
    double freq;
    double duration;
    double initTime;
};

struct AmplFreqArray //use if it's necessary
{
    std::vector<double> amplFreq;
    double diffFreq;
    double diffTime;
    int ntime;
};

#endif // NOTES_H

