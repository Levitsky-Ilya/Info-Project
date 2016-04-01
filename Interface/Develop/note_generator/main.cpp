#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <functional>

using namespace std;

bool near(float freq, float num, float range1, float range2)
{
	if (((freq + range1) >= num)&&((freq - range2) <= num)) {
		return true;
	} else {
		return false;
	}
}

string writer(string part1, string part2, string part3, string wr_name)
{
	if (part3 == "") {
		if (part2 == "") {
			return part1;
		}
		return part1 + wr_name + part2;
	}
	return part1 + wr_name + part2 + wr_name + part3;
}

int main()
{
	using namespace std::placeholders;

	struct NoteFreq {
		float note_freq;
		string note_name;
		float note_near1;
		float note_near2;
	};

	list<NoteFreq> note_freq_list = {
		{261.63,   "c'",  7.345,  7.775}, {277.18, "cis'",  7.775,  8.240},
		{293.66,   "d'",  8.240,  8.740}, {311.13, "dis'",  8.740,  9.250},
		{329.63,   "e'",  9.250,  9.800}, {349.23,   "f'",  9.800, 10.380},
		{369.99, "fis'", 10.380, 11.005}, {392.00,   "g'", 11.005, 11.650},
		{415.30, "gis'", 11.650, 12.350}, {440.00,   "a'", 12.350, 13.080},
		{466.16, "ais'", 13.080, 13.860}, {493.88,   "h'", 13.860, 14.685},
	};

	struct NoteDur {
		float note_dur;
		std::function<string(string)> func;
	};

	list<NoteDur> note_dur_list = {
		{0.0625, std::bind(writer, "16 ", "",    "",    _1)},
		{0.1250, std::bind(writer, "8 ",  "",    "",    _1)},
		{0.1875, std::bind(writer, "8. ", "",    "",    _1)},
		{0.2500, std::bind(writer, "4 ",  "",    "",    _1)},
		{0.3125, std::bind(writer, "4~ ", "16 ", "",    _1)},
		{0.3750, std::bind(writer, "4. ", "",    "",    _1)},
		{0.4375, std::bind(writer, "4 ",  "8. ", "",    _1)},
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

	struct Notes
	{
		float freq;
		int n;
		float duration;
		double init_time;
	};

	Notes note_1;
	note_1.freq = 329.70f;
	note_1.n = 1;
	note_1.duration = 0.25f;
	note_1.init_time = 0;

	Notes note_2;
	note_2.freq = 470.05f;
	note_2.n = 2;
	note_2.duration = 0.88f;
	note_2.init_time = 0.5;

	vector<struct Notes> note_queue;
	note_queue.push_back(note_1);
	note_queue.push_back(note_2);

	ofstream f("E:/Programs/Lilypond/file.ly");
		f << "{\n";

	for (unsigned int i = 0; i < note_queue.size(); i++)
	{
		string name;
		const float bit = 0.03125;
//Frequency
////////////////////////////////////////
		for (auto it = note_freq_list.begin(); it != note_freq_list.end(); ++it) {
			if (near(it->note_freq, note_queue[i].freq, it->note_near1, it->note_near2)) {
				f << it->note_name;
				name = it->note_name;
			}
		}


//Duration
////////////////////////////////////////
		/* ... */
		for (auto it = note_dur_list.begin(); it != note_dur_list.end(); ++it) {
			if (near(it->note_dur, note_queue[i].duration, bit, bit)) {
				f << it->func(name);
			}
		}

		/*if (near(0.0625, note_queue[i].duration, 2*bit, bit)) {
			f << "16 ";
		} else if (near(0.125, note_queue[i].duration, bit, bit)) {
			f << "8 ";
		} else if (near(0.1875, note_queue[i].duration, bit, bit)) {
			f << "8. ";
		} else if (near(0.25, note_queue[i].duration, bit, bit)) {
			f << "4 ";
		} else if (near(0.3125, note_queue[i].duration, bit, bit)) {
			f << "4~ " << name << "16 ";
		} else if (near(0.375, note_queue[i].duration, bit, bit)) {
			f << "4. ";
		} else if (near(0.4375, note_queue[i].duration, bit, bit)) {
			f << "4~ " << name << "8. ";
		} else if (near(0.5, note_queue[i].duration, bit, bit)) {
			f << "2 ";
		} else if (near(0.5625, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "16 ";
		} else if (near(0.625, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "8 ";
		} else if (near(0.6875, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "8. ";
		} else if (near(0.75, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "4 ";
		} else if (near(0.8125, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "4~ " << name << "16 ";
		} else if (near(0.875, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "4. ";
		} else if (near(0.9375, note_queue[i].duration, bit, bit)) {
			f << "2~ " << name << "4~ " << name << "8. ";
		} else if (near(1, note_queue[i].duration, bit, bit)) {
			f << "1 ";
		}*/
//Pause
////////////////////////////////////////
		if (near(0.0625, note_queue[i+1].init_time - note_queue[i].duration, 0.005, 0.005)) {
			f << "r16 ";
		} else if (near(0.125, note_queue[i+1].init_time - note_queue[i].duration, 0.005, 0.005)) {
			f << "r8 ";
		} else if (near(0.25, note_queue[i+1].init_time - note_queue[i].duration, 0.005, 0.005)) {
			f << "r4 ";
		} else if (near(0.5, note_queue[i+1].init_time - note_queue[i].duration, 0.005, 0.005)) {
			f << "r2 ";
		} else if (near(1, note_queue[i+1].init_time - note_queue[i].duration, 0.005, 0.005)) {
			f << "r1 ";
		}

	}
	f << "\n";
	f << "}";
	f.close();

	return 0;
}

