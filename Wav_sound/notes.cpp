#include "notes.h"
#include <fstream>
#include <thread>

/*
 * Read initial frequances from file to FreqList
 */
float & NotesList::operator [] (int n)
{
    return notesList[n];
}

Notes::Notes()
{
    ifstream fin("Frequences_for_notes.txt");
    float freq;

    if (fin.is_open()) {
        while(fin >> freq) {
            FreqList.push_back(freq);
        }
    }
    else {
        cout << "ERROR: no such note file!" << endl;
        exit(-1);
    }
    if(FreqList.size() != NUMBER_OF_NOTES) {
        cout << "ERROR: note file is damaged" << endl;
        exit(-1);
    }

    fin.close();
}

void Notes::generateMidView(const char *fileName)
{
    WavFile melody(fileName);
    vector<float> amplTime;

    int channels = melody.getChannels();
    int sampleRate = melody.getRate();

    if ((channels == 1)&&(sampleRate == 44100)) {
        melody.getAmplitudeArray(amplTime);

        //there I try to use threads, but it's only example to compare fft
        //with and without nulls
        thread thr1(executeBlock, this, 2048, sampleRate, 41,
                    NUMBER_OF_NOTES - 1, WITHOUT_NULLS, ref(amplTime));
        thread thr2(executeBlock, this, 4096, sampleRate, 41,
                    NUMBER_OF_NOTES - 1, WITH_NULLS, ref(amplTime));

        thr1.join();
        thr2.join();
        //***************

//there is a checking that function do something and may be correct:
        for (int i = 41; i < NUMBER_OF_NOTES; i++) {
            cout << partFirst[0].notesList[i] << " ";
        }
        cout << endl << endl;

        for (int i = 41; i < NUMBER_OF_NOTES; i++) {
            cout << partSecond[0].notesList[i] << " ";
        }
        int i = maxNote(partFirst[0]);
        cout << endl << "first max = " << FreqList[i] << endl;
        i = maxNote(partSecond[0]);
        cout << "second max = " << FreqList[i] << endl;
//***********
    }

    return;
}

void Notes::executeBlock(unsigned int frameSize, int sampleRate,
                         int firstNote, int lastNote, TypeFrame typeFrame,
                         vector<float> &amplTime)
{
    NotesList amplNotes;
    amplNotes.numFirstNote = firstNote;
    amplNotes.numLastNote = lastNote;
    amplNotes.diffFreq = sampleRate/frameSize;

    CFFT fft;

    complex inFft[frameSize];
    float outFft[frameSize];
    size_t melodySize = amplTime.size();

    if (typeFrame == WITH_NULLS) {
        frameSize /= 2;
    }

    for (unsigned int i = 0; i < melodySize; i += frameSize) {

        if (typeFrame == WITHOUT_NULLS) {
            for (unsigned int j = 0; j < frameSize; j++) {
                inFft[j] = complex(amplTime[i+j], 0.0);
            }
        }
        else {
            for (unsigned int j = 0; j < frameSize; j++) {
                inFft[j >> 1] = complex(amplTime[i+(j >> 1)], 0.0);
                inFft[(j >> 1) + 1] = complex(0.0, 0.0);
            }
        }

        fft.applyWindow(inFft, frameSize);
        fft.fftAlgorithm(inFft, outFft, frameSize);

        freqToNote(outFft, frameSize, amplNotes);

//!!! there are solution only for an example with first block!!!
        if (typeFrame == WITHOUT_NULLS) {
            partFirst.push_back(amplNotes);
        }
        else {
            partSecond.push_back(amplNotes);
        }
    }
}

void Notes::freqToNote(float * const outFft, int num, NotesList &notes)
{
    int outNum = 0;
    float freq = 0.0;
    // find the first number of frequence that can be compared with first note

    while(freq < FreqList[notes.numFirstNote]) {
        freq += notes.diffFreq;
        outNum++;
    }
    if (freq > FreqList[notes.numFirstNote + 1]) {
        cout << "Wrong first note: " << notes.numFirstNote << endl;
        exit(-1);
    }

    float delta, deltaDown;

    for (int j = 0; j < NUMBER_OF_NOTES; j++) {
        notes.notesList[j] = -INFINITY; // attention!!!
    }

    int k = notes.numFirstNote;
    for (int j = outNum; j < num && k < notes.numLastNote; j++) {
        delta = sqrt(FreqList[k+1] / FreqList[k]);
        deltaDown = freq - FreqList[k];

        if (deltaDown < delta) {
            if (notes[k] < outFft[j])
                notes[k] = outFft[j];
        }
        else {
            if (notes[k+1] < outFft[j])
                notes[k+1] = outFft[j];
        }

        freq += notes.diffFreq;
        if (freq > FreqList[k + 1]) {
            k++;
        }
    }
    return;
}

int Notes::maxNote(NotesList& ampl)
{
    int note = 0;
    for(int i = 0; i < NUMBER_OF_NOTES; i++) {
        if (ampl.notesList[i] > ampl.notesList[note])
            note = i;
    }
    return note;
}
