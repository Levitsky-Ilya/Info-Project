#include <iostream>
#include <string>

using namespace std;

class Exception
{
public:
    enum ErrType {
        SAVE_FAIL,
        OPEN_FAIL,
        EOF_SOON,
        EREAD,
        BAD_RIFF,
        BAD_WAVE,
        BAD_FMT,
        BAD_DATA,
        UNEXP_DEPTH
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
