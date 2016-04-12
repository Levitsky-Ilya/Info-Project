#include <iostream>
#include "notes.h"
#include <fstream>

//using namespace std;

int main()
{
    Notes perem("C:\\Users\\Admin\\Downloads\\sounds\\etude.wav");
    vector<Note> a;
    perem.generateMidView(a);

    ofstream fout("mainDump.txt");

    int size = a.size();
    cout << size << endl;

    for(int i = 0; i < size; i++) {
        fout << "init Time " << a[i].initTime;
        fout << " note " << a[i].nNote;
        fout << " duration " << a[i].duration << endl;
    }

    perem.dump(fout);
    fout.close();

    cout << "all is Ok!" << endl;
    return 0;
}
