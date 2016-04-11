#ifndef NOTES_H
#define NOTES_H

#include <array>

#include "../wav_sound.h"
#include "../fftAlgorithm/fft.h"
#include "frequencies_for_notes.h"

const int NUMBER_OF_BLOCKS = 3; // or 4, I'll define later
const float PEAK_MINIMUM = 10.0; /// attention!!! I can't define the lavel of silence now!
const float DELTA_PEAK = 1.0; /// attention!!!


struct Note
{
    int nNote;
    float duration;
    float initTime;
};

struct NoteBlock
{
    vector<Note> noteBlock;
    int current;
    int size;
};

class Notes
{
public:
    Notes(const char* fileName);
    void generateMidView(vector<Note>& notesOut);

    void dump(ostream& fout);
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
                     const float* const delta);
        void freqToNote(const float* const outFft,
                        const float* const delta,
                        array<float, NUMBER_OF_NOTES> & notes);
        void keepOnlyPeaks(int nTime);
        void peaksToNotes(vector<Note>& notes);
        void dump(int nTime, ostream& fout);
    };

    float initDiffFreq[NUMBER_OF_NOTES - 1];
    float initDeltaFreq[NUMBER_OF_NOTES - 1];

    vector<float> amplTime;
    Block blocks[NUMBER_OF_BLOCKS];

    void keepOnlyPeaks();
    void complementBlocks(int nTime);
    void checkPeaks(int nTime);

    void notesFromPeaks(vector<Note>& notesOut);

    int minBlock(NoteBlock notes[]);
    //int maxNote(array<float, NUMBER_OF_NOTES> & ampl);
};

#endif // NOTES_H
