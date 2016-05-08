#ifndef NOTE_DURATIONS
#define NOTE_DURATIONS

#include <iostream>
#include <string>
#include <list>
#include <functional>
#include <global.h>

/*
 * Giving it's own text equivalent for each note duration.
 * Function "writer" is being used for correct displaying.
 */
struct NoteDur {
	unsigned int noteDur;
	std::function<string(string)> func;
};

extern list<NoteDur> noteDurList;

#endif // NOTE_DURATIONS
