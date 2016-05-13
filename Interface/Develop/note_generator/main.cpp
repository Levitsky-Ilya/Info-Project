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
						 "etude.wav");
	}
	catch (Exception & e) {
		cout << e.getErrorMessage() << endl;
	}

	vector<Note> noteVect;
	notes.setSilenceLevel(0.5);
	notes.generateMidView(noteVect);

	Queue noteVectL(noteVect, true);
	Queue noteVectN(noteVect, false);

	noteVectN.breaker();
	noteVectL.breaker();

	ofstream file("E:/Programs/Lilypond/file.ly");

	file << "\\header {title = " << "\"etude.wav\"" << "}\n";
	file << "\\score { \n";
	file << "\\new PianoStaff << \n";

	if (noteVectN.size() != 0) {
		file << "\\new Staff { \n";
		noteVectN.drawStaff(noteVectL, file);
	}

	if (noteVectL.size() != 0) {
		file << "\\new Staff { \n";
		file << "\\clef \"bass\" \n";
		noteVectL.drawStaff(noteVectN, file);
	}

	file << ">> \n";
	file << "} \n";

	file.close();

	cout << "Assemble complete" << endl;

	return 0;
}
