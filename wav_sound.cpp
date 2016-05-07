/**
 * wav_sound.cpp
 *
 * Description: WavFile class realiation.
 *              Stores info about file.
 *              Provides methods to recieve some info.
 *              Provides method to get vector<float> of ampitudes
 *              in range [-1; 1].
 *
 * @author Ilya Levitsky ilya.levitskiy@phystech.edu
 * Copyright 2016
 **/

#define DEBUG 0     //Set to 1 to display info while scanning the file
#define TIME 0      //Set to 1 to measure time spent on scanning
#define SHOW_AMPLITUDES 0

#include "exception.h"

/** WavFile header*/
#include "wav_sound.h"

/*#include <fstream> <- curently unused, old segment is commented.
 * DELETE??
 */

#include <thread>
#include <iostream>     //cout, endl
/**
 * When TIME == 1, time spent on reading the file and filling the vector
 * is measured.
 */
#include <chrono>
#include <new>          //bad_alloc

#include <math.h>       //ceil, floor.
#include <windows.h>    //included for mapping. Not used yet

using namespace std;

const int BITS_IN_BYTE = 8;
const long SIZE_OF_LONG = sizeof(unsigned long);
const int THREADS_MAX = thread::hardware_concurrency();

/** Constructor **/
WavFile::WavFile ()
{
    file = NULL;
    fileName_ = "";
    seekToData = 0;
}

/** Destructor **/
WavFile::~WavFile ()
{}

void WavFile::initialize (const char* fileName)
{
    errno_t err;

    fileName_ = fileName;

    err = fopen_s(&file, fileName, "rb");
    if (err)
    {
        string msgstr = "Failed to open filename \"" + fileName_
                + "\", error " + to_string(err);
        throw Exception(Exception::OPEN_FAIL, msgstr);
    }

    /* Reads header of .wav file, converts to struct RiffWaveHeader. */
    err = fread(&riffWaveHeader, sizeof(RiffWaveHeader), 1, file);
    if (err != 1) {
      if (feof(file)) {
        string msgstr = "EOF met too soon.";
        throw Exception(Exception::EOF_SOON, msgstr);
      } else {
        string msgstr = "Undefined error" + to_string(err)
                + "while reading file.";
        throw Exception(Exception::EREAD, msgstr);
      }
    }

    if (strnicmp(riffWaveHeader.chunkId, "RIFF",
                sizeof(riffWaveHeader.chunkId)) != 0)
    {
      string msgstr = "Is not RIFF sequence. (" +
              (string)riffWaveHeader.chunkId + ")";
      //msgstr += ")";
      throw Exception(Exception::BAD_RIFF, msgstr);
    }
    if (strnicmp(riffWaveHeader.format, "WAVE",
                sizeof(riffWaveHeader.format)) != 0)
    {
      string msgstr = "Is not WAVE format. ("
              + (string)riffWaveHeader.format + ")";
      //msgstr += ")";
      throw Exception(Exception::BAD_WAVE, msgstr);
    }

    char* subchunkId = new char[SIZE_OF_CHUNKID];
    unsigned long subchunkSize;
    bool reashedFmt = false;
    bool reashedData = false;

    seekToData = sizeof(RiffWaveHeader);

    while(!reashedData) {
        fread(subchunkId, SIZE_OF_LONG, 1, file);
#if DEBUG
        cout << endl << "subchunkId : "
             << subchunkId[0]
             << subchunkId[1]
             << subchunkId[2]
             << subchunkId[3] << "." << endl;
#endif
        if(strnicmp(subchunkId, "fmt ", SIZE_OF_CHUNKID) == 0)
        {
            fread(&fmtSubchunk, sizeof(FmtSubchunk), 1, file);
#if DEBUG
            cout << "FMT" << endl;
            cout << "  subchunk1Size: " << fmtSubchunk.subchunk1Size << endl;
            cout << "  audioFormat: "   << fmtSubchunk.audioFormat   << endl;
            cout << "  numChannels: "   << fmtSubchunk.numChannels   << endl;
            cout << "  sampleRate: "    << fmtSubchunk.sampleRate    << endl;
            cout << "  byteRate: "      << fmtSubchunk.byteRate      << endl;
            cout << "  blockAlign: "    << fmtSubchunk.blockAlign    << endl;
            cout << "  bitsPerSample: " << fmtSubchunk.bitsPerSample << endl;
            system("pause");
#endif
            err = fseek(file, fmtSubchunk.subchunk1Size - 16, SEEK_CUR);
            if (err)
            {
              string msgstr = "Fseek error " +  to_string(err) + " in fmt.";
              throw Exception(Exception::SEEK_FAIL, msgstr);
            }
            seekToData += fmtSubchunk.subchunk1Size;
            reashedFmt = true;
        } else
        if(strnicmp(subchunkId, "data", SIZE_OF_CHUNKID) == 0)
        {
            if (!reashedFmt)
            {
              string msgstr = "No fmt subchunk found before data.";
              throw Exception(Exception::NO_FMT, msgstr);
            }
            fread(&dataSubchunk, sizeof(DataSubchunk), 1, file);

            reashedData = true;
#if DEBUG
            cout << "DATA" << endl;
            cout << " subchunk2Size: " << dataSubchunk.subchunk2Size << endl;
            cout << " seekToData: "
                 << seekToData + SIZE_OF_CHUNKID + SIZE_OF_LONG << endl;
            system("pause");
#endif
        } else {
            fread(&subchunkSize, sizeof(unsigned long), 1, file);
#if DEBUG
            cout << "subchunkSize: " << subchunkSize << endl;
            system("pause");
#endif
            err = fseek(file, subchunkSize, SEEK_CUR);
            if (err)
            {
              string msgstr = "Fseek error " + to_string(err) + " in some subchunk.";
              throw Exception(Exception::SEEK_FAIL, msgstr);
            }
            seekToData += subchunkSize;
        }
        seekToData += (SIZE_OF_CHUNKID + SIZE_OF_LONG);
    }

    fclose(file);
    delete [] subchunkId;
}

unsigned short WavFile::getRate()
{
    return fmtSubchunk.sampleRate;
}

unsigned short WavFile::getChannels()
{
    return fmtSubchunk.numChannels;
}

unsigned short WavFile::getbitsPerSample()
{
    return fmtSubchunk.bitsPerSample;
}

/** This version of getAmplitudeArray(..) is multithreaded.
 *  It's out is vector of one channel's amlitudes.
 *  fread(..) from file to big buffer, from buffer get amplitudes.
 */
void WavFile::getAmplitudeArray (vector<float> &amplTime)
{
    if(fmtSubchunk.bitsPerSample % BITS_IN_BYTE != 0 ||
       fmtSubchunk.bitsPerSample > sizeof(int) * BITS_IN_BYTE ||
       fmtSubchunk.bitsPerSample < sizeof(char) * BITS_IN_BYTE)
    {
      string msgstr = "Unexpected depth of sounding: "
              + to_string(fmtSubchunk.bitsPerSample);
      throw Exception(Exception::UNEXP_DEPTH, msgstr);
    }

#if TIME
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
#endif

    fillVector(amplTime);

#if TIME
    end = chrono::system_clock::now();
    chrono::duration<double> spent_time = end - start;
    cout << "Completed for " << spent_time.count() << "s." << endl;
#endif
}

/** Input: any vector to replace its content
 *  Algorithm: Creates buffer sized of whole data.
 *             Copies data from file to buffer.
 *             Reads from buffer and fills vector
 *             with amplitudes calculated by strtoampl(..).
 *  Output: given vector is filled with amplitudes.
 */
void WavFile::fillVector (vector<float> &amplTime)
{
    errno_t err;
    err = fopen_s(&file, fileName_.c_str(), "rb");
    if (err)
    {
      string msgstr = "fillVector failed to open filename \"" + fileName_
              + "\", error " + to_string(err);
      throw Exception(Exception::OPEN_FAIL, msgstr);
    }

    err = fseek(file, seekToData, SEEK_SET);
    if (err)
    {
      string msgstr = "Fseek error " + to_string(err)
              + "when seeked to data in fillVector";
      throw Exception(Exception::SEEK_FAIL, msgstr);
    }

    unsigned short depth = fmtSubchunk.bitsPerSample;
    unsigned long sizeOfData = dataSubchunk.subchunk2Size;
    char* buff = new char[sizeOfData];
    unsigned long blockAlign = fmtSubchunk.blockAlign;
    unsigned long nBlocks = sizeOfData/blockAlign;

    amplTime.clear();
    amplTime.resize(nBlocks);

    fread(buff, sizeOfData, 1, file);
    fclose(file);

    thread thids[THREADS_MAX - 1];
    for (int i = 0; i < THREADS_MAX - 1; i++)
      thids[i] = thread(fillVectorConcurr, this, buff, ref(amplTime),
                        nBlocks, blockAlign, depth, i);

    fillVectorConcurr(buff, amplTime,
                      nBlocks, blockAlign, depth, THREADS_MAX - 1);

    for (int i = 0; i < THREADS_MAX - 1; i++)
      thids[i].join();

    delete [] buff;
}

/** Input: buffer to read data from,
 *         vector to fill with amplitudes,
 *         index of thread.
 *  Algorithm: each thread reads from buffer and fills vector.
 *         Threads are working independantly, each with it own offset(index).
 *  Output: given vector has his elements with a certain offset
 *         filled with amplitudes.
 */
void* WavFile::fillVectorConcurr (char* buff,
                                  vector<float>& amplTime,
                                  unsigned long nBlocks,
                                  unsigned long blockAlign,
                                  unsigned short depth,
                                  unsigned long index)
{
    float tmpAmplitude;
    for(unsigned long i = index; i < nBlocks; i+= THREADS_MAX) {
      tmpAmplitude = strtoampl(buff + i*blockAlign, depth);
      amplTime[i] = tmpAmplitude;
    }
    return NULL;
}

/** Additional method to print content of vector*/
void WavFile::dumpVector(vector<float> amplTime)
{
    long sizeOfVector = amplTime.size();
    cout << "*** DUMP VECTOR ***" << endl;
    cout << "Elemets in vector: " << sizeOfVector << endl;
    cout << "Input No to display this element from vector." << endl;
    cout << "Enter negative No to exit." << endl;

    long i;
    cin >> i;
    while (i >= 0) {
        if (i < sizeOfVector)
          cout << amplTime[i] << endl;
        else
          cout << "Too big No. Elements total: " << sizeOfVector << endl;
        cin >> i;
    }
}

/** Input: str - pointer to first element of buffer to be read,
 *         depth - depth of sound.
 *  Output: Amplitude in range [-1; 1].
 */
float WavFile::strtoampl(const char* str, const unsigned short depth)
{
    int maxAmplitude = (depth==32? 0x7fffffff :
                                 depth==24? 0x7fffff :
                                 depth==16? 0x7fff : 0xff);
    union {
       int amplitude;
       char data[4];
    } a;
    a.amplitude = 0;
    unsigned short steps = depth / sizeof(char) / 8;
    for (unsigned i = 0; i < steps; i++)
        a.data[i] = str[i];

    if (a.amplitude > maxAmplitude)
       a.amplitude -= 2*maxAmplitude;   //simulating setting negative bit

#if SHOW_AMPLITUDES
    //this section might not work properly if executed concurrently!
    cout << a.data[0] << '.' <<
            a.data[1] << '.' <<
            a.data[2] << '.' <<
            a.data[3] << ". " << a.amplitude << ' ';
#endif
    float famplitude = (float)a.amplitude / maxAmplitude;
    return famplitude;
}
