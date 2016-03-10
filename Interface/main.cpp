#include "SFML/Graphics.hpp"
#include <iostream>
#include "background.h"

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
