#ifndef WAV_SOUND_H
#define WAV_SOUND_H

#include<iostream>
#include<vector>

class WavFile
{
public:
    WavFile ();
    ~WavFile ();
    int openWavFile (const char* fileName);
    int initialize ();
    int getAmplArray (std::vector<int> &ampl);
    int getTimeDiff ();
    int closeWavFile ();
private:
    int diffTime;
};

struct AmplFrecArray
{
    std::vector<int> ampl;
    int diffFrec;
    int diffTime;
    int ntime;
};

AmplFrecArray DpfTransform (const std::vector<int> &ampl, int diffTime, int sizeWindow);


#endif // WAV_SOUND_H
