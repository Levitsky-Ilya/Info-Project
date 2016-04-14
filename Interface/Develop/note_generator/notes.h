#ifndef NOTES_H
#define NOTES_H

struct Notes
{
	float freq;
	int n;
	float duration;
	float init_time;

	bool operator< (const Notes & rhs) {
		return (init_time == rhs.init_time ?
					duration == rhs.duration ?
						freq < rhs.freq :
						duration < rhs.duration :
					init_time < rhs.init_time
				);
	}
};

#endif // NOTES_H

