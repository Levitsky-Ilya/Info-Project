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
	list<unsigned int> noteTiming;
	for (size_t i = 0; i < queue.size(); i++ ) {
		noteTiming.push_back(queue[i].initTime);
		noteTiming.push_back(queue[i].initTime + queue[i].duration);
	}
	for (int num = 16; num <= 16000; num += 16) {
		noteTiming.push_back(num);
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
	unsigned int beginPauseCounter = 0;
	if ((queue.size() != 0)&&(comparedVect.size() != 0)&&
			((queue[0].initTime&(~15u)) > (comparedVect[0].initTime&(~15u)))) {
		while ((queue[0].initTime&(~15u)) - (comparedVect[0].initTime&(~15u)) -
			   beginPauseCounter >= 16) {
			file << "r1 ";
			beginPauseCounter += 16;
		}
	}
	drawNote(file);

/* Drawing pauses in the end for the same length of staffs */
	int endPauseCounter = 0;
	if ((comparedVect.size() != 0)&&(queue.size() != 0)&&
			(((comparedVect[comparedVect.size()-1].initTime +
			comparedVect[comparedVect.size()-1].duration) % 16) != 0)) {
		while (((comparedVect[comparedVect.size()-1].initTime + 16 +
				comparedVect[comparedVect.size()-1].duration)&(~15u)) -
				((queue[queue.size()-1].initTime +
				queue[queue.size()-1].duration)&(~15u)) -
				endPauseCounter >= 16) {
			file << "r1 ";
			endPauseCounter += 16;
		}
	}

	file << "\\bar \"|.\"";
	file << "\n";
	file << "}\n";
}
//==============================================================================
void Queue::drawNote(ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	firstPauseChecker(file);

/*
 * lastChordFreqs[] and freq_array[] (in printChord function) are used
 * to check if chords nearby have at lest one same note. In that case,
 * there will be additional "(" and ")" to connect chords on display.
 */
	vector<int> lastChordFreqs;

	int combNum = 0; //for tracing an amount of notes taken at single step

	for (size_t i = 0; i < queue.size(); ) {

		printChord(lastChordFreqs, i, combNum, file);
		if (((queue[i+combNum].initTime)&(~15u)) !=
				((queue[i].initTime + queue[i].duration)&(~15u))) {
			printNextBeatPause(lastChordFreqs, i, combNum, file);
		} else {
			printBeatPause(lastChordFreqs, i, combNum, file);
			checkEnd(i, combNum, file);
		}

		i = i + combNum;
	}
}
//==============================================================================
void Queue::firstPauseChecker(ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	if (queue.size() != 0) {
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			unsigned int full = 0;
			if (queue[0].initTime > 16) {
				while (queue[0].initTime - full > 16) {
					full += 16;
				}
			}
			if (it->pauseDur == (queue[0].initTime - full)) {
				file << it->pauseName;
			}
		}
	}
}
//==============================================================================
void Queue::printChord(vector<int> & lastChordFreqs, size_t & i,
		int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
//Will be cleared if it's not a chord
	string name = "< ";
	vector<int> freqArray;
/* Checking if we have chord and, if so, putting it in freqArray */
	int k = 0;
	while (((i+k) < queue.size())&&
		  (queue[i+k].initTime == queue[i].initTime)) {
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
			(queue[i].initTime == queue[i+1].initTime)) {
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
		file << freqMap[queue[i].nNote];
		name = freqMap[queue[i].nNote];
		combNum = 1;
		lastChordFreqs = freqArray;
	}
	printDuration(name, i, file);
	if (flag == false) {
		file << ") ";
	}
}
//==============================================================================
void Queue::printDuration(string & name, size_t &i, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/*
* Part, responsible for putting duration in file.
* If the note duration quantity is bigger than 1, we have additional text.
*/
	for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
		if (it->noteDur == queue[i].duration) {
			file << it->func(name);
		}
	}
}
//==============================================================================
void Queue::printNextBeatPause(vector<int> & lastChordFreqs,
		size_t & i, int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));

	unsigned int pauseTaken = 0;
/* 1) Write pause between our note and start of the next beat */
	for (auto it = note_pause_list.begin();
			it != note_pause_list.end(); ++it) {
		if (it->pauseDur == 16 - queue[i].initTime -
				queue[i].duration + ((queue[i].initTime +
				queue[i].duration)&(~15u))) {
			file << it->pauseName;
			pauseTaken += 16 - queue[i].initTime - queue[i].duration
				+ ((queue[i].initTime + queue[i].duration)&(~15u));
		}
	}
/* 2) Add necessary amount of full pauses */
	if ((queue[i+combNum].initTime&(~15u)) -
			((queue[i].initTime + queue[i].duration +
			pauseTaken)&(~15u)) > 16) {
		int full = 0;
		while (((i+combNum) < queue.size())&&
				((queue[i+combNum].initTime&(~15u)) -
				((queue[i].initTime + queue[i].duration +
				pauseTaken)&(~15u)) - full >= 16)) {
			full += 16;
			file << "r1 ";
		}
/* 3) Add pause between start of beat with next note and next note itself */
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (it->pauseDur == queue[i+combNum].initTime -
					(queue[i+combNum].initTime&(~15u))&&
					((i+combNum) < queue.size())) {
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
			if ((it->pauseDur == queue[i+combNum].initTime - queue[i].initTime -
					queue[i].duration - pauseTaken)&&
					((i+combNum) < queue.size())) {
				file << it->pauseName;
				for (size_t m = 0; m < lastChordFreqs.size(); m++) {
					lastChordFreqs[m] = 0;
				}
			}
			for (size_t m = 0; m < lastChordFreqs.size(); m++) {
				lastChordFreqs[m] = 0;
			}
		}
	}
}
//==============================================================================
void Queue::printBeatPause(vector<int> & lastChordFreqs,
		size_t & i, int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/* If our note and the next one are in the same beat, the prosedure is simple */
	for (auto it = note_pause_list.begin();
			it != note_pause_list.end(); ++it) {
		if (it->pauseDur == queue[i+combNum].initTime -
				queue[i].initTime - queue[i].duration) {
			file << it->pauseName;
			for (size_t m = 0; m < lastChordFreqs.size(); m++) {
				lastChordFreqs[m] = 0;
			}
		}
	}
}
//==============================================================================
void Queue::checkEnd(size_t & i, int & combNum, ofstream & file)
{
	vector<Note> & queue = *(static_cast<vector<Note>*>(this));
/* Adding small pauses at the end of staff*/
	if ((i + combNum) == queue.size()) {
		float lastLength;
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (it->pauseDur == ((queue[i].initTime +
					queue[i].duration)&(~15u)) + 16 -
					queue[i].initTime - queue[i].duration) {
				file << it->pauseName;
				lastLength += it->pauseDur;
			}
		}
	}
}
