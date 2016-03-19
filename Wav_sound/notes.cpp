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
        cout << "ERROR: no such file!" << endl;
        exit(-1);
    }
    if(notesFreqList.size() != NUMBER_OF_NOTES) {
        cout << "ERROR: notes file is damaged" << endl;
        exit(-1);
    }

    fin.close();
}

int Notes::generateMidView(const char *fileName)
{
    WavFile melody(fileName);
    vector<double> amplTime;
    size_t melodySize;

    int channels = melody.getChannels();
    int sampleRate = melody.getRate();

    if (channels == 1) {
        melody.getAmplitudeArray(amplTime);

        melodySize = amplTime.size();

        /*
         * first block: from fa of 1st octave to si 4th octave
         */
        const int FRAME_SIZE = 2048; // there sampleRate must be 44100. Definitely.
        const int FA_FIRST = 41; // is "fa" really 41??
        const double diffFreq = sampleRate/FRAME_SIZE;

        vector<double> inFft(FRAME_SIZE);
        vector<double> outFft;
        NotesList amplNotes;

        for (unsigned int i = 0; i < melodySize; i += FRAME_SIZE) {

            for (int j = 0; j < FRAME_SIZE; j++) {
                inFft[j] = amplTime[i+j];
            }

            fftAlgorithm(inFft, outFft); // there is 100% wrong name!

            int outFreqNum = 0;
            while(diffFreq*outFreqNum < notesFreqList[FA_FIRST])
                outFreqNum++;
            if (diffFreq*outFreqNum > notesFreqList[FA_FIRST + 1]) {
                cout << "fignya with fa." << endl;
                exit(-1);
            }
/*
 * next code is bred of ill imaginary
 * I try to turn freq-ampl into note-ampl
 * for each note choose the max
 * but it's first part, for first fa, so notes bellow must be with "0"
 */
            for (int j = 0; j < FA_FIRST; j++) {
                amplNotes.notesList[j] = 0;
            }
            for (int j = FA_FIRST; j < NUMBER_OF_NOTES; j++) {
                double freq = diffFreq*outFreqNum;
                double deltaUp = notesFreqList[j+1] - freq;
                double deltaDown = freq - notesFreqList[j];

                if (deltaDown < deltaUp) {
                    amplNotes.notesList[j] = outFft[outFreqNum];
                    outFreqNum++;
                    cout << amplNotes.notesList[j] << endl;
                }
                else {
                    amplNotes.notesList[j] = outFft[outFreqNum - 1]; // wrong! wrong! wrong!
                }
            }

        }
    }

    return 0;
}
