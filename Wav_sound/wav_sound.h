#ifndef WAV_SOUND_H
#define WAV_SOUND_H

#include<iostream>
#include<vector>

class WavFile
{
public:
    WavFile ();
    ~WavFile ();
    int open (const char* fileName);
    int initialize ();
    int getAmplArray ();
    int DpfTransform (int sizeFrame, int nTime, double initFrec); //Arguments will be modify
    int close ();

    AmplFrecArray DpfResult;
private:
    std::vector<int> ampl;
    double diffTime;
};

struct AmplFrecArray
{
    std::vector<int> ampl;
    double diffFrec;
    double diffTime;
    double initFrec;
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
    //std::vector<Note> octaveSmall;
    //std::vector<Note> octaveGreat;
    std::vector<Note> octave1;
    std::vector<Note> octave2;
    std::vector<Note> octave3;
};

#endif // WAV_SOUND_H
