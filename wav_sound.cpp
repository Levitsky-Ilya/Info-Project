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

#include "exceptions.h"

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
const unsigned long PAGE_SIZE = 4096;
const unsigned long SIZE_OF_LONG = sizeof(unsigned long);
const int THREADS_MAX = thread::hardware_concurrency();

/** Constructor **/
WavFile::WavFile ()
{
    file = nullptr;
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
        string msg = "Failed to open filename \"" + fileName_
                + "\", error " + to_string(err);
        throw NotesExceptions::OpenFile(msg);
    }

    /* Reads header of .wav file, converts to struct RiffWaveHeader. */
    err = fread(&riffWaveHeader, sizeof(RiffWaveHeader), 1, file);
    if (err != 1) {
      if (feof(file)) {
        string msg = "EOF met too soon.";
        throw NotesExceptions::FileDamaged(msg);
      } else {
        string msg = "Undefined error" + to_string(err)
                + "while reading file.";
        throw NotesExceptions::ReadFile(msg);
      }
    }

    if (strnicmp(riffWaveHeader.chunkId, "RIFF",
                sizeof(riffWaveHeader.chunkId)) != 0)
    {
      string msg = "Is not RIFF sequence. (" +
              (string)riffWaveHeader.chunkId + ")";
      throw NotesExceptions::FalseFormat(msg);
    }
    if (strnicmp(riffWaveHeader.format, "WAVE",
                sizeof(riffWaveHeader.format)) != 0)
    {
      string msg = "Is not WAVE format. ("
              + (string)riffWaveHeader.format + ")";
      throw NotesExceptions::FalseFormat(msg);
    }

    char subchunkId[SIZE_OF_CHUNKID];
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
#endif
        err = fseek(file, fmtSubchunk.subchunk1Size - 16, SEEK_CUR);
        if (err)
        {
          string msg = "Fseek error " +  to_string(err) + " in fmt.";
          throw NotesExceptions::FileDamaged(msg);
        }
        seekToData += fmtSubchunk.subchunk1Size;
        reashedFmt = true;
      } else if(strnicmp(subchunkId, "data", SIZE_OF_CHUNKID) == 0) {
        if (!reashedFmt)
        {
          string msg = "No fmt subchunk found before data.";
          throw NotesExceptions::FileDamaged(msg);
        }
        fread(&dataSubchunk, sizeof(DataSubchunk), 1, file);

        reashedData = true;
#if DEBUG
        cout << "DATA" << endl;
        cout << " subchunk2Size: " << dataSubchunk.subchunk2Size << endl;
        cout << " seekToData: "
             << seekToData + SIZE_OF_CHUNKID + SIZE_OF_LONG << endl << endl;
#endif
      } else {
        fread(&subchunkSize, sizeof(unsigned long), 1, file);
#if DEBUG
        cout << "subchunkSize: " << subchunkSize << endl;
#endif
        err = fseek(file, subchunkSize, SEEK_CUR);
        if (err)
        {
          string msg = "Fseek error " + to_string(err) + " in some subchunk.";
          throw NotesExceptions::FileDamaged(msg);
        }
        seekToData += subchunkSize;
      }
      seekToData += (SIZE_OF_CHUNKID + SIZE_OF_LONG);
    }
    fclose(file);
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
      string msg = "Unexpected depth of sounding: "
              + to_string(fmtSubchunk.bitsPerSample);
      throw NotesExceptions::UnexpDepth(msg);
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
    unsigned long sizeOfData = dataSubchunk.subchunk2Size;
    char* buff = new(nothrow) char[sizeOfData];
    if(buff == nullptr) {
      fillVectorBySegments(amplTime);
      delete [] buff;
      return;
    }

    errno_t err;
    err = fopen_s(&file, fileName_.c_str(), "rb");
    if (err) {
      string msg = "fillVector failed to open filename \"" + fileName_
              + "\", error " + to_string(err);
      throw NotesExceptions::OpenFile(msg);
    }

    err = fseek(file, seekToData, SEEK_SET);
    if (err) {
      string msg = "Fseek error " + to_string(err)
              + "when seeked to data in fillVector";
      throw NotesExceptions::FileDamaged(msg);
    }

    /* Whole file consists of blocks. */
    unsigned short depth = fmtSubchunk.bitsPerSample;
    unsigned long blockAlign = fmtSubchunk.blockAlign;
    unsigned long nBlocks = sizeOfData / blockAlign;

    amplTime.clear();
    amplTime.resize(nBlocks);

#if DEBUG
    cout << "DEBUG OF FILL_VECTOR"        << endl;
    cout << "  depth: "       << depth      << endl;
    cout << "  sizeOfData: "  << sizeOfData << endl;
    cout << "  blockAlign: "  << blockAlign << endl;
    cout << "  nBlocks: "     << nBlocks    << endl << endl;
#endif

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

/** This function is called if allocating memory for whole file
 * was unsuccessful. This algorithm is using less memory,
 * trying to allocate it by segments, until they less than a page.
 *  Input: any vector to replace its content
 *  Algorithm: Creates buffer sized of one segment.
 *             Copies data from file to buffer.
 *             Reads from buffer and fills vector
 *             with amplitudes calculated by strtoampl(..).
 *             Read file that way till remainder inclusive.
 *  Output: given vector is filled with amplitudes.
 */
void WavFile::fillVectorBySegments (vector<float> &amplTime)
{
    /* Whole file now is divided to segments with equal size and one remainder.
     * Each of them consists of blocks.
     */
    unsigned short depth = fmtSubchunk.bitsPerSample;
    unsigned long blockAlign = fmtSubchunk.blockAlign;
    unsigned long sizeOfData = dataSubchunk.subchunk2Size;
    unsigned long nBlocksTotal = sizeOfData / blockAlign;
    unsigned long segmentsTotal = 4;

    unsigned long nBlocks = nBlocksTotal / segmentsTotal;
    unsigned long nBlocksRemain = nBlocksTotal % segmentsTotal;
    unsigned long segmentSize = nBlocks * blockAlign;
    unsigned long remainderSize = nBlocksRemain * blockAlign;

    //Next part is trying to allocate memory for buffer, exception otherwise.
    char* buff = nullptr;
    bool newSuccess = false;
    while(newSuccess != true)
    {
      if (segmentSize < PAGE_SIZE) {
        string msg = "Memory allocation failed in fillVectorBySegments.";
        throw NotesExceptions::BadAlloc(msg);
      }
      char* buff = new(nothrow) char[segmentSize];
      if(buff != nullptr)
        newSuccess = true;
      else {
        segmentsTotal *= 2;
        nBlocks = nBlocksTotal / segmentsTotal;
        nBlocksRemain = nBlocksTotal % segmentsTotal;
        segmentSize = nBlocks * blockAlign;
        remainderSize = nBlocksRemain * blockAlign;
      }
    }

    errno_t err;
    err = fopen_s(&file, fileName_.c_str(), "rb");
    if (err)  {
      string msg = "fillVector failed to open filename \"" + fileName_
              + "\", error " + to_string(err);
      throw NotesExceptions::OpenFile(msg);
    }

    err = fseek(file, seekToData, SEEK_SET);
    if (err) {
      string msg = "Fseek error " + to_string(err)
              + "when seeked to data in fillVector";
      throw NotesExceptions::FileDamaged(msg);
    }

    amplTime.clear();
    amplTime.resize(nBlocks);

#if DEBUG
    cout << "DEBUG OF FILL_VECTOR_BY_SEGMENTS"    << endl;
    cout << "  depth: "         << depth         << endl;
    cout << "  blockAlign: "    << blockAlign    << endl;
    cout << "  sizeOfData: "    << sizeOfData    << endl;
    cout << "  nBlocksTotal: "  << nBlocksTotal  << endl;
    cout << "  segmentsTotal: " << segmentsTotal << endl;

    cout << "  nBlocks: "       << nBlocks       << endl;
    cout << "  nBlocksRemain: " << nBlocksRemain << endl;
    cout << "  segmentSize: "   << segmentSize   << endl;
    cout << "  remainderSize: " << remainderSize << endl << endl;
#endif

    for(unsigned long segment = 0; segment <= segmentsTotal; segment++)
    {
      if(segment == segmentsTotal && remainderSize != 0) {
      /* If some data left in remainder of size less than segment. */
          segmentSize = remainderSize;
          nBlocks = nBlocksRemain;
      }
      fread(buff, segmentSize, 1, file);

      thread thids[THREADS_MAX - 1];
      for (int i = 0; i < THREADS_MAX - 1; i++)
        thids[i] = thread(fillVectorConcurr, this, buff, ref(amplTime),
                          nBlocks, blockAlign, depth, i);

      fillVectorConcurr(buff, amplTime,
                        nBlocks, blockAlign, depth, THREADS_MAX - 1);

      for (int i = 0; i < THREADS_MAX - 1; i++)
        thids[i].join();
    }

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
    return nullptr;
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
    cout << a.data[0] << '.'
         << a.data[1] << '.'
         << a.data[2] << '.'
         << a.data[3] << ". " << a.amplitude << ' ';
#endif
    float famplitude = (float)a.amplitude / maxAmplitude;
    return famplitude;
}
