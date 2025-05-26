#pragma once
#include <SFML/Graphics.hpp>
#include <utility>

class MainMenu {
public:
    MainMenu(sf::VideoMode currentMode);
    std::pair<bool, sf::VideoMode> run();

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text title, playText, escText;

    sf::VideoMode chosenMode;
};
