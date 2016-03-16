#include "myrenderer.h"

using namespace sf;

/* Constructor, sets window parameters and background */
MyRenderer::MyRenderer()
:
    background_spr(new Sprite()),
    window_(new RenderWindow(VideoMode(1280, 720), "My window"))
{
	setBackground("E:/Programs/Qt/Projects/sfml/images/fon_1280x720.jpg");
}

MyRenderer::~MyRenderer()
{
	while (!buttons_.empty()) {
		delete buttons_.back();
		buttons_.pop_back();
	}
}


/* Function responsible for setting background for further displaying in .draw function */
void MyRenderer::setBackground(const std::string & path)
{
    static Image background_img;
    background_img.loadFromFile(path);
    static Texture background;
    background.loadFromImage(background_img);

    background_spr->setTexture(background);
    background_spr->setPosition(0,0);
}


void MyRenderer::setButton()
{
	buttons_.push_back(new StartButton("START"));
}

/*
 * Function that checks if window is opened.
 * This exicts besause window_ is private
 * and we can't use it just like that
 */
bool MyRenderer::isWindowOpen() const
{
    return window_->isOpen();
}

/* Function for drawing object and displaying it */
void MyRenderer::draw()
{
    window_->draw(*background_spr);
    window_->display();
}

/* Function through which GUI will react on different events */
void MyRenderer::proccessStep()
{
	RenderWindow & window = *window_; //creating referance to the object window with type RenderWindow by window_ adress (sort of renaming)
    Event event;
	while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
    }
}
