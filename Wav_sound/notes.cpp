#include "notes.h"
#include <thread>
#include <assert.h>
#include <fstream>

const int NOTE_C[NUMBER_OF_BLOCKS] = {24, 12, 0};

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
    //blocks[3].lastNote = blocks[2].firstNote;
}

void Notes::generateMidView(vector<Note> & notesOut)
{
    thread thr0(blocks[0].execute, &(blocks[0]), ref(amplTime), ref(initDeltaFreq));
    thread thr1(blocks[1].execute, &(blocks[1]), ref(amplTime), ref(initDeltaFreq));
    thread thr2(blocks[2].execute, &(blocks[2]), ref(amplTime), ref(initDeltaFreq));
    //thread thr3(blocks[3].execute, &(blocks[3]), ref(amplTime), ref(initDeltaFreq));

    thr0.join();
    thr1.join();
    thr2.join();
    //thr3.join();

    /*for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].execute(amplTime, initDeltaFreq);
    }*/

    dump();
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

void Notes::dump()
{
    ofstream fout("dump.txt");
    int size = blocks[0].block.size();
    int ntime = 0;
    for(ntime = 0; ntime < size; ntime++) {
        fout << "time = " << ntime << endl << endl;
        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
            blocks[i].dump(ntime / (1 << i), fout);
        }
        int i = maxNote(blocks[0].block[ntime]);
        fout << endl << "first max: " << i << " " << initNotes[i] << endl;
    }

    //cout << "first ampl: " << amplTime[0] << endl;
    //unsigned int size = amplTime.size() - 1;
    //cout << "last ampl: " << amplTime[size] << endl << endl;

    /*for (int i = 0; i < 20; i++)
        cout << amplTime[i] << " ";
    cout << endl << endl;
    for (unsigned int i = size; i > size - (blocks[0].frameSize); i--)
        cout << amplTime[i] << " ";
    cout << endl;*/

    fout.close();
}

void Notes::Block::dump(int ntime, ostream &fout)
{
    fout << "first note: " << firstNote << " " << initNotes[firstNote] << endl;
    fout << "last note: " << lastNote << " " << initNotes[lastNote] << endl;
    fout << "time = " << ntime << endl;
    fout << "size of block = " << block.size() << endl;
    fout << endl;
    for (int i = firstNote; i <= lastNote; i++) {
        //assert(ntime < block.size());
        fout << block[ntime][i] << " ";
        //cout.flush();
    }
    fout << endl << endl;
}
