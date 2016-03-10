#include <SFML/Graphics.hpp>
#include <iostream>
#include "background.h"

using namespace sf;

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
