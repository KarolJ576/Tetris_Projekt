#include "App.h"
#include "MainMenu.h"
#include "LevelSelect.h"
#include "Game.h"

void App::run() {
    while (state != AppState::Exit) {
        switch (state) {
        case AppState::MainMenu:
            showMainMenu();
            break;
        case AppState::LevelSelect:
            showLevelSelect();
            break;
        case AppState::Game:
            showGame();
            break;
        default:
            state = AppState::Exit;
            break;
        }
    }
}

void App::showMainMenu() {
    MainMenu menu(videoMode);
    auto result = menu.run();
    if (!result.first) {
        // ESC - zakończ program
        state = AppState::Exit;
        return;
    }
    videoMode = result.second;
    state = AppState::LevelSelect;
}

void App::showLevelSelect() {
    LevelSelect levelSelect(videoMode);
    int selected = levelSelect.run();
    if (selected == 0)
        state = AppState::MainMenu;
    else {
        chosenLevel = selected;
        state = AppState::Game;
    }
}

void App::showGame() {
    Game game(videoMode, chosenLevel);
    game.run();
    state = AppState::MainMenu;
}
