#pragma once
#include <SFML/Graphics.hpp>

class LevelSelect {
public:
    LevelSelect(sf::VideoMode mode);
    int run();

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text inputText;
    sf::Text userInput;
    sf::Text backText;
};
