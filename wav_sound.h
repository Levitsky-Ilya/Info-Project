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

struct WavHeader
{
    // WAV-format begin with RIFF-header:

    // Containes symbols "RIFF" in ASCII.
    // (0x52494646 in big-endian)
    char chunkId[4];

    // Size of file counting after this parameter.
    unsigned long chunkSize;

    /* Containes symbols "WAVE" in ASCII.
     * (0x57415645 в big-endian)
     */
    char format[4];

/** Format "WAVE" consists of 2 subchunks: "fmt " & "data": **/

/** First subchunk - fmt; declares format of audio data. **/
    char subchunk1Id[4];          // Containes symbols "fmt" (0x666d7420)

    // Size of subchunk, beginning right after this (16 for PCM).
    unsigned long subchunk1Size;

    unsigned short audioFormat;

    unsigned short numChannels;

    unsigned long sampleRate;

    unsigned long byteRate;       // sampleRate * numChannels * bitsPerSample/8

    // numChannels * bitsPerSample/8
    // Number of bytes in 1 sample, including all channels.
    unsigned short blockAlign;

    unsigned short bitsPerSample; // ("Depth" of sounding).

/** Second subchunk - data; containes audio data and size. **/

    char subchunk2Id[4];          // Containes symbols "data" (0x64617461)

    // Amount of bytes in data area.
    unsigned long subchunk2Size;

    // Wav data are following.
};

class WavFile
{
public:
    WavFile (const char *fileName);
    ~WavFile ();
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
    WavHeader header;
    char* fileName_;
    float AllDurationSeconds;

    void fillVector(vector<float> &amplTime);
    float strtoampl(const char* str, const unsigned short depth);

    int DurationMinutes;
    float DurationSeconds;
};

#endif // WAV_SOUND_H
