#include "notes.h"
#include <fstream>

/*
 * Read definite frequances from file to notesFreq
 */
Notes::Notes()
{
    ifstream fin("Frequences_for_notes.txt");
    double freq;

    if (fin.is_open()) {
        while(fin >> freq) {
            notesFreqList.push_back(freq);
        }
    }
    else {
        cout << "ERROR: no such note file!" << endl;
        exit(-1);
    }
    if(notesFreqList.size() != NUMBER_OF_NOTES) {
        cout << "ERROR: note file is damaged" << endl;
        exit(-1);
    }

    fin.close();
}

void Notes::generateMidView(const char *fileName)
{
    WavFile melody(fileName);
    vector<double> amplTime;
    size_t melodySize;

    int channels = melody.getChannels();
    int sampleRate = melody.getRate();

    if ((channels == 1)&&(sampleRate == 44100)) {
        melody.getAmplitudeArray(amplTime);

        melodySize = amplTime.size();

        /*
         * first block: from fa of 1st octave to si 4th octave
         */
        const unsigned int FRAME_SIZE = 2048; // there sampleRate must be 44100. Definitely.
        NotesList amplNotes;
        amplNotes.numFirstNote = 41; // is "fa" really 41??
        amplNotes.numLastNote = NUMBER_OF_NOTES - 1;
        amplNotes.diffFreq = sampleRate/FRAME_SIZE;

        CFFT fft;

        complex inFft[FRAME_SIZE];
        double outFft[FRAME_SIZE];

        for (unsigned int i = 0; i < melodySize; i += FRAME_SIZE) {

            for (unsigned int j = 0; j < FRAME_SIZE; j++) {
                inFft[j] = complex(amplTime[i+j], 0.0);
            }

            fft.fftAlgorithm(inFft, outFft, FRAME_SIZE); //Ira's part is here

            freqToNote(outFft, FRAME_SIZE, amplNotes);

            partOne.push_back(amplNotes);
        }
//there is a checking that function do something and may be correct:
        for (int i = 0; i < NUMBER_OF_NOTES; i++) {
            cout << partOne[0].notesList[i] << " ";
        }
        cout << endl;
//***********
    }

    return;
}

void Notes::freqToNote(double * const outFft, int num, NotesList &notes)
{
    int outNum = 0;
    double freq = 0.0;
    // find the first number of frequence that can be compared with first note

    while(freq < notesFreqList[notes.numFirstNote]) {
        freq += notes.diffFreq;
        outNum++;
    }
    if (freq > notesFreqList[notes.numFirstNote + 1]) {
        cout << "Wrong first note: " << notes.numFirstNote << endl;
        exit(-1);
    }

    double deltaUp, deltaDown;

    for (int j = 0; j < NUMBER_OF_NOTES; j++) {
        notes.notesList[j] = -INFINITY; // attention!!!
    }

    int k = notes.numFirstNote;
    for (int j = outNum; j < num && k < notes.numLastNote; j++) {
        deltaUp = notesFreqList[k+1] - freq;
        deltaDown = freq - notesFreqList[k];

        if (deltaDown < deltaUp) {
            if (notes.notesList[k] < outFft[j])
                notes.notesList[k] = outFft[j];
        }
        else {
            if (notes.notesList[k+1] < outFft[j])
                notes.notesList[k+1] = outFft[j];
        }

        freq += notes.diffFreq;
        if (freq > notesFreqList[k + 1]) {
            k++;
        }
    }
    return;
}
