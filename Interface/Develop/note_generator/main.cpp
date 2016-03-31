#include <iostream>
#include <vector>

using namespace std;

bool near(float freq, float num, float range)
{
	if (((freq + range) >= num)&&((freq - range) <= num)) {
		return true;
	} else {
		return false;
	}
}

int main()
{
	struct Notes
	{
		float freq;
		int n;
		float duration;
		double init_time;
	};

	Notes note_1;
	note_1.freq = 261.63f;
	note_1.n = 1;
	note_1.duration = 0.25f;
	note_1.init_time = 0;

	Notes note_2;
	note_2.freq = 329.63f;
	note_2.n = 2;
	note_2.duration = 0.0625f;
	note_2.init_time = 0.5;

	vector<struct Notes> note_queue;
	note_queue.push_back(note_1);
	note_queue.push_back(note_2);

	FILE *f = fopen("E:/Programs/Lilypond/file.ly", "w");
	fputs ("{\n", f);

	for (unsigned int i = 0; i < note_queue.size(); i++)
	{
		if (near(261.63, note_queue[i].freq, 1.5)) {
			fputs("c'", f);
		} else if (near(329.63, note_queue[i].freq, 1.5)) {
			fputs("e'", f);
		}

		if (near(0.0625, note_queue[i].duration, 0.005)) {
			fputs("16 ", f);
		} else if (near(0.125, note_queue[i].duration, 0.005)) {
			fputs("8 ", f);
		} else if (near(0.25, note_queue[i].duration, 0.005)) {
			fputs("4 ", f);
		} else if (near(0.5, note_queue[i].duration, 0.005)) {
			fputs("2 ", f);
		} else if (near(1, note_queue[i].duration, 0.005)) {
			fputs("1 ", f);
		}

		if (near(0.0625, note_queue[i+1].init_time - note_queue[i].duration, 0.005)) {
			fputs("r16 ", f);
		} else if (near(0.125, note_queue[i+1].init_time - note_queue[i].duration, 0.005)) {
			fputs("r8 ", f);
		} else if (near(0.25, note_queue[i+1].init_time - note_queue[i].duration, 0.005)) {
			fputs("r4 ", f);
		} else if (near(0.5, note_queue[i+1].init_time - note_queue[i].duration, 0.005)) {
			fputs("r2 ", f);
		} else if (near(1, note_queue[i+1].init_time - note_queue[i].duration, 0.005)) {
			fputs("r1 ", f);
		}

	}
	fputs("\n", f);
	fputs("}", f);
	fclose(f);

	return 0;
}

