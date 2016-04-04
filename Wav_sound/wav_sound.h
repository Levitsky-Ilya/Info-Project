#ifndef WAV_SOUND_H
#define WAV_SOUND_H

#include<iostream>
#include<vector>

using namespace std;

class WavFile
{
public:
    WavFile (const char* filename);
    ~WavFile ();
    int getRate();
    int getChannels();
    int getAmplitudeArray (vector<double> &amplTime);
#if 0
    int getAmplFromStereo ();
#endif

private: // Add more info
    const char* fileName;
    int sampleRate;
    int channels;
};

#endif // WAV_SOUND_H
