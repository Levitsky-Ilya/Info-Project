#ifndef WAV_SOUND_H
#define WAV_SOUND_H

#include <iostream>
#include <vector>

using namespace std;

class WavFile
{
public:
    WavFile ();
    ~WavFile ();
    int open (const char* fileName);
    int initialize ();
    int getAmplArray ();
    int DpfTransform (int sizeFrame, int nTime, double initFrec); //Arguments will be modified
    int close ();

    AmplFreqArray DpfResult;
private:
    vector<int> ampl;
    double diffTime;
};

struct AmplFreqArray
{
    vector<int> ampl;
    double diffFreq;
    double diffTime;
    double initFreq;
    int ntime;
};

struct Note
{
    double frec;
    double duration;
    double initTime;
};

class Notes
{
public:
    int generateMidView(const char* fileName);
    //vector<Note> octaveSmall;
    //vector<Note> octaveGreat;
    vector<Note> octave1;
    vector<Note> octave2;
    vector<Note> octave3;
};

#endif // WAV_SOUND_H
