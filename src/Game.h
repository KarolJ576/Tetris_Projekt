#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game(sf::VideoMode mode, int level);
    void run();
    

private:
    sf::RenderWindow window;
    sf::VideoMode videoMode;
    int tileSize;
    int fieldOffsetX;
    int fieldOffsetY;
    int startLevel;
};
