#ifndef _GLOBAL_VARIABLES_
#define _GLOBAL_VARIABLES_
#include <SFML/Graphics.hpp>

    unsigned int width = 800;
    unsigned int height = 600;
    unsigned short charSize = 24;
    std::string initial = "shell> ";
    std::string displayString = initial;
    sf::Text text;

    sf::RectangleShape cursor(sf::Vector2f(10.f, 26.f)); // Width of 2px, height of 26px



#endif