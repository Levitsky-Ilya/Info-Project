#include <iostream>
#include "notes.h"
#include "../exception.h"
#include "../exceptions.h"
#include <fstream>

//using namespace std;

int main()
{
    Notes perem;
    perem.setSilenceLavel(0.7);

    try {
    perem.initialize("C:\\Users\\Admin\\Downloads\\sounds\\Am-C7-Em-chiptune.wav");
    //perem.initialize("C:\\InfoProject\\Info-Project\\piano_A3.wav");
    } catch (NotesExceptions::Connect &e) {
        cout << e.what() << endl;
        return 0;
    } catch (Exception &e) {
        cout << e.getErrorMessage() << endl;
        return 0;
    } catch (...) {
        cout << "Sorry, failed initialize. That's all." << endl;
        return 0;
    }

    vector<Note> a;

    try {
    perem.generateMidView(a);
    } catch (...) {
        cout << "Сan't generate any notes. Sorry" << endl;
        return 0;
    }

    ofstream fout("mainDump.txt");

    int size = a.size();
    cout << size << endl;

    for(int i = 0; i < size; i++) {
        fout << "init Time " << a[i].initTime;
        fout << " note " << a[i].nNote;
        fout << " " << initNotes[a[i].nNote];
        fout << " duration " << a[i].duration << endl;
    }

    perem.dump(fout);
    fout.close();

    cout << "all is Ok!" << endl;
    return 0;
}
