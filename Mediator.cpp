/**
 * Mediator.cpp
 *
 * Description: Input  - path to WAVE file.
 *              Output - short description of header and length of audio.
 *
 * @author Ilya Levitsky ilya.levitskiy@phystech.edu
 * Copyright 2016
 **/


#include <iostream>
#include <iomanip>
#include <math.h>
#include <assert.h>

using namespace std;

struct WAVHEADER
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

    // numSamples * numChannels * bitsPerSample/8
    // Amount of bytes in data area.
    unsigned long subchunk2Size;

    // Wav data are following.
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Wrong amount of arguments. Must be 1 - path to file" << endl;
        system("pause");
        return -1;
    }

    FILE *file;
    errno_t err;
    err = fopen_s(&file, argv[1], "rb");
    if (err)
    {
      cout << "Failed open file" << argv[1] << " , error " << err << endl;
      system("pause");
      return -1;
    }

    WAVHEADER header;

    /* Reads header of .wav file, converts to struct WAVHEADER.
     * Returnes 1 if success.
     */
    err = fread(&header, sizeof(WAVHEADER), 1, file);
    if (err != 1) {
      if(feof(file)) {
        cout << "EOF met too soon." << endl;
      } else {
        cout << "Undefined error while reading file." << endl;
      }
      system("pause");
      return -1;
    }

    cout << "Sample rate:" << header.sampleRate << endl;
    cout << "Channels:" << header.numChannels << endl;
    cout << "Bits per sample:" << header.bitsPerSample << endl;

    // Calculating duration of playing in seconds
    float DurationSeconds = 1.0f * header.subchunk2Size
                             /(header.bitsPerSample / 8)
                             / header.numChannels / header.sampleRate;
    int DurationMinutes = (int)floor(DurationSeconds) / 60;
    DurationSeconds = DurationSeconds - (DurationMinutes * 60);

    cout << "Duration: ";

    cout.width(2);
    cout.fill('0');

    cout << DurationMinutes  << ":";

    cout.precision(4);
    cout.width(3);
    cout.fill('0');
    //!!! INCORRECT OUT WHEN TIME < 10sec.

    cout << DurationSeconds << endl;

  //  printf_s("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

    fclose(file);

    system("pause");
    return 0;
}
