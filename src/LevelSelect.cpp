#include "LevelSelect.h"
#include <sstream>

LevelSelect::LevelSelect(sf::VideoMode mode) {
    window.create(mode, "Select Level");
    font.loadFromFile("arial.ttf");

    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setString("Enter Level: ");
    inputText.setPosition(100, 100);

    userInput.setFont(font);
    userInput.setCharacterSize(24);
    userInput.setFillColor(sf::Color::Red);
    userInput.setPosition(100, 140);

    backText.setFont(font);
    backText.setCharacterSize(20);
    backText.setFillColor(sf::Color::Blue);
    backText.setString("Back (ESC)");
    backText.setPosition(100, 300);
}

int LevelSelect::run() {
    std::string currentInput;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return 0;
                }
                if (e.key.code == sf::Keyboard::Enter) {
                    if (!currentInput.empty()) {
                        int level = std::stoi(currentInput);
                        window.close();
                        return level;
                    }
                }
                if (e.key.code == sf::Keyboard::BackSpace && !currentInput.empty()) {
                    currentInput.pop_back();
                }
            }
            if (e.type == sf::Event::TextEntered) {
                if (std::isdigit(e.text.unicode)) {
                    currentInput += static_cast<char>(e.text.unicode);
                }
            }
        }

        userInput.setString(currentInput);

        window.clear(sf::Color::White);
        window.draw(inputText);
        window.draw(userInput);
        window.draw(backText);
        window.display();
    }
    return 0;
}
