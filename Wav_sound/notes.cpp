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

void Notes::generateMidView(vector<Note>& notesOut)
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

/****************************/
    ofstream fout("dump.txt");
    dump(fout);
    fout.close();
/***************************/

    keepOnlyPeaks();

    notesFromPeaks(notesOut);

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

void Notes::keepOnlyPeaks()
{
    int size = blocks[0].block.size();
    for (int i = 0; i < size; i++) {

        complementBlocks(i);

        //after this cycle first notes of elder blocks are peaks:
        for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
            blocks[j].keepOnlyPeaks(i >> j); // bad!!!
        }
        //because of it we need this function:
        checkPeaks(i);
    }
}

void Notes::complementBlocks(int nTime)
{
    for (int i = 0; i < NUMBER_OF_BLOCKS - 1; i++) {
        if (blocks[i].block[nTime >> i][blocks[i].firstNote] <
                blocks[i + 1].block[nTime >> (i+1)][blocks[i + 1].lastNote]) {
            blocks[i].block[nTime >> i][blocks[i].firstNote] =
                    blocks[i + 1].block[nTime >> (i+1)][blocks[i + 1].lastNote];
        }
    }
}

void Notes::checkPeaks(int nTime)
{
    for (int i = 1; i < NUMBER_OF_BLOCKS; i++) {
        if (blocks[i - 1].block[nTime >> (i - 1)][blocks[i - 1].firstNote + 1] >
                blocks[i].block[nTime >> i][blocks[i].lastNote]) {

            blocks[i].block[nTime >> i][blocks[i].lastNote] = -INFINITY;
        }
    }
    complementBlocks(nTime);
}

void Notes::Block::keepOnlyPeaks(int nTime)
{
    for (int i = lastNote; i > firstNote; i--) {
        if (block[nTime][i] <= PEAK_MINIMUM ||
                block[nTime][i] < block[nTime][i - 1]) {
            block[nTime][i] = -INFINITY; // there is no 'seconds' in melody!!!
        }
    }
}

void Notes::notesFromPeaks(vector<Note>& notesOut)
{
    NoteBlock notes[NUMBER_OF_BLOCKS];

    /*thread thr0(blocks[0].peaksToNotes, &(blocks[0]), ref(notes[0].noteBlock));
    thread thr1(blocks[1].peaksToNotes, &(blocks[1]), ref(notes[1].noteBlock));
    thread thr2(blocks[2].peaksToNotes, &(blocks[2]), ref(notes[2].noteBlock));
    //thread thr3(blocks[3].peaksToNotes, &(blocks[3]), ref(notes[3].noteBlock));

    thr0.join();
    thr1.join();
    thr2.join();
    //thr3.join();
*/
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        blocks[i].peaksToNotes(notes[i].noteBlock);
    }

    int outSize = 0;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        notes[i].current = 0;
        notes[i].size = notes[i].noteBlock.size();
        outSize += notes[i].size;
    }

    //notesOut.reserve(outSize);

    for (int i = 0; i < outSize; i++) {
        int a = minBlock(notes);
        //notesOut[i] = notes[a].noteBlock[notes[a].current];
        notesOut.push_back(notes[a].noteBlock[notes[a].current]);
        notes[a].current++;
    }
}

int Notes::minBlock(NoteBlock notes[])
{
    int min = 0;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        if (notes[i].current == notes[i].size) {
            /*notes[i].current --;
            notes[i].noteBlock[notes[i].current].initTime = INFINITY;*/
            continue;
        }
        if (notes[i].noteBlock[notes[i].current].initTime <
                notes[min].noteBlock[notes[min].current].initTime) {
            min = i;
        }
    }

    //notes[min].current++;

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
                note.initTime = nTime / diffFreq; // there check types!!! (later)

                int dur = 1;
                while (nTime + dur < size &&
                       block[nTime + dur][i] > -INFINITY) {

                    block[nTime + dur][i] = -INFINITY;
                    dur++;
                }
                note.duration = dur / diffFreq; // attettion to types too

                notes.push_back(note);
            }

}

/*int Notes::maxNote(array<float, NUMBER_OF_NOTES> &ampl)
{
    int note = 0;
    for(int i = 0; i < NUMBER_OF_NOTES; i++) {
        if (ampl[i] > ampl[note])
            note = i;
    }
    return note;
}
*/

void Notes::dump(ostream &fout)
{
    fout << "***" << " Notes Dump " << "***" << endl;
    int size = blocks[0].block.size();
    int ntime = 0;
    for(ntime = 0; ntime < size; ntime++) {
        fout << "time = " << ntime << endl << endl;
        for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
            blocks[i].dump(ntime >> i, fout);
        }
        //int i = maxNote(blocks[0].block[ntime]);
        //fout << endl << "first max: " << i << " " << initNotes[i] << endl;
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

    fout << "**********" << endl;
    fout << "**********" << endl;
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
