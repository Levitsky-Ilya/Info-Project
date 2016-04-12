#include "mybutton.h"

/* Constructor, creating the button via initialization list */
MyButton::MyButton(std::string const & str)
	:
	  button_(sfg::Button::Create(str))
{}

/* Destructor */
MyButton::~MyButton() {}

/* Connecting GetSignal with Function() */
void MyButton::initialize()
{
	button_->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Function, this));
}

void MyButton::Function() {}
