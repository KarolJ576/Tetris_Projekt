#include "MainMenu.h"
#include <tuple>
#include <cctype>
#include <sstream>

MainMenu::MainMenu(sf::VideoMode currentMode, DatabaseManager& dbRef)
    : chosenMode(currentMode), db(dbRef) {

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
    playText.setPosition(80, 180);
    playText.setFillColor(sf::Color::Black);

    escText.setFont(font);
    escText.setString("Exit (ESC)");
    escText.setCharacterSize(24);
    escText.setPosition(80, 220);
    escText.setFillColor(sf::Color::Black);

    namePrompt.setFont(font);
    namePrompt.setString("Enter your name:");
    namePrompt.setCharacterSize(20);
    namePrompt.setPosition(80, 100);
    namePrompt.setFillColor(sf::Color::Black);

    nameInput.setFont(font);
    nameInput.setCharacterSize(20);
    nameInput.setPosition(80, 130);
    nameInput.setFillColor(sf::Color::Blue);
    nameInput.setString("_");

    // High score text setup
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(18);
    highScoreText.setFillColor(sf::Color::Black);

    auto [bestPlayer, bestScore] = db.getHighScore();
    std::stringstream ss;
    if (!bestPlayer.empty()) {
        ss << "High Score: " << bestPlayer << " - " << bestScore;
    }
    else {
        ss << "High Score: none";
    }

    highScoreText.setString(ss.str());

    // Ustawienie na dole okna
    float yBottom = static_cast<float>(currentMode.height) - 40.f;
    highScoreText.setPosition(20, yBottom);
}

std::tuple<bool, sf::VideoMode, std::string> MainMenu::run() {
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::TextEntered) {
                if (e.text.unicode == '\b') {
                    if (!playerName.empty())
                        playerName.pop_back();
                }
                else if (e.text.unicode < 128 && std::isalnum(e.text.unicode)) {
                    if (playerName.size() < 12)
                        playerName += static_cast<char>(e.text.unicode);
                }
                nameInput.setString(playerName + "_");
            }

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    window.close();
                    return { true, chosenMode, playerName };
                }
                if (e.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return { false, chosenMode, "" };
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(namePrompt);
        window.draw(nameInput);
        window.draw(playText);
        window.draw(escText);
        window.draw(highScoreText); // <--- tu rysujemy wynik
        window.display();
    }

    return { false, chosenMode, "" };
}
