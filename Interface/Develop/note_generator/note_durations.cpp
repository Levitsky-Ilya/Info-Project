#include "note_durations.h"

using namespace std::placeholders;

list<NoteDur> noteDurList = {
	{ 1, std::bind(writer, "16 ", "",    "",    _1)},
	{ 2, std::bind(writer, "8 ",  "",    "",    _1)},
	{ 3, std::bind(writer, "8. ", "",    "",    _1)},
	{ 4, std::bind(writer, "4 ",  "",    "",    _1)},
	{ 5, std::bind(writer, "4~ ", "16 ", "",    _1)},
	{ 6, std::bind(writer, "4. ", "",    "",    _1)},
	{ 7, std::bind(writer, "4~ ", "8. ", "",    _1)},
	{ 8, std::bind(writer, "2 ",  "",    "",    _1)},
	{ 9, std::bind(writer, "2~ ", "16 ", "",    _1)},
	{10, std::bind(writer, "2~ ", "8 ",  "",    _1)},
	{11, std::bind(writer, "2~ ", "8. ", "",    _1)},
	{12, std::bind(writer, "2~ ", "4 ",  "",    _1)},
	{13, std::bind(writer, "2~ ", "4~ ", "16 ", _1)},
	{14, std::bind(writer, "2~ ", "4. ", "",    _1)},
	{15, std::bind(writer, "2~ ", "4 ",  "8. ", _1)},
	{16, std::bind(writer, "1 ",  "",    "",    _1)},
};
