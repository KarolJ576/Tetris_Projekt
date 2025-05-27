#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Game {
public:
    Game(sf::VideoMode mode, int level, const std::string& playerName); // dodano playerName
    void run();

private:
    sf::RenderWindow window;
    sf::VideoMode videoMode;
    int tileSize;
    int fieldOffsetX;
    int fieldOffsetY;
    int startLevel;
    std::string playerName; // dodano pole do przechowywania imienia
};
