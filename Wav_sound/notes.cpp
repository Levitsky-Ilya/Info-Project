#include "notes.h"
#include <thread>
#include <assert.h>

const int NOTE_C[NUMBER_OF_BLOCKS] = {36, 24, 12, 0};

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
        while (sampleRate / (frameSize << 1) > initDiffFreq[NOTE_C[i]]) {
            frameSize <<= 1;
        }
        blocks[i].frameSize = frameSize;
        blocks[i].diffFreq = sampleRate / frameSize;

        firstNote = NOTE_C[i];
        while(initDiffFreq[firstNote + 1] < blocks[i].diffFreq) //attention!!! I use previous note!
            firstNote++;

        blocks[i].firstNote = firstNote;
        blocks[i].outNum = (int)(initNotes[firstNote] / blocks[i].diffFreq);
        blocks[i].outNum++;
    }

    blocks[0].lastNote = NUMBER_OF_NOTES - 1;
    blocks[1].lastNote = blocks[0].firstNote;
    blocks[2].lastNote = blocks[1].firstNote;
    blocks[3].lastNote = blocks[2].firstNote;
}

void Notes::generateMidView(vector<Note> & notesOut)
{
    int ntime = 0;
    //thread thr1(blocks[0].execute, this, ref(amplTime));
    //thr1.join();

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].execute(amplTime, initDeltaFreq);
    }

//there is a checking that function do something and may be correct:
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].dump(ntime);
    }
    int i = maxNote(blocks[0].block[ntime]);
    cout << endl << "first max = " << initNotes[i] << endl;
//***********

    return;
}

void Notes::Block::execute(const vector<float> & amplTime,
                           const float * const delta)
{
    array<float, NUMBER_OF_NOTES> amplNotes;
    amplNotes.fill(-INFINITY);

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

void Notes::Block::dump(int ntime)
{
    cout << "first note: " << firstNote << " " << initNotes[firstNote] << endl;
    cout << "last note: " << lastNote << " " << initNotes[lastNote] << endl;
    cout << endl;

    for (int i = firstNote; i <= lastNote; i++) {
        //assert(ntime < block.size());
        cout << block[ntime][i] << " ";
        //cout.flush();
    }
    cout << endl << endl;
}

void Notes::Block::freqToNote(const float * const outFft,
                              const float * const delta,
                              array<float, NUMBER_OF_NOTES> & notes)
{
    float freq = outNum * diffFreq;
    float deltaDown = 0.0;

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
