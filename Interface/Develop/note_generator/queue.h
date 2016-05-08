#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <../additional_files/notes.h>

class Queue : public vector<struct Note>
{
	public:
/* Constructor */
	Queue(const vector<Note> & noteVect, bool isLow);
/* Destructor */
	~Queue();
/*
 * This function breaks all notes in vector so that it becomes possible to make
 * chords from them. Also it breaks long notes by beats.
 */
	vector<struct Note> breaker();

/*
 * This function fills file with all necessary text
 * for correct music display.
 */
	void drawStaff(const Queue & comparedVect, ofstream & file);

/*
 * This function is responsible for putting
 * everything in the file in the right order.
 */
	void drawNote(ofstream & file);

/* Checking pause at the beginning of first music beat */
	void firstPauseChecker(ofstream & file);

/* Writes information about note/notes in chord into file */
	void printChord(vector<int> & lastChordFreqs, size_t & i,
			int & combNum, ofstream & file);

/* Writes duration of notes into file */
	void printDuration(string & name, size_t & i, ofstream & file);

/* Writes pauses if next note is in different tact */
	void printNextBeatPause(vector<int> & lastChordFreqs,
			size_t & i, int & combNum, ofstream & file);

/*
 * Writes pauses if next note is in same tact or
 * timing between is less than 1/16
 */
	void printBeatPause(vector<int> & lastChordFreqs,
			size_t & i, int & combNum, ofstream & file);

/* Actions at the end of staff */
	void checkEnd(size_t & i, ofstream & file);

	const float BIT = 0.03125;
};

#endif // QUEUE_H
