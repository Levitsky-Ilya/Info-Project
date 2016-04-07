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

/** WavFile header*/
#include "wav_sound.h"

/*#include <fstream> <- curently unused, old segment is commented.
 * DELETE??
 */
/*#include <iomanip> <- curently unused, needed for correct display of duration.
 * NEEDED?
 */

#include <iostream>     //cout, endl
/**
 * When TIME == 1, time spent on reading the file and filling the vector
 * is measured.
 */
#include <chrono>

#include <math.h>       //ceil, floor.
#include <windows.h>    //included for mapping. Not used yet

using namespace std;

const int BITS_IN_BYTE = 8;
const int SECONDS_IN_MINUTE = 60;
const int PAGE_SIZE = 4096;
const int THREE_QUART_PAGE_SIZE = 3072;

//Try to use mapping!!
//CreateFileMapping(file, NULL, PAGE_READONLY, 0, subchunk2Size, NULL)

/** Constructor **/
WavFile::WavFile (const char* fileName)
{
    errno_t err;

    fileName_ = new char[strlen(fileName) + 1];
    err = strcpy_s(fileName_, strlen(fileName) + 1, fileName);
    if (err)
    {
      cout << "Failed save filename \"" << fileName << "\" , error " << err << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }
    err = fopen_s(&file, fileName, "rb");
    if (err)
    {
      cout << "Failed open file" << fileName << " , error " << err << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }

    /* Reads header of .wav file, converts to struct WavHeader. */
    err = fread(&header, sizeof(WavHeader), 1, file);
    if (err != 1) {
      if(feof(file)) {
        cout << "EOF met too soon." << endl;
      } else {
        cout << "Undefined error while reading file." << endl;
        /** Can/Should we define?!!! */
      }
      system("pause");
      exit(EXIT_FAILURE);
    }

    /* Checking whether is Wav file or not. TRY TO throw exception if fault.*/
    if(strnicmp(header.chunkId, "RIFF", sizeof(header.chunkId)) != 0) {
      cout << sizeof(header.chunkId) << endl;
      cout << "Is not RIFF sequence. (" << header.chunkId << ")" << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }
    if(strnicmp(header.format, "WAVE", sizeof(header.format)) != 0) {
      cout << "Is not WAVE format. (" << header.format << ")" << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }
    if(strnicmp(header.subchunk1Id, "fmt ", sizeof(header.subchunk1Id)) != 0) {
      cout << sizeof(header.subchunk1Id) << endl;
      cout << "Couldn't find fmt subchunk, found:"
              " (" << header.subchunk1Id << ")" << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }
    if(strnicmp(header.subchunk2Id, "data",  sizeof(header.subchunk2Id)) != 0) {
      cout << "Could not find data subchunk, found:"
              " (" << header.subchunk2Id << ")" << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }
    
    /* Calculating duration of playing in min:sec. */
    AllDurationSeconds = 1.0f * header.subchunk2Size
                             /(header.bitsPerSample / BITS_IN_BYTE)
                             / header.numChannels / header.sampleRate;
    DurationMinutes = (int)floor(AllDurationSeconds) / SECONDS_IN_MINUTE;
    DurationSeconds = AllDurationSeconds - (DurationMinutes * SECONDS_IN_MINUTE);

    fclose(file);
}

/** Destructor **/
WavFile::~WavFile ()
{
    delete fileName_;
}

unsigned short WavFile::getRate()
{
    return header.sampleRate;
}

unsigned short WavFile::getChannels()
{
    return header.numChannels;
}

unsigned short WavFile::getbitsPerSample()
{
    return header.bitsPerSample;
}

/** This version of getAmplitudeArray(..) is single threaded.
 *  It's out is vector of one channel's amlitudes.
 *  fread(..) from file to big buffer, from buffer get amplitudes.
 */
void WavFile::getAmplitudeArray (vector<float> &amplTime)
{
    /*
    ifstream file(fileName_);
    streambuf* pbuf = file.rdbuf();
    unsigned long nSamples = header.subchunk2Size/header.blockAlign;
    unsigned short channels = getChannels();
    */

    /*
    if (channels == 1)
      union Block {
        unsigned int sample;
        char buffer[5];
      } block;
    else if(channels == 2)
      union Block {
        __int32 sample;
        char buffer[5];
      } block;
    else assert(!"No so much samples");
    */

    /*
    union Block {
      unsigned short sample;
      char buffer[2 + 1] = {0};     //change 2 to bytesPerSample!
    } block;
    unsigned short bytesPerSample = header.bitsPerSample/BITS_IN_BYTE;
    pbuf->pubseekpos(sizeof(WavHeader));

    cout << "Samples " << nSamples << endl;
    system("pause");

    for(unsigned int i = 0; i < nSamples; i++) {

      pbuf->sgetn(block.buffer, bytesPerSample);
      cout << "Sample " << block.sample << endl;
      //cout << "Buffer " << block.buffer << endl;
      amplTime.push_back(block.sample);
      cout << "i " << i << endl;
      if(i == nSamples/2) system("pause");

      for(unsigned short j = 1; j < channels; j++)
        pbuf->sgetn(block.buffer, bytesPerSample);

    }
    */

#if TIME
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();
#endif

    if(header.bitsPerSample % BITS_IN_BYTE != 0 ||
       header.bitsPerSample > sizeof(int) * BITS_IN_BYTE ||
       header.bitsPerSample < sizeof(char) * BITS_IN_BYTE)
    {
        //Throw exception if maxAmplitude == 0xff and depth !=8 ????
       cout << "Unexpected depth of sounding: "
            << header.bitsPerSample << endl;
        exit(EXIT_FAILURE);
    }

    fillVector(amplTime);

#if TIME
    end = chrono::system_clock::now();
    chrono::duration<double> spent_time = end - start;
    cout << "Completed for " << spent_time.count() << "s." << endl;
#endif
}

/** Input: empty vector
 *  Algorithm: Creates buffer sized about one page.
 *             Divides file into pages.
 *             Reads from buffer and fills vector
 *             with amplitudes calculated by strtoampl(..).
 *  Output: given vector is filled with amplitudes.
 */
void WavFile::fillVector (vector<float> &amplTime)
{
    errno_t err;
    err = fopen_s(&file, fileName_, "rb");
    if (err)
    {
      cout << "Failed open file" << fileName_ << " , error " << err << endl;
      system("pause");
      exit(EXIT_FAILURE);
    }

    fseek(file, sizeof(WavHeader), SEEK_SET);

    unsigned short depth = header.bitsPerSample;
    unsigned short pageSize = (depth == 24? THREE_QUART_PAGE_SIZE : PAGE_SIZE);

    char* buff = new char[pageSize];
    unsigned long sizeOfFile = header.subchunk2Size;
    unsigned long pagesTotal = sizeOfFile / pageSize;
    unsigned long sizeOfFileRemainder = sizeOfFile % pageSize;
    unsigned long blockAlign = header.blockAlign;

#if DEBUG
    cout << "DEBUG" << endl;
    cout << "depth: " << depth << endl;
    cout << "pageSize: " << pageSize << endl;
    cout << "sizeOfFile: " << sizeOfFile << endl;
    cout << "pagesTotal: " << pagesTotal << endl;
    cout << "sizeOfFileRemainder: " << sizeOfFileRemainder << endl;
    cout << "blockAlign: " << blockAlign << endl << endl;
    system("pause");
#endif

    float tmpAmplitude;
    for(unsigned long page = 0; page < pagesTotal; page++)
    {
      fread(buff, pageSize, 1, file);
      /* Reading only first channel. */
      for(unsigned long i = 0; i < pageSize; i += blockAlign) {
#if DEBUG
        cout << "Page " << page << " out of " << pagesTotal << ".\t";
        cout << "At " << i << " out of " << pageSize  << ".\t";
#endif

        tmpAmplitude = strtoampl(buff + i, depth); //Good buff + i???
        amplTime.push_back(tmpAmplitude);

#if DEBUG
        cout << tmpAmplitude << endl;
#endif
      }
    }
    /* Read remaining part of file less then a page*/
    if(sizeOfFileRemainder != 0)
      fread(buff, sizeOfFileRemainder, 1, file);
      for(unsigned long i = 0; i < sizeOfFileRemainder; i += blockAlign) {
#if DEBUG
        cout << "At " << i << " out of " << sizeOfFileRemainder  << ".\t";
#endif

        tmpAmplitude = strtoampl(buff + i, depth);
        amplTime.push_back(tmpAmplitude);

#if DEBUG
        cout << tmpAmplitude << endl;
#endif
      }
    fclose(file);
    delete [] buff;
}

/** Additional method to print content of vector*/
void WavFile::dumpVector(vector<float> amplTime)
{
    cout << "Elemets in vector: " << amplTime.size() << endl;
    cout << "Input No to display this element from vector." << endl;
    cout << "Enter negative No to exit." << endl;
    system("pause");
    /*
    for (vector<float>::iterator it = amplTime.begin();
         it != amplTime.end();
         it++)
    {
      cout << *it << '\t';
    }
    */
    long i;
    cin >> i;
    while (i >= 0) {
        cout << amplTime[i] << endl;
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

#if DEBUG
    cout << a.data[0] << '.' <<
            a.data[1] << '.' <<
            a.data[2] << '.' <<
            a.data[3] << ". " << a.amplitude << ' ';
#endif
    float famplitude = (float)a.amplitude / maxAmplitude;
    return famplitude;
}
