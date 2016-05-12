/**
 * notes.cpp
 *
 * Description: Notes class realization.
 *              Generates intermediate notes viev:
 *              applies the fft algorithm to vector<float> of amplitudes,
 *              analyzes amplitudes of frequences,
 *              generates amplitudes of notes,
 *              finds the maximum peaks of amplitudes,
 *              finally returnes notes of peaks as vector<Note>.
 * @author Maria Kataeva mariya.katayeva@phystech.edu
 * Copyright 2016
 **/

#include "notes.h"
#include "exceptions.h"

#include <thread>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

#if NUMBER_OF_BLOCKS == 4
const int NOTE_C[NUMBER_OF_BLOCKS] = {36, 24, 12, 0};
#else
const int NOTE_C[NUMBER_OF_BLOCKS] = {24, 12, 0};
#endif

#define MIN_DURATION 0.1
#define DUMP_BEFORE_PEAKS 1 // type 1 to dump before selection peaks to dump.txt

Notes::Notes():
    initLevel(0.5),
    silenceLevel(0.0)
{
    for (int i = 0; i < NUMBER_OF_NOTES - 1; i++) {
        initDiffFreq[i] = initNotes[i+1] - initNotes[i];
    }

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].diffFreq = 0.0;
        blocks[i].firstNote = 0;
        blocks[i].frameSize = 0;
        blocks[i].lastNote = 0;
    }
}

void Notes::initialize(string fileName)
{
    WavFile melody;
    melody.initialize(fileName.c_str());
    unsigned int sampleRate = melody.getRate();

    if (sampleRate == 0) {
        string msg = "Failed to read headers of file: " + fileName;
        throw NotesExceptions::Connect(msg);
    }

    unsigned int frameSize = 1;
    int firstNote = 0;
    //int channels = melody.getChannels();

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        while (sampleRate / (frameSize << 1) > initDiffFreq[NOTE_C[i]]) {
            frameSize <<= 1;
        }
        if (frameSize <= NUMBER_OF_NOTES) {
            string msg = "Failed to read hider of file: " + fileName +
                    " wrong sample rate: " + std::to_string(sampleRate);
            throw NotesExceptions::Connect(msg);
        }

        blocks[i].frameSize = frameSize;
        blocks[i].diffFreq = (float)sampleRate / (float)frameSize;

        firstNote = NOTE_C[i];
        while(initDiffFreq[firstNote] < blocks[i].diffFreq)
            firstNote++;
        assert(firstNote < NUMBER_OF_NOTES - 1);

        blocks[i].firstNote = firstNote;
    }

    blocks[0].lastNote = NUMBER_OF_NOTES - 1;
    blocks[1].lastNote = blocks[0].firstNote;
    blocks[2].lastNote = blocks[1].firstNote;
#if NUMBER_OF_BLOCKS == 4
    blocks[3].lastNote = blocks[2].firstNote;
#endif

    melody.getAmplitudeArray(amplTime);
    if (amplTime.size() == 0) {
        string msg = "Failed to give data from file";
        throw NotesExceptions::Connect(msg);
    }

}

bool Notes::setSilenceLevel(float level)
{
    if (level >= 0 && level <= 1) {
        initLevel = level;
        return true;
    }
    else {
        return false;
    }
}

void Notes::generateMidView(vector<Note>& notesOut)
{
    notesOut.resize(0);

    thread thr0(blocks[0].execute, &(blocks[0]), ref(amplTime));
    thread thr1(blocks[1].execute, &(blocks[1]), ref(amplTime));
    thread thr2(blocks[2].execute, &(blocks[2]), ref(amplTime));
#if NUMBER_OF_BLOCKS == 4
    thread thr3(blocks[3].execute, &(blocks[3]), ref(amplTime));
#endif

    thr0.join();
    thr1.join();
    thr2.join();
#if NUMBER_OF_BLOCKS == 4
    thr3.join();
#endif

    /*for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].execute(amplTime, initDeltaFreq);
    }*/

#if DUMP_BEFORE_PEAKS
    ofstream fout("dump.txt");
    if (! dump(fout)) {
        cout << "Some problem with damping a file before indentifying pesks";
        cout << endl;
    }
    fout.close();
#endif
    getMaxAmpl();
    indentifyPeaks();

    notesFromPeaks(notesOut);

    return;
}

void Notes::Block::execute(const vector<float> & amplTime)
{
    AmplNotes amplNotes;

    CFFT fft;

    complex inFft[frameSize];
    float outFft[frameSize];
    size_t melodySize = amplTime.size();

    for (unsigned int i = 0; i < melodySize; i += frameSize) {

        //there is a duct tape, I think. Or not. I'll decide later.
        for (unsigned int j = 0; j < frameSize; j++) {
            if (i + j < melodySize) {
                inFft[j] = complex(amplTime[i+j], 0.0);
            }
            else {
                inFft[j] = complex(0.0, 0.0);
            }
        }

        fft.applyWindowG(inFft, frameSize);
        fft.fftAlgorithm(inFft, outFft, frameSize);

        freqToNote(outFft, amplNotes);

        block.push_back(amplNotes);
    }
}

void Notes::Block::freqToNote(const float * const outFft,
                              AmplNotes & notes)
{
    notes.amplNotes.fill(-INFINITY);
    float max = -INFINITY;

    unsigned int j = 0;
    float freq = 0;
    for (int k = firstNote; k <= lastNote; k++) {
        j = (int)(initNotes[k] / diffFreq);
        assert (j + 1 < frameSize);

        freq = diffFreq * j;
        if (freq + diffFreq - initNotes[k] < initNotes[k] - freq) {
            notes[k] = outFft[j + 1];
        }
        else {
            notes[k] = outFft[j];
        }
        if (max < notes[k]) {
            max = notes[k];
        }
    }

    notes.maxAmpl = max;

    return;
}

void Notes::getMaxAmpl()
{

    float maxAmplitude = -INFINITY;
    for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
        unsigned int size = blocks[j].block.size();
        for (unsigned int i = 0; i < size; i++) {
            if(blocks[j].block[i].maxAmpl > maxAmplitude)
                maxAmplitude = blocks[j].block[i].maxAmpl;
        }
    }
    if (maxAmplitude > 0)
        silenceLevel = maxAmplitude * initLevel;
    else
        silenceLevel = 0; //what should we do if there are not positive amplitudes?
}

void Notes::indentifyPeaks()
{
    unsigned int size = blocks[0].block.size();

    //chage if there is some application of typeFrame!!!
    for (unsigned int i = 0; i < size; i++) {

        //after this cycle there may be a conflict between
        //first note of current block
        //and last notes of elder block
        for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
            if (i % (1 << j) == 0) {
                blocks[j].indentifyPeaks(i >> j, silenceLevel);
            }
        }
        //because of it I solve this problem there:
        checkPeaks(i);
    }
}

void Notes::Block::indentifyPeaks(unsigned int nTime, float silenceLevel)
{
    assert(nTime < block.size());

    for (int i = lastNote; i >= firstNote; i--) {
        if (block[nTime][i] < silenceLevel ||
                block[nTime][i] < block[nTime].maxAmpl - DELTA_PEAK) {
            block[nTime][i] = -INFINITY;
        }
    }
}

/*void Notes::Block::keepOnlyPeaks(int nTime)
{
    if (block[nTime][lastNote] <= PEAK_MINIMUM ||
            block[nTime][lastNote] < block[nTime].maxAmpl - DELTA_PEAK ||
            block[nTime][lastNote] < block[nTime][lastNote - 1]) {
        block[nTime][lastNote] = -INFINITY;
    }

    for (int i = lastNote - 1; i >= firstNote; i--) {
        int j = i;
        while (block[nTime][j] < block[nTime][j + 1] && j > firstNote) {
            j--;
        }
        while (i > j) {
            block[nTime][i] = -INFINITY;
            i--;
        }
        if (block[nTime][i] <= PEAK_MINIMUM ||
                block[nTime][i] < block[nTime].maxAmpl - DELTA_PEAK ||
                block[nTime][i] < block[nTime][i - 1]) {

            block[nTime][i] = -INFINITY; // there is no 'seconds' in melody!!!
        }
    }
}
*/

void Notes::checkPeaks(unsigned int nTime)
{
    //chage if there is some application of typeFrame!!!
    for (int i = 0; i < NUMBER_OF_BLOCKS - 1; i++) {
        assert((nTime >> i) < blocks[i].block.size() &&
               (nTime >> (i+1)) < blocks[i+1].block.size());

        if (blocks[i].block[nTime >> i][blocks[i].firstNote] == -INFINITY) {
            blocks[i+1].block[nTime >> (i+1)][blocks[i+1].lastNote] = -INFINITY;
        }
        if (blocks[i+1].block[nTime >> (i+1)][blocks[i+1].lastNote] ==
                -INFINITY) {

            blocks[i].block[nTime >> i][blocks[i].firstNote] = -INFINITY;
        }
    }
}

void Notes::notesFromPeaks(vector<Note>& notesOut)
{
    NoteBlock notes[NUMBER_OF_BLOCKS];

    thread thr0(blocks[0].peaksToNotes, &(blocks[0]), ref(notes[0].noteBlock));
    thread thr1(blocks[1].peaksToNotes, &(blocks[1]), ref(notes[1].noteBlock));
    thread thr2(blocks[2].peaksToNotes, &(blocks[2]), ref(notes[2].noteBlock));
#if NUMBER_OF_BLOCKS == 4
    thread thr3(blocks[3].peaksToNotes, &(blocks[3]), ref(notes[3].noteBlock));
#endif

    thr0.join();
    thr1.join();
    thr2.join();
#if NUMBER_OF_BLOCKS == 4
    thr3.join();
#endif

    /*for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].peaksToNotes(notes[i].noteBlock);
    }*/

    unsigned int outSize = 0;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        notes[i].current = 0;
        notes[i].size = notes[i].noteBlock.size();
        outSize += notes[i].size;
    }

    notesOut.resize(outSize);
    unsigned int initTime = 0;
    if (outSize > 0) {
        int a = minBlock(notes);
        notesOut[0] = notes[a].noteBlock[notes[a].current];
        notesOut[0].duration *= (1 << a);

        initTime = notesOut[0].initTime * (1 << a);
        notesOut[0].initTime = 0;

        notes[a].current++;
    }

    for (unsigned int i = 1; i < outSize; i++) {
        int a = minBlock(notes);
        notesOut[i] = notes[a].noteBlock[notes[a].current];
        notesOut[i].duration <<= a;
        notesOut[i].initTime <<= a;
        notesOut[i].initTime -= initTime;
        notes[a].current++;
    }
}

int Notes::minBlock(NoteBlock notes[])
{
    int min = 0;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        assert(notes[i].current >= 0 && notes[i].current <= notes[i].size);

        if (notes[i].size == 0) {
            if (i == min) {
                min++;
            }
            continue;
        }
        if (notes[i].current == notes[i].size) {
            notes[i].current --;
            notes[i].noteBlock[notes[i].current].initTime = UINT_MAX >> i;
        }
        if (notes[i].noteBlock[notes[i].current].initTime << i <
                notes[min].noteBlock[notes[min].current].initTime << min) {
            min = i;
        }
    }

    return min;
}


void Notes::Block::peaksToNotes(vector<Note>& notes)
{
    int size = block.size();
    for (int nTime = 0; nTime < size; nTime++)
        for (int i = lastNote; i > firstNote; i--)
            if (block[nTime][i] > -INFINITY) {
                Note note;

                note.nNote = i;
                note.initTime = nTime;
                int dur = 1;
                while (nTime + dur < size &&
                       block[nTime + dur][i] > -INFINITY) {

                    block[nTime + dur][i] = -INFINITY;
                    dur++;
                }
                note.duration = dur;
                if (note.duration > MIN_DURATION) {
                    notes.push_back(note);
                }
            }

}

bool Notes::dump(ostream &fout)
{
    if (fout.fail()) {
        return false;
    }

    fout << "***" << " Notes Dump " << "***" << endl;

    unsigned int size = blocks[0].block.size();
    for(unsigned int ntime = 0; ntime < size; ntime++) {
        fout << "time = " << ntime << endl << endl;

        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {

            //chage if there is some application of typeFrame!!!
            if (ntime % (1 << i) == 0) {
                if (! blocks[i].dump(ntime >> i, fout)) {
                    return false;
                }
            }
        }
    }

    fout << "**********" << endl;
    fout << "**********" << endl;

    return true;
}

bool Notes::Block::dump(unsigned int ntime, ostream &fout)
{
    if (fout.fail()) {
        return false;
    }

    assert(ntime < block.size());

    fout << "first note: " << firstNote << " " << initNotes[firstNote] << endl;
    fout << "last note: " << lastNote << " " << initNotes[lastNote] << endl;
    fout << "frame size = " << frameSize << endl;
    fout << "time = " << ntime << endl;
    fout << "size of block = " << block.size() << endl;
    fout << "max ampl " << block[ntime].maxAmpl << endl;
    fout << endl;

    for (int i = firstNote; i <= lastNote; i++) {
        fout << block[ntime][i] << " ";
    }
    fout << endl << endl;
    return true;
}

bool Notes::dumpInitAmpl(ostream &fout)
{
    if (fout.fail()) {
        return false;
    }

    unsigned long size = amplTime.size();
    for (unsigned long int i = 0; i < size; i++)
        fout << amplTime[i] << " ";
    fout << endl;

    return true;
}
