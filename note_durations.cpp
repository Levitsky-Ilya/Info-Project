#include "note_durations.h"

using namespace std::placeholders;

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
