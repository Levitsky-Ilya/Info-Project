#include "SFML/Graphics.hpp"
#include <iostream>
#include "myrenderer.h"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Button.hpp>

int main()
{
    MyRenderer mr;

    mr.draw();
    while (mr.isWindowOpen()) {
        mr.proccessStep();
        mr.draw();
    }
    return 0;
}
