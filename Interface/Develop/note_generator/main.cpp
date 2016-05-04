#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <functional>
#include <algorithm>
#include <map>
#include <../additional_files/notes.h>
#include <../additional_files/wav_sound.h>
#include <../additional_files/frequencies_for_notes.h>
#include <../additional_files/fft.h>
#include <../additional_files/complex.h>
#include <../additional_files/exception.h>
#include <freq_names.h>
#include <note_pauses.h>
#include <global.h>
#include <queue.h>

using namespace std;

int main()
{
	Notes notes;
	try {
		notes.initialize("E:/Programs/Qt/Projects/note_generator/"
						 "C-E-G-E-C-G-E-D.wav");
	}
	catch (Exception & e) {
		cout << e.getErrorMessage() << endl;
	}

	vector<Note> noteVect;
	notes.generateMidView(noteVect);

	Queue noteVectL(noteVect, true);
	Queue noteVectN(noteVect, false);

	noteVectN.breaker();
	noteVectL.breaker();

	ofstream f("E:/Programs/Lilypond/file.ly");

	f << "normal = \\new Staff { \n";

	noteVectN.drawStaff(noteVectL, f);

	f << "bass = \\new Staff { \n";
	f << "\\clef \"bass\" \n";

	noteVectL.drawStaff(noteVectN, f);

	f << "{\n";
	f << "\\new PianoStaff << \n";
	f << "\\normal \n";
	f << "\\bass \n";
	f << ">> \n";
	f << "} \n";

	f.close();

	return 0;
}
