#ifndef WAV_SOUND_H
#define WAV_SOUND_H

class WavFile
{
public:
    WavFile ();
    ~WavFile ();
    int openWavFile ();
    int initialize ();
    int getSize ();
    int getAmplArray ();
    int closeWavFile ();
};
#endif // WAV_SOUND_H
