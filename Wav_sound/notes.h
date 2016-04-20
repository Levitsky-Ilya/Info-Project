/**
 * notes.h
 *
 * Description: Notes class header.
 *
 * @author Maria Kataeva mariya.katayeva@phystech.edu
 * Copyright 2016
 **/

#ifndef NOTES_H
#define NOTES_H

#include <array>

#include "../wav_sound.h"
#include "../fftAlgorithm/fft.h"
#include "frequencies_for_notes.h"

#define NUMBER_OF_BLOCKS 3 // type 4 to use four blocks

const float PEAK_MINIMUM = 20.0; /// attention!!! I can't define the lavel of silence now!
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

struct AmplNotes
{
    array<float, NUMBER_OF_NOTES> amplNotes;
    float maxAmpl;
    float& operator[] (int n) {
        return amplNotes[n];
    }
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
        vector<AmplNotes> block;
        unsigned int frameSize;
        int firstNote;
        int lastNote;
        float diffFreq;
        //TypeFrame typeFrame;

        void execute(const vector<float> & amplTime);
        void freqToNote(const float* const outFft,
                        AmplNotes & notes);
        void keepOnlyPeaks(int nTime);
        void peaksToNotes(vector<Note>& notes);
        void dump(int nTime, ostream& fout);
    };

    float initDiffFreq[NUMBER_OF_NOTES - 1];

    vector<float> amplTime;
    Block blocks[NUMBER_OF_BLOCKS];

    void keepOnlyPeaks();
    void complementBlocks(int nTime);
    void checkPeaks(int nTime);

    void notesFromPeaks(vector<Note>& notesOut);

    int minBlock(NoteBlock notes[]);
    //int maxNote(AmplNotes & ampl);
};

#endif // NOTES_H
