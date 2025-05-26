#pragma once
#include <SFML/Graphics.hpp>

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
    int chosenLevel = 1;

    void showMainMenu();
    void showLevelSelect();
    void showGame();
};
