/**
 * wav_sound.cpp
 *
 * Description: WavFile class header.
 *
 * @author Ilya Levitsky ilya.levitskiy@phystech.edu
 * Copyright 2016
 **/

#ifndef WAV_SOUND_H
#define WAV_SOUND_H

#if 0
#include <cstdint>
#endif

#include <cstdio>
#include <vector>


using namespace std;

const long SIZE_OF_CHUNKID = 4;

struct RiffWaveHeader
{
    /** WAV-format begin with RIFF-header. **/

    // Containes symbols "RIFF" in ASCII.
    char chunkId[SIZE_OF_CHUNKID] = {0};

    // Size of file counting after this parameter.
    unsigned long chunkSize = 0;

    // Containes symbols "WAVE" in ASCII.
    char format[SIZE_OF_CHUNKID] = {0};

/** Format "WAVE" consists of subchunks. **/
};

struct FmtSubchunk
{
    /** FmtSubchunk begins with "fmt " symbols **/
    //char subchunk1Id[SIZE_OF_CHUNKID];          // Containes symbols "fmt "

    unsigned long subchunk1Size = 0;

    unsigned short audioFormat = 0;

    unsigned short numChannels = 0;

    unsigned long sampleRate = 0;

    unsigned long byteRate = 0;

    // sampleRate * numChannels * bitsPerSample/8

    // numChannels * bitsPerSample/8
    // Number of bytes in 1 sample, including all channels.
    unsigned short blockAlign = 0;

    unsigned short bitsPerSample = 0; // ("Depth" of sounding).
};

struct DataSubchunk
{
    /** DataSubchunk begins with "data" symbols **/
    //char subchunk2Id[SIZE_OF_CHUNKID];          // Containes symbols "data" (0x64617461)

    // Amount of bytes in data area.
    unsigned long subchunk2Size = 0;

    // Wav data are following.
};

class WavFile
{
public:
    WavFile ();
    ~WavFile ();
    void initialize(const char* fileName);
    unsigned short getRate();
    unsigned short getChannels();
    unsigned short getbitsPerSample();
    void dumpVector(vector<float> amplTime);
    void getAmplitudeArray (vector<float> &amplTime);
#if 0
    int getAmplFromStereo ();
#endif

private:
    FILE* file;
    RiffWaveHeader riffWaveHeader;
    FmtSubchunk fmtSubchunk;
    DataSubchunk dataSubchunk;
    string fileName_;
    unsigned long seekToData;

    void fillVector(vector<float> &amplTime);
    float strtoampl(const char* str, const unsigned short depth);
};

#endif // WAV_SOUND_H
