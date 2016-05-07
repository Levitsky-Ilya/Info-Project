#ifndef NOTE_PAUSES_H
#define NOTE_PAUSES_H

#include <string>
#include <list>

using namespace std;

/* Giving it's own text equivalent for each pause duration */
struct NotePause {
	float pauseDur;
	string pauseName;
};

extern list<NotePause> note_pause_list;

#endif // NOTE_PAUSES_H
