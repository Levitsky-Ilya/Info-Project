#include <iostream>
#include <fstream>
#include <cstdlib>
#include <new>  ///NEEDED??????

#include "wav_sound.h"
#include "exception.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Usage: [filename]" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    try {
      WavFile wavFile;
      wavFile.initialize(argv[1]);


      cout << "  BitsPerSample: " << wavFile.getbitsPerSample() << endl;
      cout << "  Channels: "      << wavFile.getChannels() << endl;
      cout << "  Rate: "          << wavFile.getRate() << endl;

      vector<float> amplTime;
      wavFile.getAmplitudeArray(amplTime);
      wavFile.dumpVector(amplTime);
    }
    catch (Exception exception) {
      cout << "Error:" << exception.getErrorType() << endl;
      cout << exception.getErrorMessage() << endl;
      exit(EXIT_FAILURE);
    }
    catch (bad_alloc& ba)
    {
      cout << "bad_alloc caught: " << ba.what() << '\n';
      exit(EXIT_FAILURE);
    }
    return 0;
}
