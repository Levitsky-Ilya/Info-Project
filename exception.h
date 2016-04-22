#include <iostream>
#include <string>

using namespace std;

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
        NO_EMPTY_VECTOR //Vector given to fillVector is not empty.
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
