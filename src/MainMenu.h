#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>
#include "DatabaseManager.h"

class MainMenu {
public:
    MainMenu(sf::VideoMode currentMode, DatabaseManager& dbRef);  // ← poprawiona linia
    std::tuple<bool, sf::VideoMode, std::string> run();

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text title, playText, escText, namePrompt, nameInput, highScoreText;
    sf::VideoMode chosenMode;
    std::string playerName;
    DatabaseManager& db;  // ← przechowujemy referencję do bazy danych
};
