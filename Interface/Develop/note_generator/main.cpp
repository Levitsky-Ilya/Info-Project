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

using namespace std;

// Function for comparing two float quantities
bool near(float data, float num, float range1, float range2)
{
	if (((data + range1) >= num)&&((data - range2) <= num)) {
		return true;
	} else {
		return false;
	}
}

// Function for writing durations properly
string writer(string part1, string part2, string part3, string wrName)
{
	if (part3 == "") {
		if (part2 == "") {
			return part1;
		}
		return part1 + wrName + part2;
	}
	return part1 + wrName + part2 + wrName + part3;
}

// Duration comparer (not being used right now)
bool compareDur(const Note & note1, const Note & note2)
{
	return (note1.duration < note2.duration);
}

/*
 * This function breaks all notes in vector so that
 * it becomes possible to make chords from them.
 * Also this function breaks long notes by tacts.
 */
vector<struct Note> breaker(vector<struct Note> & queue)
{

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

/*
 * This function is responsible for putting
 * everything in the file in the right order.
 */
void drawNote(vector<struct Note> & queue, ofstream & f)
{
	using namespace std::placeholders;

/* Giving it's own text equivalent for each frequency */
	map<int, string> freqMap;

	freqMap[0]  =     "c,,"; freqMap[1]  =   "cis,,"; freqMap[2]  =     "d,,";
	freqMap[3]  =   "dis,,"; freqMap[4]  =     "e,,"; freqMap[5]  =     "f,,";
	freqMap[6]  =   "fis,,"; freqMap[7]  =     "g,,"; freqMap[8]  =   "gis,,";
	freqMap[9]  =     "a,,"; freqMap[10] =   "ais,,"; freqMap[11] =     "b,,";
	freqMap[12] =      "c,"; freqMap[13] =    "cis,"; freqMap[14] =      "d,";
	freqMap[15] =    "dis,"; freqMap[16] =      "e,"; freqMap[17] =      "f,";
	freqMap[18] =    "fis,"; freqMap[19] =      "g,"; freqMap[20] =    "gis,";
	freqMap[21] =      "a,"; freqMap[22] =    "ais,"; freqMap[23] =      "b,";
	freqMap[24] =       "c"; freqMap[25] =     "cis"; freqMap[26] =       "d";
	freqMap[27] =     "dis"; freqMap[28] =       "e"; freqMap[29] =       "f";
	freqMap[30] =     "fis"; freqMap[31] =       "g"; freqMap[32] =     "gis";
	freqMap[33] =       "a"; freqMap[34] =     "ais"; freqMap[35] =       "b";
	freqMap[36] =      "c'"; freqMap[37] =    "cis'"; freqMap[38] =      "d'";
	freqMap[39] =    "dis'"; freqMap[40] =      "e'"; freqMap[41] =      "f'";
	freqMap[42] =    "fis'"; freqMap[43] =      "g'"; freqMap[44] =    "gis'";
	freqMap[45] =      "a'"; freqMap[46] =    "ais'"; freqMap[47] =      "b'";
	freqMap[48] =     "c''"; freqMap[49] =   "cis''"; freqMap[50] =     "d''";
	freqMap[51] =   "dis''"; freqMap[52] =     "e''"; freqMap[53] =     "f''";
	freqMap[54] =   "fis''"; freqMap[55] =     "g''"; freqMap[56] =   "gis''";
	freqMap[57] =     "a''"; freqMap[58] =   "ais''"; freqMap[59] =     "b''";
	freqMap[60] =    "c'''"; freqMap[61] =  "cis'''"; freqMap[62] =    "d'''";
	freqMap[63] =  "dis'''"; freqMap[64] =    "e'''"; freqMap[65] =    "f'''";
	freqMap[66] =  "fis'''"; freqMap[67] =    "g'''"; freqMap[68] =  "gis'''";
	freqMap[69] =    "a'''"; freqMap[70] =  "ais'''"; freqMap[71] =    "b'''";
	freqMap[72] =   "c''''"; freqMap[73] = "cis''''"; freqMap[74] =   "d''''";
	freqMap[75] = "dis''''"; freqMap[76] =   "e''''"; freqMap[77] =   "f''''";
	freqMap[78] = "fis''''"; freqMap[79] =   "g''''"; freqMap[80] = "gis''''";
	freqMap[81] =   "a''''"; freqMap[82] = "ais''''"; freqMap[83] =   "b''''";

/* Giving it's own text equivalent for each pause duration */
	struct NotePause {
		float pauseDur;
		string pauseName;
	};

	list<NotePause> note_pause_list = {
		{0.0625,       "r16 "}, {0.1250,     "r8 "},
		{0.1875,       "r8. "}, {0.2500,     "r4 "},
		{0.3125,    "r4 r16 "}, {0.3750,    "r4. "},
		{0.4375,   "r4. r16 "}, {0.5000,     "r2 "},
		{0.5625,    "r2 r16 "}, {0.6250,  "r2 r8 "},
		{0.6875,    "r2 r8. "}, {0.7500,    "r2. "},
		{0.8125, "r2 r4 r16 "}, {0.8750, "r2 r4. "},
		{0.9375, "r2 r4 r8. "},
	};

/*
 * Giving it's own text equivalent for each note duration.
 * Functoin "writer" is being used for correct displaying.
 */
	struct NoteDur {
		float noteDur;
		std::function<string(string)> func;
	};

	list<NoteDur> noteDurList = {
		{0.0625, std::bind(writer, "16 ", "",    "",    _1)},
		{0.1250, std::bind(writer, "8 ",  "",    "",    _1)},
		{0.1875, std::bind(writer, "8. ", "",    "",    _1)},
		{0.2500, std::bind(writer, "4 ",  "",    "",    _1)},
		{0.3125, std::bind(writer, "4~ ", "16 ", "",    _1)},
		{0.3750, std::bind(writer, "4. ", "",    "",    _1)},
		{0.4375, std::bind(writer, "4~ ", "8. ", "",    _1)},
		{0.5000, std::bind(writer, "2 ",  "",    "",    _1)},
		{0.5625, std::bind(writer, "2~ ", "16 ", "",    _1)},
		{0.6250, std::bind(writer, "2~ ", "8 ",  "",    _1)},
		{0.6875, std::bind(writer, "2~ ", "8. ", "",    _1)},
		{0.7500, std::bind(writer, "2~ ", "4 ",  "",    _1)},
		{0.8125, std::bind(writer, "2~ ", "4~ ", "16 ", _1)},
		{0.8750, std::bind(writer, "2~ ", "4. ", "",    _1)},
		{0.9375, std::bind(writer, "2~ ", "4 ",  "8. ", _1)},
		{1.0000, std::bind(writer, "1 ",  "",    "",    _1)},
	};

/*
 * tmp[] and freq_array[] (see below) are used to check if chords nearby
 * have at lest one same note. In that case, there will be
 * additional "(" and ")" to connect chords on display.
 */
	vector<int> tmp;

	int combNum = 0; //for tracing an amount of notes taken at single step
	bool firstPause = true; //flag for pauses in first music tact
	const float BIT = 0.03125;

/* Deleting notes that take less than 1/32 from whole */
	for (unsigned int i = 0; i < queue.size(); i++) {
		if (queue[i].duration < BIT) {
			for (size_t num = i; num < queue.size() - 1; num++) {
				queue[num] = queue[num+1];
			}
			queue.pop_back();
		}
	}

	for (unsigned int i = 0; i < queue.size(); ) {
//Will be cleared if it's not a chord
		string name = "< ";
		vector<int> freqArray;

// Pause in beginning of first music tact
		for (auto it = note_pause_list.begin();
				it != note_pause_list.end(); ++it) {
			if (firstPause && near(it->pauseDur, queue[0].initTime, BIT, BIT)) {
				f << it->pauseName;
				firstPause = false;
			}
		}

/*
 * Checking if we have chord and, if so,
 * putting it in freqArray.
 */
		int k = 0;
		while (((i+k) < queue.size())&&
			  (queue[i+k].initTime == queue[i].initTime)&&
			   (queue[i+k].duration >= BIT)) {
			freqArray.push_back(queue[i+k].nNote);
			k++;
		}

/*
 * tmp storing frequencies for previous chords.
 * We compare tmp and freqArray. If we find same notes,
 * we need to use "(" and ")" in text file to place the slur.
 * flag is used to place ")" then time the comes.
 */
		bool flag = true;
		for (size_t m = 0; flag && m < freqArray.size(); m++) {
			for (size_t n = 0; flag && n < tmp.size(); n++)	{
				if (tmp[n] == freqArray[m]) {
					f << "( ";
					flag = false;
				}
			}
		}

/*
 * Frequecies of chord are stored between "<" and ">", so we place them there.
 * Single note is just being written without any additional characters.
 * In that case, if the next note is not the same one, we clear tmp,
 * if it is the same, data from freqArray is being written in tmp (it's the same
 * for chords). There is also a special case then note duration is too short.
 * In this case, we just clear tmp.
 */
		k = 0;
		if (((i+k) < queue.size())&&
				(queue[i].initTime == queue[i+1].initTime)&&
				(queue[i+k].duration >= BIT)) {
			f << "< ";
			while (((i+k) < queue.size())&&
					(queue[i].initTime == queue[i+k].initTime)) {
				f << freqMap[queue[i+k].nNote];
				f << " ";
				name += freqMap[queue[i+k].nNote];
				name += " ";
				k++;
			}
			combNum = k;
			f << ">";
			name += ">";
			tmp = freqArray;
		} else {
			if (queue[i].duration >= BIT) {
				f << freqMap[queue[i].nNote];
				name = "";
				name += freqMap[queue[i].nNote];
				combNum = 1;
				if (queue[i].nNote != queue[i+1].nNote) {
					for (size_t m = 0; m < tmp.size(); m++) {
						tmp[m] = 0;
					}
				} else {
					tmp = freqArray;
				}
			} else {
				for (size_t m = 0; m < tmp.size(); m++) {
					tmp[m] = 0;
				}
				combNum = 1;
			}
		}


/*
 * Part, responsible for putting duration in file.
 * If the note duration quantity is bigger than 1, we have additional text.
 */
		float length;
		if (queue[i].duration > 1) {
			float full = 0;
			while (queue[i].duration - full > 1) {
				full++;
				f << "1~ " + name;
			}
			for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
				if (near(it->noteDur, queue[i].duration - full, BIT, BIT)) {
					f << it->func(name);
					length += it->noteDur + full;
				}
			}
		} else {
			for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
				if (near(it->noteDur, queue[i].duration, BIT, BIT)) {
					f << it->func(name);
					length += it->noteDur;
				}
			}
		}

		if (flag == false) {
			f << ") ";
		}

/* Here pauses between notes are being added. */
		float pauseTaken = 0;
/* If the next note is in different tact, we need to do several things */
		if ((int)(queue[i+combNum].initTime)!=
				(int)(queue[i].initTime + queue[i].duration)&&
				(queue[i+combNum].initTime - queue[i].initTime -
				queue[i].duration > 2*BIT)) {
/* 1) Write pause between our note and start of the next tact */
			for (auto it = note_pause_list.begin();
					it != note_pause_list.end(); ++it) {
				if (near(it->pauseDur, 1 - queue[i].initTime -
						queue[i].duration + (int)(queue[i].initTime +
						queue[i].duration), BIT, BIT)) {
					f << it->pauseName;
					pauseTaken += 1 - queue[i].initTime - queue[i].duration
						+ (int)(queue[i].initTime + queue[i].duration);
					while (length > 1) {
						length--;
					}
					length += it->pauseDur;
				}
			}
/* Case then we don't put enough notes to make a full tact */
			if ((int)queue[i+combNum].initTime ==
					queue[i+combNum].initTime) {
				if (queue[i+combNum].initTime - length > BIT) {
					f << "r16 ";
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
					f << "r1 ";
				}
/* 3) Add pause between start of tact with next note and next note itself */
				for (auto it = note_pause_list.begin();
						it != note_pause_list.end(); ++it) {
					if (near(it->pauseDur, queue[i+1].initTime -
							(int)(queue[i+1].initTime), BIT, BIT)&&
							((i+1) < queue.size())) {
						f << it->pauseName;
						for (size_t m = 0; m < tmp.size(); m++) {
							tmp[m] = 0;
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
						f << it->pauseName;
						for (size_t m = 0; m < tmp.size(); m++) {
							tmp[m] = 0;
						}
					}
				}
			}
		} else {
/* If our note and the next one are in the same tact, the prosedure is simple */
			for (auto it = note_pause_list.begin();
					it != note_pause_list.end(); ++it) {
				if (near(it->pauseDur, queue[i+combNum].initTime -
						queue[i].initTime - queue[i].duration, BIT, BIT)) {
					f << it->pauseName;
					length += it->pauseDur;
					for (size_t m = 0; m < tmp.size(); m++) {
						tmp[m] = 0;
					}
				}
			}
			while (length > 1) {
				length--;
			}
/*
 * If the note is in next tact and our note is not being prolonged in it,
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
				f << "r16 ";
				for (size_t m = 0; m < tmp.size(); m++) {
					tmp[m] = 0;
				}
				length = 0;
			}
/* Adding small pauses at the end of notes */
			if (i == queue.size() - 1) {
				float lastLength;
				for (auto it = note_pause_list.begin();
						it != note_pause_list.end(); ++it) {
					if (near(it->pauseDur, (int)(queue[i].initTime +
							queue[i].duration) + 1 -
							queue[i].initTime - queue[i].duration, BIT, BIT)) {
						f << it->pauseName;
						lastLength += it->pauseDur;
					}
				}
/* Case when we put not enough notes in file to make a full tact */
				if ((int)(queue[i].initTime + queue[i].duration) + 1 -
						queue[i].initTime - queue[i].duration -
						lastLength > (2*BIT)) {
					f << "r16 ";
				}
			}
		}

		i = i + combNum;
	}
}



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

	vector<Note> noteVectN;
	vector<Note> noteVectL;
	for (size_t i = 0; i < noteVect.size(); i++) {
		if (noteVect[i].nNote < 36) {
			noteVectL.push_back(noteVect[i]);
		} else {
			noteVectN.push_back(noteVect[i]);
		}
	}

	breaker(noteVectN);
	breaker(noteVectL);

	ofstream f("E:/Programs/Lilypond/file.ly");

	f << "normal = \\new Staff { \n";

/* Drawing pauses at the beginning fot the same length of staffs*/
	int beginPauseCounterN = 0;
	if ((noteVectN.size() != 0)&&(noteVectL.size() != 0)) {
		while ((int)noteVectN[0].initTime - (int)noteVectL[0].initTime -
			   beginPauseCounterN >= 1) {
			f << "r1 ";
			beginPauseCounterN++;
		}
/* We have to make new vector without these pauses for algorythm to work */
		vector<Note> newNoteVectN;
		for (size_t i = 0; i < noteVectN.size(); i++) {
			newNoteVectN.push_back(noteVectN[i]);
			newNoteVectN[i].initTime = noteVectN[i].initTime -
			beginPauseCounterN;
		}
		drawNote(newNoteVectN, f);
	} else {
		drawNote(noteVectN, f);
	}




/* Drawing pauses in the end for the same length of staffs */
	int endPauseCounterN = 0;
	if ((noteVectL.size() != 0)&&(noteVectN.size() != 0)) {
		while ((int)(noteVectL[noteVectL.size()-1].initTime +
				noteVectL[noteVectL.size()-1].duration) -
				(int)(noteVectN[noteVectN.size()-1].initTime +
				noteVectN[noteVectN.size()-1].duration) -
				endPauseCounterN >= 1) {
			f << "r1 ";
			endPauseCounterN++;
		}
	}
	f << "\n";
	f << "}\n";

	f << "bass = \\new Staff { \n";
	f << "\\clef \"bass\" \n";

/* See above */
	int beginPauseCounterL = 0;
	if ((noteVectN.size() != 0)&&(noteVectL.size() != 0)) {
		while ((int)noteVectL[0].initTime - (int)noteVectN[0].initTime -
			   beginPauseCounterL >= 1) {
			f << "r1 ";
			beginPauseCounterL++;
		}
/* See above */
		vector<Note> newNoteVectL;
		for (size_t i = 0; i < noteVectL.size(); i++) {
			newNoteVectL.push_back(noteVectL[i]);
			newNoteVectL[i].initTime = noteVectL[i].initTime -
			beginPauseCounterL;
		}
		drawNote(newNoteVectL, f);
	} else {
		drawNote(noteVectL, f);
	}




/* See above */
	int endPauseCounterL = 0;
	if ((noteVectL.size() != 0)&&(noteVectN.size() != 0)) {
		while ((int)(noteVectN[noteVectN.size()-1].initTime +
				noteVectN[noteVectN.size()-1].duration) -
				(int)(noteVectL[noteVectL.size()-1].initTime +
				noteVectL[noteVectL.size()-1].duration) -
				endPauseCounterL >= 1) {
			f << "r1 ";
			endPauseCounterL++;
		}
	}

	f << "\n";
	f << "}\n";

	f << "{\n";
	f << "\\new PianoStaff << \n";
	f << "\\normal \n";
	f << "\\bass \n";
	f << ">> \n";
	f << "} \n";

	f.close();

	return 0;
}
