#include <iostream>
#include <fstream>
#include <cstdlib>

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
      WavFile wavFile(argv[1]);

      cout << "bitsPerSample: " << wavFile.getbitsPerSample() << endl;
      cout << "Channels: "      << wavFile.getChannels() << endl;
      cout << "Rate: "          << wavFile.getRate() << endl;

      vector<float> amplTime;
      wavFile.getAmplitudeArray(amplTime);
      wavFile.dumpVector(amplTime);
    }
    catch (Exception exception) {
        cout << "Error:" << exception.getErrorType() << endl;
        cout << exception.getErrorMessage() << endl;
        exit(EXIT_FAILURE);
    }
    system("pause");
    return 0;
}
