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
    
    MainMenu menu(videoMode, db);
    auto [playChosen, mode, name] = menu.run();

    if (playChosen) {
        playerName = name; 
        videoMode = mode;  
        state = AppState::LevelSelect; 
    }
    else {
        state = AppState::Exit;
    }
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
    Game game(videoMode, chosenLevel, playerName); 
    game.run();

    state = AppState::MainMenu;
}
