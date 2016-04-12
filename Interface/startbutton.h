#ifndef STARTBUTTON
#define STARTBUTTON

#include "mybutton.h"

/*
 * Class to work with START button.
 * Using MyButton class as a parent class
 */
class StartButton: public MyButton
{
	public:
		StartButton(std::string const & str);
		~StartButton();
		void Function();
};

#endif // STARTBUTTON

