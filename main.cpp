#include <iostream>
#include <cstring>
#include <windows.h>

const int SIZE_OF_FILE = 100; //For example

using namespace std;


//argv should have this \/ type for CreateFile() func, but warnings is shown!
//I don't think that argument is iterprerted right

int main(int argc, wchar_t* argv[])
//form of main {_tmain} SUPPORTS wchar_t as argument, but code doesn't compile that way:
/* int _tmain(int argc, wchar_t* argv[])   //  WinMain@16? */
{
    if(argc != 2)
    {
        cout << "Usage: [filename]" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    cout << "argument: "<< argv[1] << endl;
    HANDLE file; //HANDLE???
    errno_t err;

    /*
    err = fopen_s(&file, argv[1], "rb");
    if (err)
    {
      cout << "Failed open file " << argv[1] << " , error " << err << endl;
      //system("pause");
      exit(EXIT_FAILURE);
    }
    */

    //wchar_t ?
    //Info about function:
    //https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa363858(v=vs.85).aspx
    file = CreateFile  (argv[1],    // <- how to transform from char* to wchar_t*?
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_READONLY,
                        NULL);

    err = GetLastError();   //Returns error of functions realised in windows.h
    cout << "Creating file err: " << err << endl;

    //https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa366537(v=vs.85).aspx
    char* mapped = (char*)CreateFileMapping(file,
                                            NULL,
                                            PAGE_READONLY,
                                            0,
                                            SIZE_OF_FILE,
                                            NULL);
    err = GetLastError();
    cout << "File mapping err: " << err << endl;

    system("pause");
    return 0;
}

/** EVERYTHING BELOW IS TEMPORARE **/
/* Checke sizes*/
/*
Size of char     1 byte
Size of bool     1
Size of short    2
Size of int      4
Size of __int8   1
Size of __int16  2
Size of __int32  4
Size of __int64  8
Size of long     4
Size of long long    8
Size of float        4
Size of double       8
Size of long double  12
*/
