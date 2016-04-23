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
    Notes();
    void initialize(string fileName);
    void generateMidView(vector<Note>& notesOut);

    bool dump(ostream& fout);
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
        void indentifyPeaks(unsigned int nTime);
        void peaksToNotes(vector<Note>& notes);
        bool dump(unsigned int nTime, ostream& fout);
    };

    float initDiffFreq[NUMBER_OF_NOTES - 1];

    vector<float> amplTime;
    Block blocks[NUMBER_OF_BLOCKS];

    void indentifyPeaks();
    void complementBlocks(unsigned int nTime);
    void checkPeaks(unsigned int nTime);

    void notesFromPeaks(vector<Note>& notesOut);

    int minBlock(NoteBlock notes[]);
    //int maxNote(AmplNotes & ampl);
};

#endif // NOTES_H
