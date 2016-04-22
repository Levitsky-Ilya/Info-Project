#include <iostream>
#include <string>

using namespace std;

class Exception
{
public:
    enum ErrType {
        SAVE_FAIL,
        OPEN_FAIL,
        SEEK_FAIL,
        EOF_SOON,
        EREAD,
        NO_FMT,
        BAD_RIFF,
        BAD_WAVE,
        UNEXP_DEPTH,
        NO_EMPTY_VECTOR
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
