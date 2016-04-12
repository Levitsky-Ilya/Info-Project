#include <iostream>
#include <fstream>
#include <cstdlib>

#include <wav_sound.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Usage: [filename]" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    WavFile wavFile(argv[1]);
    cout << "bitsPerSample: " << wavFile.getbitsPerSample() << endl;
    cout << "Channels: "      << wavFile.getChannels() << endl;
    cout << "Rate: "          << wavFile.getRate() << endl;
    vector<float> amplTime;
    wavFile.getAmplitudeArray(amplTime);
    wavFile.dumpVector(amplTime);
    system("pause");
    return 0;
}
