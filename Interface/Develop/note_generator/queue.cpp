#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include "../additional_files/notes.h"
#include "queue.h"
#include "note_pauses.h"
#include "note_durations.h"
#include "freq_names.h"
#include "global.h"
//==============================================================================
Queue::Queue(const vector<Note> & noteVect, bool isLow) {
	if (isLow) {
		for (size_t i = 0; i < noteVect.size(); i++) {
			if (noteVect[i].nNote < 36)
				push_back(noteVect[i]);
		}
	} else {
		for (size_t i = 0; i < noteVect.size(); i++) {
			if (noteVect[i].nNote >= 36)
				push_back(noteVect[i]);
		}
	}
}
//==============================================================================
Queue::~Queue() {}
//==============================================================================
vector<struct Note> Queue::breaker()
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	list<struct Note> noteList;
	struct Note noteAdded;

/* Creating list with time of notes' "starts", "ends"
 * and also with all integer values of time.
 */
	list<float> noteTiming;
	for (size_t i = 0; i < queue.size(); i++ ) {
		noteTiming.push_back(queue[i].initTime);
		noteTiming.push_back(queue[i].initTime + queue[i].duration);
	}
	for (float bar = 1; bar <= 1000; bar++) {
		noteTiming.push_back(bar);
	}
	noteTiming.sort();
	noteTiming.unique();

/* "Converting" vector to list */
	while(!queue.empty()) {
		noteList.push_front(queue.back());
		queue.pop_back();
	}

/*
 * Comparing quantities from noteTiming with
 * current note "start" and "end". Creating additional note if necessary
 * and shortening the note from this additional note have been created.
 */
	for (auto it2 = noteTiming.begin(); it2 != noteTiming.end(); ++it2){
		for (auto it1 = noteList.begin(); it1 != noteList.end(); ++it1 ) {
			if ((it1->initTime < *it2)&&
					((it1->initTime + it1->duration) > *it2)) {
				noteAdded.duration = it1->initTime + it1->duration - *it2;
				noteAdded.initTime = *it2;
				noteAdded.nNote = it1->nNote;

				it1->duration = *it2 - it1->initTime;

				noteList.insert(it1, noteAdded);
			}
		}
	}

/* "Converting" list to vector */
	while (!noteList.empty()) {
		queue.push_back(noteList.front());
		noteList.pop_front();
	}

	sort(queue.begin(), queue.end());

	return queue;

}
//==============================================================================
void Queue::drawStaff (const Queue & comparedVect, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/* Drawing pauses at the beginning fot the same length of staffs*/
	int beginPauseCounter = 0;
	if ((queue.size() != 0)&&(comparedVect.size() != 0)) {
		while ((int)queue[0].initTime - (int)comparedVect[0].initTime -
			   beginPauseCounter >= 1) {
			file << "r1 ";
			beginPauseCounter++;
		}
/* We have to make new vector without these pauses for algorythm to work */
		for (size_t i = 0; i < queue.size(); i++) {
			queue[i].initTime -= beginPauseCounter;
		}
	}

	drawNote(file);

/* Drawing pauses in the end for the same length of staffs */
	int endPauseCounter = 0;
	if ((comparedVect.size() != 0)&&(queue.size() != 0)) {
		while ((int)(comparedVect[comparedVect.size()-1].initTime +
				comparedVect[comparedVect.size()-1].duration) -
				(int)(queue[queue.size()-1].initTime +
				queue[queue.size()-1].duration) -
				endPauseCounter >= 1) {
			file << "r1 ";
			endPauseCounter++;
		}
	}

	file << "\n";
	file << "}\n";
}
//==============================================================================
void Queue::drawNote(ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	deleteSupersmall();
	firstPauseChecker(file);

/*
 * lastChordFreqs[] and freq_array[] (in printChord function) are used
 * to check if chords nearby have at lest one same note. In that case,
 * there will be additional "(" and ")" to connect chords on display.
 */
	vector<int> lastChordFreqs;

	int combNum = 0; //for tracing an amount of notes taken at single step

	for (size_t i = 0; i < queue.size(); ) {

		float length = printChord(lastChordFreqs, i, combNum, file, length);
/*
 * Check if note is in different beat and the difference
 * in timing is more than 1/16
 */
		if (((int)(queue[i+combNum].initTime)!=
				(int)(queue[i].initTime + queue[i].duration))&&
				(queue[i+combNum].initTime - queue[i].initTime -
				queue[i].duration > 2*BIT)) {
			printNextBeatPause(lastChordFreqs, length, i, combNum, file);
		} else {
			printBeatPause(lastChordFreqs, length, i, combNum, file);
			checkEnd(length, i, file);
		}
		i = i + combNum;
	}
}
//==============================================================================
void Queue::deleteSupersmall()
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	for (unsigned int i = 0; i < queue.size(); i++) {
		if (queue[i].duration < BIT) {
			for (size_t num = i; num < queue.size() - 1; num++) {
				queue[num] = queue[num+1];
			}
			queue.pop_back();
		}
	}
}
//==============================================================================
void Queue::firstPauseChecker(ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	if (queue.size() != 0) {
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (near(it->pauseDur, queue[0].initTime, BIT, BIT)) {
				file << it->pauseName;
			}
		}
	}
}
//==============================================================================
float Queue::printChord(vector<int> & lastChordFreqs, size_t & i,
		int & combNum, ofstream & file, float &length)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
//Will be cleared if it's not a chord
	string name = "< ";
	vector<int> freqArray;
/* Checking if we have chord and, if so, putting it in freqArray */
	int k = 0;
	while (((i+k) < queue.size())&&
		  (queue[i+k].initTime == queue[i].initTime)&&
		   (queue[i+k].duration >= BIT)) {
		freqArray.push_back(queue[i+k].nNote);
		k++;
	}
/*
 * lastChordFreqs storing frequencies for previous chords. We compare it and
 * freqArray. If we find same notes, we need to use "(" and ")" in text file
 * to place the slur. flag is used to place ")" then time the comes.
 */
	bool flag = true;
	for (size_t m = 0; flag && m < freqArray.size(); m++) {
		for (size_t n = 0; flag && n < lastChordFreqs.size(); n++)	{
			if (lastChordFreqs[n] == freqArray[m]) {
				file << "( ";
				flag = false;
			}
		}
	}
/*
* Frequecies of chord are stored between "<" and ">", so we place them there.
* Single note is just being written without any additional characters. In that
* case, if the next note is not the same one, we clear lastChordFreqs, if it
* is the same, data from freqArray is being written in lastChordFreqs (it's the
* same for chords). There is also a special case then note duration is too
* short. In this case, we just clear lastChordFreqs.
*/
	k = 0;
	if (((i+k) < queue.size())&&
			(queue[i].initTime == queue[i+1].initTime)&&
			(queue[i+k].duration >= BIT)) {
		file << "< ";
		while (((i+k) < queue.size())&&
				(queue[i].initTime == queue[i+k].initTime)) {
			file << freqMap[queue[i+k].nNote] << " ";
			name += freqMap[queue[i+k].nNote];
			name += " ";
			k++;
		}
		combNum = k;
		file << ">";
		name += ">";
		lastChordFreqs = freqArray;
	} else {
		if (queue[i].duration >= BIT) {
			file << freqMap[queue[i].nNote];
			name = freqMap[queue[i].nNote];
			combNum = 1;
			if (queue[i].nNote != queue[i+1].nNote) {
				for (size_t m = 0; m < lastChordFreqs.size(); m++) {
					lastChordFreqs[m] = 0;
				}
			} else {
				lastChordFreqs = freqArray;
			}
		} else {
			for (size_t m = 0; m < lastChordFreqs.size(); m++) {
				lastChordFreqs[m] = 0;
			}
			combNum = 1;
		}
	}
	length = printDuration(name, length, i, file);
	if (flag == false) {
		file << ") ";
	}
	return length;
}
//==============================================================================
float Queue::printDuration(string & name, float & length,
		size_t &i, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/*
* Part, responsible for putting duration in file.
* If the note duration quantity is bigger than 1, we have additional text.
*/
	if (i == 0) {
		length = 0;
	}
	if (queue[i].duration > 1) {
		float full = 0;
		while (queue[i].duration - full > 1) {
			full++;
			file << "1~ " + name;
		}
		for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
			if (near(it->noteDur, queue[i].duration - full, BIT, BIT)) {
				file << it->func(name);
				length += it->noteDur + full;
			}
		}
	} else {
		for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
			if (near(it->noteDur, queue[i].duration, BIT, BIT)) {
				file << it->func(name);
				length += it->noteDur;
			}
		}
	}
	return length;
}
//==============================================================================
void Queue::printNextBeatPause(vector<int> & lastChordFreqs, float & length,
		size_t & i, int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	float pauseTaken = 0;
/* 1) Write pause between our note and start of the next beat */
	for (auto it = note_pause_list.begin();
			it != note_pause_list.end(); ++it) {
		if (near(it->pauseDur, 1 - queue[i].initTime -
				queue[i].duration + (int)(queue[i].initTime +
				queue[i].duration), BIT, BIT)) {
			file << it->pauseName;
			pauseTaken += 1 - queue[i].initTime - queue[i].duration
				+ (int)(queue[i].initTime + queue[i].duration);
			while (length > 1) {
				length--;
			}
			length += it->pauseDur;
		}
	}
/* Case then we don't put enough notes to make a full beat */
	if ((int)queue[i+combNum].initTime ==
			queue[i+combNum].initTime) {
		if (queue[i+combNum].initTime - length > BIT) {
			file << "r16 ";
			length = 0;
		}
	}
/* 2) Add necessary amount of full pauses */
	if (int(queue[i+combNum].initTime) -
			(int)(queue[i].initTime + queue[i].duration +
			pauseTaken) > 1) {
		float full = 0;
		while (((i+combNum) < queue.size())&&
				((int)(queue[i+combNum].initTime) -
				(int)(queue[i].initTime + queue[i].duration +
				pauseTaken) - full >= 1)) {
			full++;
			file << "r1 ";
		}
/* 3) Add pause between start of beat with next note and next note itself */
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (near(it->pauseDur, queue[i+1].initTime -
					(int)(queue[i+1].initTime), BIT, BIT)&&
					((i+1) < queue.size())) {
				file << it->pauseName;
				for (size_t m = 0; m < lastChordFreqs.size(); m++) {
					lastChordFreqs[m] = 0;
				}
			}
		}
	} else {
/* Analogy for 2) and 3) if we don't have full pauses */
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (near(it->pauseDur,
					queue[i+1].initTime - queue[i].initTime -
					queue[i].duration - pauseTaken, BIT, BIT)&&
					((i+1) < queue.size())) {
				file << it->pauseName;
				for (size_t m = 0; m < lastChordFreqs.size(); m++) {
					lastChordFreqs[m] = 0;
				}
			}
		}
	}
}
//==============================================================================
void Queue::printBeatPause(vector<int> & lastChordFreqs, float & length,
		size_t & i, int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/* If our note and the next one are in the same beat, the prosedure is simple */
	for (auto it = note_pause_list.begin();
			it != note_pause_list.end(); ++it) {
		if (near(it->pauseDur, queue[i+combNum].initTime -
				queue[i].initTime - queue[i].duration, BIT, BIT)) {
			file << it->pauseName;
			length += it->pauseDur;
			for (size_t m = 0; m < lastChordFreqs.size(); m++) {
				lastChordFreqs[m] = 0;
			}
		}
	}
	while (length > 1) {
		length--;
	}
/*
* If the note is in next beat and our note is not being prolonged in it,
* then, if we have supersmall pause between notes, and we have a need
* to add it (this is decided by value of "length" parameter), we place a pause
*/
	if (((int)(queue[i+combNum].initTime) != (int)(queue[i].initTime))&&
			(((int)(queue[i+combNum].initTime) -
			(int)(queue[i].initTime) - length) <= (2*BIT))&&
			(((int)(queue[i+combNum].initTime) -
			(int)(queue[i].initTime) - length) > 0)&&
			(queue[i+combNum].initTime - queue[i].initTime -
			queue[i].duration < BIT)) {
		file << "r16 ";
		for (size_t m = 0; m < lastChordFreqs.size(); m++) {
			lastChordFreqs[m] = 0;
		}
		length = 0;
	}
}
//==============================================================================
void Queue::checkEnd(float & length, size_t & i, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/* Adding small pauses at the end of staff*/
	if (i == queue.size() - 1) {
		float lastLength;
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (near(it->pauseDur, (int)(queue[i].initTime +
					queue[i].duration) + 1 -
					queue[i].initTime - queue[i].duration, BIT, BIT)) {
				file << it->pauseName;
				lastLength += it->pauseDur;
			}
		}
/* Case when we put not enough notes in file to make last beat full */
		if (1 - length - lastLength > BIT) {
			file << "r16 ";
		}
	}
}
