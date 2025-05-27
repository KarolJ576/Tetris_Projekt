#pragma once
#include <SFML/Graphics.hpp>
#include "DatabaseManager.h"

enum class AppState {
    MainMenu,
    LevelSelect,
    Game,
    Exit
};

class App {
public:
    void run();

private:
    AppState state = AppState::MainMenu;
    sf::RenderWindow window;
    sf::VideoMode videoMode = sf::VideoMode(320, 480);
    std::string playerName;
    int chosenLevel = 1;

    DatabaseManager db;

    void showMainMenu();
    void showLevelSelect();
    void showGame();
};
