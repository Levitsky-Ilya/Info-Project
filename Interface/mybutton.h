#ifndef MYBUTTON
#define MYBUTTON

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Button.hpp>

/*
 * This class is responsible for creating buttons.
 * This class has some derived classes, each for it's own button
 * Function(), which will be defined in the .cpp file of derived
 * class, will contain the set of commands for button.
 * Explicit is written to prevent some compiler magic (to restrain explicit type cast).
 */

class MyButton
{
	public:
		explicit MyButton(std::string const & str);
		virtual ~MyButton() = 0;
		decltype(sfg::Button::Create()) button_;
		virtual void Function() = 0;
		void initialize();
};

#endif // MYBUTTON

