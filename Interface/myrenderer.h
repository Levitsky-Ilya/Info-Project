#ifndef MYRENDERER_H
#define MYRENDERER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Button.hpp>

/*
 * This class displays background by it's path
 * and also responsible for window rendering
 */
class MyRenderer
{
    public:
        MyRenderer();
        void setBackground(const std::string & path);
        void setButton();
        bool isWindowOpen() const;
        void draw();
        void proccessStep();

    private:
        sf::Sprite * background_spr;
        sf::RenderWindow * window_;
};

#endif // MYRENDERER_H
