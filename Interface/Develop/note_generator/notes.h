#ifndef NOTES_H
#define NOTES_H

struct Notes
{
	int n;
	float duration;
	float init_time;

	bool operator< (const Notes & rhs) {
		return (init_time == rhs.init_time ?
					duration == rhs.duration ?
						n < rhs.n :
						duration < rhs.duration :
					init_time < rhs.init_time
				);
	}
};

#endif // NOTES_H

