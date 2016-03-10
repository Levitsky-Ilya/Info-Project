#ifndef MYRENDERER_H
#define MYRENDERER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

//using namespace sf;

//working with background
class MyRenderer
{
public:
    MyRenderer();
    //setting background by path
    void setBackground(const std::string & path);
    void setButton(const std::string & path);
    bool isWindowOpen() const;
    void draw();
    void proccessStep();
private:
    sf::Sprite * background_spr;
    sf::Sprite * button1_spr;
    sf::Sprite * button2_spr;
    sf::RenderWindow * window_;

    const int BUTTON_SIZE_X = 199;
    const int BUTTON_SIZE_Y = 199;

};

#endif // MYRENDERER_H
