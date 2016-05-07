#ifndef NOTES_H
#define NOTES_H

struct Notes
{
	int nNote;
	float duration;
	float initTime;

	bool operator< (const Notes & rhs) {
		return (initTime == rhs.initTime ?
					duration == rhs.duration ?
						nNote < rhs.nNote :
						duration < rhs.duration :
					initTime < rhs.initTime
				);
	}
};

#endif // NOTES_H

