#include "notes.h"
#include <thread>

const int DO[NUMBER_OF_BLOCKS] = {36, 24, 12, 0};

/*float & NotesList::operator [] (int n)
{
    return notesList[n];
}
*/

Notes::Notes(const char *fileName)
{
    for (int i = 0; i < NUMBER_OF_NOTES - 1; i++) {
        initDiffFreq[i] = initNotes[i+1] - initNotes[i];
        initDeltaFreq[i] = sqrt(initNotes[i+1] / initNotes[i]);
    }

    WavFile melody(fileName);
    melody.getAmplitudeArray(amplTime);

    int sampleRate = melody.getRate();
    unsigned int frameSize = 1;
    int firstNote = 0;
    //int channels = melody.getChannels();

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        while (sampleRate / frameSize > initDiffFreq[DO[i]]) {
            frameSize <<= 1;
        }
        frameSize >>= 1;

        blocks[i].frameSize = frameSize;
        blocks[i].diffFreq = sampleRate / frameSize;

        firstNote = DO[i];
        while(initDiffFreq[firstNote] < blocks[i].diffFreq)
            firstNote++;

        blocks[i].firstNote = firstNote;
    }

    blocks[0].lastNote = NUMBER_OF_NOTES - 1;
    blocks[1].lastNote = blocks[0].firstNote;
    blocks[2].lastNote = blocks[1].firstNote;
    blocks[3].lastNote = blocks[2].firstNote;
}

void Notes::generateMidView(vector<Note> & notesOut)
{
    //thread thr1(blocks[0].execute, this, ref(amplTime));
    //thr1.join();

    blocks[0].execute(amplTime, initDeltaFreq);

//there is a checking that function do something and may be correct:
    int prpr = 1;
    cout << blocks[0].firstNote << " " << initNotes[blocks[0].firstNote] << endl << endl;
    for (int i = blocks[0].firstNote; i < NUMBER_OF_NOTES; i++) {
        cout << blocks[0].block[prpr][i] << " ";
    }
    cout << endl << endl;

    /*for (int i = 41; i < NUMBER_OF_NOTES; i++) {
        cout << partSecond[0].notesList[i] << " ";
    }*/
    int i = maxNote(blocks[0].block[prpr]);
    cout << endl << "first max = " << initNotes[i] << endl;
    /*i = maxNote(partSecond[0]);
    cout << "second max = " << initNotes[i] << endl;*/
//***********

    return;
}

void Notes::Block::execute(const vector<float> & amplTime,
                           const float * const delta)
{
    array<float, NUMBER_OF_NOTES> amplNotes;
    CFFT fft;

    complex inFft[frameSize];
    float outFft[frameSize];
    size_t melodySize = amplTime.size();

    for (unsigned int i = 0; i < melodySize; i += frameSize) {

        for (unsigned int j = 0; j < frameSize; j++) {
            inFft[j] = complex(amplTime[i+j], 0.0);
        }

        fft.applyWindow(inFft, frameSize);
        fft.fftAlgorithm(inFft, outFft, frameSize);

        freqToNote(outFft, delta, amplNotes);

        block.push_back(amplNotes);
    }
}

void Notes::Block::freqToNote(const float * const outFft,
                              const float * const delta,
                              array<float, NUMBER_OF_NOTES> & notes)
{
    unsigned int outNum = 0;
    float freq = 0.0;
    // find the first number of frequence that can be compared with first note

    while(freq < initNotes[firstNote]) {
        freq += diffFreq;
        outNum++;
    }
    if (freq > initNotes[firstNote + 1]) {
        cout << "Wrong first note: " << initNotes[firstNote] << endl;
        exit(-1);
    }

    float deltaDown;
    notes.fill(-INFINITY); //there fill!!!

    int k = firstNote;
    for (unsigned int j = outNum; (j < frameSize) && (k < lastNote); j++) {

        deltaDown = freq - initNotes[k];

        if (deltaDown < delta[k]) {
            if (notes[k] < outFft[j])
                notes[k] = outFft[j];
        }
        else {
            if (notes[k+1] < outFft[j])
                notes[k+1] = outFft[j];
        }

        freq += diffFreq;
        if (freq > initNotes[k + 1]) {
            k++;
        }
    }
    return;
}

int Notes::maxNote(array<float, NUMBER_OF_NOTES> &ampl)
{
    int note = 0;
    for(int i = 0; i < NUMBER_OF_NOTES; i++) {
        if (ampl[i] > ampl[note])
            note = i;
    }
    return note;
}
