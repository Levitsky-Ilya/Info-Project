#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <functional>
#include <algorithm>
#include <map>
#include <notes.h>

using namespace std;

bool near(float freq, float num, float range1, float range2)
{
	if (((freq + range1) >= num)&&((freq - range2) <= num)) {
		return true;
	} else {
		return false;
	}
}

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

bool compareDur(const Notes & note1, const Notes & note2)
{
	return (note1.duration < note2.duration);
}

vector<struct Notes> breaker(vector<struct Notes> & queue)
{

	list<struct Notes> noteList;
	struct Notes noteAdded;

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

	while(!queue.empty()) {
		noteList.push_front(queue.back());
		queue.pop_back();
	}

	for (auto it2 = noteTiming.begin(); it2 != noteTiming.end(); ++it2){
		for (auto it1 = noteList.begin(); it1 != noteList.end(); ++it1 ) {
			if ((it1->initTime < *it2)&&((it1->initTime + it1->duration) > *it2)) {
				noteAdded.duration = it1->initTime + it1->duration - *it2;
				noteAdded.initTime = *it2;
				noteAdded.nNote = it1->nNote;

				it1->duration = *it2 - it1->initTime;

				noteList.insert(it1, noteAdded);
			}
		}
	}

	while (!noteList.empty()) {
		queue.push_back(noteList.front());
		noteList.pop_front();
	}

	sort(queue.begin(), queue.end());

	return queue;

}


void drawNote(vector<struct Notes> & queue, ofstream & f)
{
	using namespace std::placeholders;

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
		{0.9375, "r2 r4 r8. "}, {1.0000,     "r1 "},
	};

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

	vector<int> tmp;

	vector<int> freqArray;
	int combNum = 0;
	bool firstPause = true;

	for (unsigned int i = 0; i < queue.size(); )
	{
		string name = "< ";

		const float bit = 0.03125;

		for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
			if (firstPause && near(it->pauseDur, queue[0].initTime, bit, bit)) {
				f << it->pauseName;
				firstPause = false;
			}
		}

		int k = 0;
		while (((i+k) < queue.size())&&
			  (queue[i+k].initTime == queue[i].initTime)) {
			freqArray.push_back(queue[i+k].nNote);
			k++;
		}
		bool flag = true;
		for (int m = 0; flag && m < k; m++) {
			for (size_t n = 0; flag && n < tmp.size(); n++)	{
				if (tmp[n] == freqArray[m]) {
					f << "( ";
					flag = false;
				}
			}
		}
		k = 0;
		if (((i+k) < queue.size())&&
					(queue[i].initTime == queue[i+1].initTime)) {
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
		}

		if (queue[i].duration > 1) {
			float full = 0;
			while (queue[i].duration - full > 1) {
				full++;
				f << "1~ " + name;
			}
			for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
				if (near(it->noteDur, queue[i].duration - full, bit, bit)) {
					f << it->func(name);
				}
			}
		} else {
			for (auto it = noteDurList.begin(); it != noteDurList.end(); ++it) {
				if (near(it->noteDur, queue[i].duration, bit, bit)) {
					f << it->func(name);
				}
			}
		}
		if (flag == false) {
			f << ") ";
		}

		float pauseTaken = 0;
		if ((int)(queue[i+combNum].initTime) !=
				(int)(queue[i].initTime + queue[i].duration)) {
			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (near(it->pauseDur, 1 - queue[i].initTime
						- queue[i].duration + (int)(queue[i].initTime
							+ queue[i].duration), bit, bit)) {
					f << it->pauseName;
					pauseTaken += 1 - queue[i].initTime - queue[i].duration
						+ (int)(queue[i].initTime + queue[i].duration);
				}
			}
			if (queue[i+combNum].initTime - queue[i].initTime
					- queue[i].duration - pauseTaken > 1) {
				float full = 1;
				while (queue[i+combNum].initTime - queue[i].initTime
						- queue[i].duration - full - pauseTaken > 1) {
					full++;
					f << "r1 ";
				}
				for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
					if (near(it->pauseDur, queue[i+1].initTime
							- (int)(queue[i+1].initTime), bit, bit)) {
						f << it->pauseName;
						for (size_t m = 0; m < tmp.size(); m++) {
							tmp[m] = 0;
						}
					}
				}
			} else {
				for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
					if (near(it->pauseDur, queue[i+1].initTime
							- queue[i].initTime - queue[i].duration - pauseTaken, bit, bit)) {
						f << it->pauseName;
						for (size_t m = 0; m < tmp.size(); m++) {
							tmp[m] = 0;
						}
					}
				}
			}
		} else {
			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (near(it->pauseDur, queue[i+combNum].initTime
						- queue[i].initTime - queue[i].duration, bit, bit)) {
					f << it->pauseName;
					for (size_t m = 0; m < tmp.size(); m++) {
						tmp[m] = 0;
					}
				}
			}
		}

		i = i + combNum;
	}
}



int main()
{
	Notes note_1;
	note_1.nNote = 38;
	note_1.duration = 0.4f;
	note_1.initTime = 0.1;

	Notes note_2;
	note_2.nNote = 47;
	note_2.duration = 1.16f;
	note_2.initTime = 0.1;

	Notes note_3;
	note_3.nNote = 41;
	note_3.duration = 0.16f;
	note_3.initTime = 0.5;

	Notes note_4;
	note_4.nNote = 43;
	note_4.duration = 3.5f;
	note_4.initTime = 4.0;

	Notes note_5;
	note_5.nNote = 45;
	note_5.duration = 2.68f;
	note_5.initTime = 4.5;

	Notes note_6;
	note_6.nNote = 37;
	note_6.duration = 0.25f;
	note_6.initTime = 5.8;


	Notes note_7;
	note_7.nNote = 25;
	note_7.duration = 0.25f;
	note_7.initTime = 0;

	Notes note_8;
	note_8.nNote = 35;
	note_8.duration = 3.3f;
	note_8.initTime = 0.5;

	Notes note_9;
	note_9.nNote = 28;
	note_9.duration = 1.0f;
	note_9.initTime = 1.5;

	vector<struct Notes> noteVectN;
	noteVectN.push_back(note_1);
	noteVectN.push_back(note_2);
	noteVectN.push_back(note_3);
	noteVectN.push_back(note_4);
	noteVectN.push_back(note_5);
	noteVectN.push_back(note_6);
	vector<struct Notes> noteVectL;
	noteVectL.push_back(note_7);
	noteVectL.push_back(note_8);
	noteVectL.push_back(note_9);

	breaker(noteVectN);
	breaker(noteVectL);

	ofstream f("E:/Programs/Lilypond/file.ly");

	f << "normal = \\new Staff { \n";
	drawNote(noteVectN, f);
	int pauseCounterN = 0;
	while ((int)(noteVectL[noteVectL.size()-1].initTime
			+ noteVectL[noteVectL.size()-1].duration
				- noteVectN[noteVectN.size()-1].initTime
					- noteVectN[noteVectN.size()-1].duration) - pauseCounterN > 1) {
		f << "r1 ";
		pauseCounterN++;
	}
	f << "\n";
	f << "}\n";

	f << "bass = \\new Staff { \n";
	f << "\\clef \"bass\" \n";
	drawNote(noteVectL, f);
	int pauseCounterL = 0;
	while ((int)(noteVectN[noteVectN.size()-1].initTime
			+ noteVectN[noteVectN.size()-1].duration
				- noteVectL[noteVectL.size()-1].initTime
					- noteVectL[noteVectL.size()-1].duration) - pauseCounterL > 1) {
		f << "r1 ";
		pauseCounterL++;
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

