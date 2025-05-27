#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include <ctime>
#include "Tetromino.h"
#include "Field.h"
#include "Utils.h"
#include <MainMenu.h>
#include <App.h>
#include "DatabaseManager.h"


Game::Game(sf::VideoMode mode, int level, const std::string& playerName)
    : videoMode(mode), startLevel(level), playerName(playerName) {
    window.create(mode, "Tetris");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return;

    if (videoMode.width >= 640) {
        tileSize = 32;
        fieldOffsetX = 160;
        fieldOffsetY = 0;
    }
    else {
        tileSize = 16;
        fieldOffsetX = 80;
        fieldOffsetY = 0;
    }


    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);
    scoreText.setString("Score: 0");
}

void Game::run() {
    srand(static_cast<unsigned>(time(0)));

    sf::Clock clock;
    float timer = 0;
    float delay;

    int currentType = rand() % 7;
    int nextType = rand() % 7;
    int score = 0, linesCleared = 0, level = startLevel;
    delay = getDelayForLevel(level);

    bool rotate = false, gameOver = false, paused = false;
    float dx = 0;

    sf::Vector2f offset(28, 31);

    Field field;
    Tetromino tetromino;

    sf::RectangleShape block(sf::Vector2f(16, 16));
    block.setOutlineThickness(1);
    block.setOutlineColor(sf::Color::Black);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return;

    sf::Text scoreText, levelText, pausedText, gameOverText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(250, 20);

    levelText.setFont(font);
    levelText.setCharacterSize(14);
    levelText.setFillColor(sf::Color::Black);
    levelText.setPosition(250, 70);

    pausedText.setFont(font);
    pausedText.setCharacterSize(24);
    pausedText.setFillColor(sf::Color::Blue);
    pausedText.setString("Paused");
    pausedText.setPosition(70, 100);

    sf::Text nextLabel;
    nextLabel.setFont(font);
    nextLabel.setCharacterSize(14);
    nextLabel.setFillColor(sf::Color::Black);
    nextLabel.setString("Next:");
    nextLabel.setPosition(250, 120);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(70, 150);

    sf::RectangleShape border(sf::Vector2f(10 * 16, 20 * 16));
    border.setPosition(offset);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(2);

    auto spawnNew = [&]() {
        currentType = nextType;
        nextType = rand() % 7;

        tetromino.spawn(currentType);
        if (!tetromino.check(field.data)) {
            gameOver = true;
            std::stringstream msg;
            msg << "Game Over!\nScore: " << score << "\nLevel: " << level
                << "\nPress R to Retry\nPress ESC to Exit";
            gameOverText.setString(msg.str());
        }
        };

    spawnNew();

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        if (!paused && !gameOver) timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (gameOver) {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        static DatabaseManager db;
                        db.saveScore(playerName, score, level);
                        window.close();
                    }
                    else if (event.key.code == sf::Keyboard::R) {
                        field.reset();
                        score = linesCleared = 0;
                        level = startLevel;
                        delay = getDelayForLevel(level);
                        gameOver = false;
                        gameOverText.setString("");
                        spawnNew();
                    }
                }
                else {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    if (event.key.code == sf::Keyboard::P)
                        paused = !paused;
                    if (!paused) {
                        if (event.key.code == sf::Keyboard::Left) dx = -1;
                        if (event.key.code == sf::Keyboard::Right) dx = 1;
                        if (event.key.code == sf::Keyboard::Up) rotate = true;
                    }
                }
            }
        }

        if (!paused && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            delay = 0.05f;

        if (!paused && !gameOver) {
            tetromino.move(static_cast<int>(dx));
            if (!tetromino.check(field.data)) tetromino.undo();

            if (rotate) {
                tetromino.rotate();
                if (!tetromino.check(field.data)) tetromino.undo();
            }

            if (timer > delay) {
                for (int i = 0; i < 4; i++) tetromino.b[i] = tetromino.a[i];
                for (int i = 0; i < 4; i++) tetromino.a[i].y += 1;

                if (!tetromino.check(field.data)) {
                    for (int i = 0; i < 4; i++)
                        field.data[tetromino.b[i].y][tetromino.b[i].x] = tetromino.type + 1;
                    spawnNew();
                }

                timer = 0;
                delay = getDelayForLevel(level);
            }

            field.clearLines(score, linesCleared, level);
        }

        dx = 0;
        rotate = false;

        std::stringstream ss;
        ss << "Score:\n" << score;
        scoreText.setString(ss.str());

        std::stringstream lvlss;
        lvlss << "Level: " << level << "\nNext in: " << (10 - linesCleared % 10);
        levelText.setString(lvlss.str());

        window.clear(sf::Color::White);
        window.draw(border);
        window.draw(scoreText);
        window.draw(levelText);
        window.draw(nextLabel);

        auto previewShape = Tetromino::getShape(nextType);
        for (const auto& p : previewShape) {
            block.setFillColor(getColor(nextType));
            block.setPosition(250 + p.x * 16, 140 + p.y * 16);
            window.draw(block);
        }

        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                if (field.data[i][j] == 0) continue;
                block.setFillColor(getColor(field.data[i][j] - 1));
                block.setPosition(j * 16 + offset.x, i * 16 + offset.y);
                window.draw(block);
            }
        }

        if (!gameOver) {
            for (int i = 0; i < 4; i++) {
                block.setFillColor(getColor(tetromino.type));
                block.setPosition(tetromino.a[i].x * 16 + offset.x, tetromino.a[i].y * 16 + offset.y);
                window.draw(block);
            }
        }

        if (paused && !gameOver)
            window.draw(pausedText);

        if (gameOver)
            window.clear(sf::Color::White);
        window.draw(gameOverText);

        window.display();
    }
}