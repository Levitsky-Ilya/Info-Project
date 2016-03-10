#include "background.h"

using namespace sf;

MyRenderer::MyRenderer()
:
    background_spr(new Sprite()),
    button1_spr(new Sprite()),
    button2_spr(new Sprite()),
    window_(new RenderWindow(VideoMode(1280, 720), "My window"))
{
    setBackground("E:/Programs/Qt/Projects/sfml/images/fon_1280x720.jpg");
    setButton("E:/Programs/Qt/Projects/sfml/images/button_violet_1.png");
}

//working with background
//setting background by path
void MyRenderer::setBackground(const std::string & path)
{
    static Image background_img;
    background_img.loadFromFile(path);
    static Texture background;
    background.loadFromImage(background_img);

    background_spr->setTexture(background);
    background_spr->setPosition(0,0);
}


void MyRenderer::setButton(const std::string & path)
{
    static Texture button;
    button.loadFromFile(path);
    button.setSmooth(true);

    button1_spr->setTexture(button);
    // button_spr->setTextureRect(IntRect(Vector2i(500,540), Vector2i(button.getSize())));
    button1_spr->setPosition(500, 540);
    button2_spr->setTexture(button);
    button2_spr->setPosition(750, 540);
}

bool MyRenderer::isWindowOpen() const
{
    return window_->isOpen();
}
void MyRenderer::draw()
{
    window_->draw(*background_spr);
    window_->draw(*button1_spr);
    window_->draw(*button2_spr);
    window_->display();
}

void MyRenderer::proccessStep()
{
    RenderWindow & window = *window_;
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseButtonPressed) {
            if ((event.mouseButton.button == Mouse::Left)) {

                Vector2f pos = Vector2f(Mouse::getPosition(window));
                FloatRect i = button1_spr->getGlobalBounds();
                FloatRect j = button2_spr->getGlobalBounds();
                if (i.contains(pos))
                    std::cout << "You pressed the 1st button!" << std::endl;
                if (j.contains(pos))
                    std::cout << "You pressed the 2nd button!" << std::endl;

                /*
                if((pos.x > 500) && (pos.x < (500 + BUTTON_SIZE_X)) &&
                (pos.y > 500) && (pos.y < (540 + BUTTON_SIZE_Y))) {
                    std::cout << "You pressed the button!" << std::endl;
                }
*/

            }
        }
    }
}
