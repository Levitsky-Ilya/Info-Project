/**
 * wav_sound.cpp
 *
 * Description: !!!
 *
 * @author Ilya Levitsky ilya.levitskiy@phystech.edu
 * Copyright 2016
 **/

#define DEBUG 0

#include <wav_sound.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <math.h>
#include <assert.h>
#include <windows.h>

using namespace std;

const int BITS_IN_BYTE = 8;

//Try to use mapping!!
//CreateFileMapping(file, NULL, PAGE_READONLY, 0, subchunk2Size, NULL)

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
    DurationMinutes = (int)floor(AllDurationSeconds) / 60;
    DurationSeconds = AllDurationSeconds - (DurationMinutes * 60);

    fclose(file);
}

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
 *  Reading from directly from file?, because sys/mmap.h is unavailible!!!
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

    switch (header.bitsPerSample)
    {
    case 32:
        fillVector<int> (amplTime);
        break;

    case 16:
        fillVector<short> (amplTime);
        break;

    case 8:
        fillVector<char> (amplTime);
        break;

    default:
        cout << "Unexpected depth of sounding: "
             << header.bitsPerSample << endl;
    }
}


template <class T>
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

    unsigned short channels = header.numChannels;
    unsigned short depth = header.bitsPerSample;
    T amplitude;
    float tmpAmplitude;
    unsigned int maxAmplitude = (depth==32? 0x7fffffff :
                                            depth==16? 0x7fff : 0xff);
    unsigned long BlocksTotal = header.subchunk2Size/header.blockAlign;

    /* Reading only first channel. */
    for(unsigned long i = 0; i < BlocksTotal; i++) {
      #if DEBUG
      cout << i << " out of " << BlocksTotal << ' ';
      #endif

      fread(&amplitude, sizeof(T), 1, file);
      tmpAmplitude = (float)amplitude / maxAmplitude;
      amplTime.push_back(tmpAmplitude);
      err = fseek(file, sizeof(T) * (channels - 1), SEEK_CUR);

      #if DEBUG
      cout << tmpAmplitude << endl;
      #endif
    }
}


void WavFile::dumpVector(vector<float> amplTime)
{
    cout << amplTime.size() << endl;
    system("pause");
    for (vector<float>::iterator it = amplTime.begin();
         it != amplTime.end();
         it++)
    {
      cout << *it << '\t';
    }
}
