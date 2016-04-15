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

bool dur_comp(const Notes & note1, const Notes & note2)
{
	return (note1.duration < note2.duration);
}

vector<struct Notes> breaker(vector<struct Notes> & queue) {

	list<struct Notes> note_l;
	struct Notes add_note;

	list<float> note_timing;
	for (size_t i = 0; i < queue.size(); i++ ) {
		note_timing.push_back(queue[i].init_time);
		note_timing.push_back(queue[i].init_time + queue[i].duration);
	}
	note_timing.sort();
	note_timing.unique();

	while(!queue.empty()) {
		note_l.push_front(queue.back());
		queue.pop_back();
	}

	for (auto it1 = note_l.begin(); it1 != note_l.end(); ++it1 ) {
		for (auto it2 = note_timing.begin(); it2 != note_timing.end(); ++it2){
			if ((it1->init_time < *it2)&&((it1->init_time + it1->duration) > *it2)) {
				add_note.duration = it1->init_time + it1->duration - *it2;
				add_note.init_time = *it2;
				add_note.freq = it1->freq;

				it1->duration = *it2 - it1->init_time;

				note_l.insert(it1, add_note);
			}
		}
	}

	while (!note_l.empty()) {
		queue.push_back(note_l.front());
		note_l.pop_front();
	}

	sort(queue.begin(), queue.end());

	return queue;

#if 0
	for (auto it = note_l.begin(); it != note_l.end(); ) {
		auto k = (++it)--;
		while (it->init_time == k->init_time) {
			if (k->duration == it->duration)
				continue;
			add_note.duration = k->duration - it->duration;
			add_note.init_time = it->init_time + it->duration;
			add_note.freq = k->freq;

			k->duration = it->duration;

			temp.push_back(add_note);
			k++;
		}
		k--;
		auto x = k;
		while (add_note.init_time < x->init_time) {
			x++;
		}
		note_l.splice(x,std::move(temp));
		it = (++k)--;

		x++;
		auto y = x;
		while (x->init_time == y->init_time) ++y;
		note_l.sort();
	}
	while (!note_l.empty()) {
		queue.push_back(note_l.front());
		note_l.pop_back();
	}

	return queue;
#endif // 0
}

//!!!!!!!!!!!!!
#if 0
string chord(const vector<struct Notes> & queue, int note_num) {

	//по идее можно выполнять для любой ноты, просто это будет аккорд из одной ноты


}
#endif // 0

int main()
{
	using namespace std::placeholders;

	map<float, string> freq_map;

	freq_map[261.63] =   "c' "; freq_map[277.18] = "cis' ";
	freq_map[293.66] =   "d' "; freq_map[311.13] = "dis' ";
	freq_map[329.63] =   "e' "; freq_map[349.23] =   "f' ";
	freq_map[369.99] = "fis' "; freq_map[392.00] =   "g' ";
	freq_map[415.30] = "gis' "; freq_map[440.00] =   "a' ";
	freq_map[466.16] = "ais' "; freq_map[493.88] =   "b' ";

	map<float, string> lfreq_map;

	lfreq_map[130.81] =   "c "; lfreq_map[138.59] = "cis ";
	lfreq_map[146.83] =   "d "; lfreq_map[155.56] = "dis ";
	lfreq_map[164.81] =   "e "; lfreq_map[174.61] =   "f ";
	lfreq_map[185.00] = "fis "; lfreq_map[196.00] =   "g ";
	lfreq_map[207.65] = "gis "; lfreq_map[220.00] =   "a ";
	lfreq_map[233.08] = "ais "; lfreq_map[246.94] =   "b ";


	struct NotePause {
		float pause_dur;
		string pause_name;
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


	Notes note_1;
	note_1.freq = 277.18f;
	note_1.n = 1;
	note_1.duration = 0.25f;
	note_1.init_time = 0.1;

	Notes note_2;
	note_2.freq = 493.88f;
	note_2.n = 2;
	note_2.duration = 0.78f;
	note_2.init_time = 0.1;

	Notes note_3;
	note_3.freq = 130.81f;
	note_3.n = 3;
	note_3.duration = 0.25f;
	note_3.init_time = 0.71;

	vector<struct Notes> note_n_queue;
	note_n_queue.push_back(note_1);
	note_n_queue.push_back(note_2);
	vector<struct Notes> note_l_queue;
	note_l_queue.push_back(note_3);

	breaker(note_n_queue);

	ofstream f("E:/Programs/Lilypond/file.ly");
	f << "normal = \\new Staff { \n";

	vector<float> tmp;

	vector<float> freq_array;
	int comb_num = 0;
	bool first_pause = true;

	for (unsigned int i = 0; i < note_n_queue.size(); i++)
	{
		string name;

		const float bit = 0.03125;

		if (note_n_queue[i].freq >= 261.63) {

			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (first_pause && near(it->pause_dur, note_n_queue[0].init_time, bit, bit)) {
					f << it->pause_name;
					first_pause = false;
				}
			}

			int k = 0;
			while (((i+k) < note_n_queue.size())&&
				  (note_n_queue[i+k].init_time == note_n_queue[i].init_time)) {
				freq_array.push_back(note_n_queue[i+k].freq);
				k++;
			}
			bool flag = true;
			for (int m = 0; flag && m < k; m++) {
				for (size_t n = 0; flag && n < tmp.size(); n++)	{
					if (tmp[n] == freq_array[m]) {
						f << "( ";
						flag = false;
					}
				}
			}
			k = 0;
			if (((i+k) < note_n_queue.size())&&
						(note_n_queue[i].init_time == note_n_queue[i+1].init_time)) {
				f << "< ";
				while (((i+k) < note_n_queue.size())&&
					  (note_n_queue[i].init_time == note_n_queue[i+k].init_time)) {
					f << freq_map[note_n_queue[i+k].freq];
					name += freq_map[note_n_queue[i+k].freq];
					k++;
				}
				comb_num = k;
				f << ">";
			} else {
				f << freq_map[note_n_queue[i].freq];
				name = freq_map[note_n_queue[i].freq];
			}

			tmp = freq_array;

			for (auto it = note_dur_list.begin(); it != note_dur_list.end(); ++it) {
				if (near(it->note_dur, note_n_queue[i].duration, bit, bit)) {
					f << it->func(name);
				}
			}
			if (flag == false) {
				f << " )";
			}

			i = i + comb_num - 1;

			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (near(it->pause_dur, note_n_queue[i+2].init_time - note_n_queue[i+1].init_time - note_n_queue[i+1].duration, bit, bit)) {
					f << it->pause_name;
				}
			}
		}
	}

	f << "\n";
	f << "}\n";

	f << "bass = \\new Staff {\n";
	f << "\\clef \"bass\" \n";
	/*for (unsigned int i = 0; i < note_l_queue.size(); i++)
	{
		string name;
		const float bit = 0.03125;

		if (note_l_queue[i].freq < 261.63) {

			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (near(it->pause_dur, note_l_queue[0].init_time, bit, bit)) {
					f << it->pause_name;
				}
			}

			for (auto it = note_lfreq_list.begin(); it != note_lfreq_list.end(); ++it) {
				if (it->note_freq == note_l_queue[i].freq) {
					f << it->note_name;
					name = it->note_name;
				}
			}

			for (auto it = note_dur_list.begin(); it != note_dur_list.end(); ++it) {
				if (near(it->note_dur, note_l_queue[i].duration, bit, bit)) {
					f << it->func(name);
				}
			}

			for (auto it = note_pause_list.begin(); it != note_pause_list.end(); ++it) {
				if (near(it->pause_dur, note_l_queue[i+1].init_time - note_l_queue[i].init_time - note_l_queue[i].duration, bit, bit)) {
					f << it->pause_name;
				}
			}
		};
	}*/
	f << "\n";
	f << "}\n";

	f << "{\n";
	f << "\\override Score.BarLine.stencil = ##f \n";
	f << "\\override Score.BarLine.allow-span-bar = ##f \n";
	f << "<< \n";
	f << "\\normal \n";
	f << "\\bass \n";
	f << ">> \n";
	f << "} \n";

	f.close();

	return 0;
}

