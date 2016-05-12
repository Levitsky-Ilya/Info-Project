#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <iostream>
#include <string>
#include <array>
#include "wav_sound.h"
#include "fft.h"
#include "frequencies_for_notes.h"

//using namespace std;

class Exception
{
public:
    enum ErrType {
        OPEN_FAIL,      //Failed to open file.
        SEEK_FAIL,      //Failed to seek in file.
        EOF_SOON,       //Found EOF too soon while reading file.
        EREAD,          //Error while reading file.
        NO_FMT,         //No fmt subchunk found.
        BAD_RIFF,       //No RIFF label found.
        BAD_WAVE,       //No WAVE label found.
        UNEXP_DEPTH,    //Unexpected depth of sound.
    };
    Exception(ErrType errCode, string& errMessage) :
        errType(errCode),
        errMessage(errMessage)
    {}
    ~Exception() {}
    ErrType getErrorType() { return errType; }
    string getErrorMessage() { return errMessage; }
protected:
    ErrType errType;
    string errMessage;
};

#endif //MYEXCEPTION_H
