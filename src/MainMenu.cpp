#include "MainMenu.h"

MainMenu::MainMenu(sf::VideoMode currentMode) : chosenMode(currentMode) {
    window.create(currentMode, "Main Menu");
    font.loadFromFile("arial.ttf");

    title.setFont(font);
    title.setString("TETRIS");
    title.setCharacterSize(36);
    title.setPosition(80, 40);
    title.setFillColor(sf::Color::Black);

    playText.setFont(font);
    playText.setString("Play (Enter)");
    playText.setCharacterSize(24);
    playText.setPosition(80, 150);
    playText.setFillColor(sf::Color::Black);

    escText.setFont(font);
    escText.setString("Exit (ESC)");
    escText.setCharacterSize(20);
    escText.setPosition(80, 200);
    escText.setFillColor(sf::Color::Black);
}

std::pair<bool, sf::VideoMode> MainMenu::run() {
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Enter) {
                    window.close();
                    return { true, chosenMode };
                }
                if (e.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return { false, chosenMode };  // ESC - wyjdź z programu
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(playText);
        window.draw(escText);
        window.display();
    }

    return { false, chosenMode };
}
