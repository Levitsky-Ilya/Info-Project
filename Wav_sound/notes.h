#ifndef NOTES_H
#define NOTES_H

#include <array>

#include "wav_sound.h"
#include "../fftAlgorithm/fft.h"
#include "frequencies_for_notes.h"

const int NUMBER_OF_BLOCKS = 4; // or 3, I'll define later


struct Note
{
    float freq;
    int nNote;
    int duration;
    float initTime;
};

class Notes
{
public:
    Notes(const char* fileName);
    void generateMidView(vector<Note> & notesOut);

private:
    //enum TypeFrame {SIMPLE, WITH_OVERLAP};
    struct Block
    {
        vector<array<float, NUMBER_OF_NOTES>> block;
        unsigned int frameSize;
        int firstNote;
        int lastNote;
        int outNum;
        float diffFreq;
        //TypeFrame typeFrame;

        void execute(const vector<float> & amplTime,
                     const float * const delta);
        void freqToNote(const float * const outFft,
                        const float * const delta,
                        array<float, NUMBER_OF_NOTES> & notes);
        void dump(int nTime);
    };

    float initDiffFreq[NUMBER_OF_NOTES - 1];
    float initDeltaFreq[NUMBER_OF_NOTES - 1];

    vector<float> amplTime;
    Block blocks[NUMBER_OF_BLOCKS];

    int maxNote(array<float, NUMBER_OF_NOTES> & ampl);
};

#endif // NOTES_H
