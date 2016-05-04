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

	Queue(const vector<Note> & noteVect, bool isLow);
	~Queue();
/*
 * This function breaks all notes in vector so that
 * it becomes possible to make chords from them.
 * Also this function breaks long notes by beats.
 */
		vector<struct Note> breaker();

/*
 * This function is responsible for putting
 * everything in the file in the right order.
 */
		void drawNote(ofstream & f);

		void drawStaff(const Queue & comparedVect, ofstream & f);
};

#endif // QUEUE_H

