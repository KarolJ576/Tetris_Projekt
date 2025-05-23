#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <sstream>

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };
int score = 0;
int linesCleared = 0;
int level = 1;

struct Point {
    int x, y;
} a[4], b[4];

int figures[7][4] = {
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5  // O
};

bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

sf::Color getColor(int shape) {
    switch (shape) {
    case 0: return sf::Color::Cyan;       // I
    case 1: return sf::Color::Red;        // Z
    case 2: return sf::Color::Green;      // S
    case 3: return sf::Color::Magenta;    // T
    case 4: return sf::Color(255, 165, 0); // L (orange)
    case 5: return sf::Color::Blue;       // J
    case 6: return sf::Color::Yellow;     // O
    default: return sf::Color::White;
    }
}

float getDelayForLevel(int level) {
    return std::max(0.05f, 0.5f - (level - 1) * 0.05f);
}

void resetGame(int& score, int& linesCleared, int& level, int field[M][N], bool& gameOver, float& delay) {
    score = 0;
    linesCleared = 0;
    level = 1;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            field[i][j] = 0;
    gameOver = false;
    delay = getDelayForLevel(level);
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris");

    sf::Clock clock;
    float timer = 0, delay = getDelayForLevel(level);

    int current = rand() % 7;
    float dx = 0;
    bool rotate = false;
    bool gameOver = false;
    bool paused = false;

    sf::Vector2f offset(28, 31);
    sf::RectangleShape block(sf::Vector2f(16, 16));
    block.setOutlineThickness(1);
    block.setOutlineColor(sf::Color::Black);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return -1;

    sf::Text scoreText, levelText, gameOverText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(250, 20);

    levelText.setFont(font);
    levelText.setCharacterSize(14);
    levelText.setFillColor(sf::Color::Black);
    levelText.setPosition(250, 70);

    sf::Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(24);
    pausedText.setFillColor(sf::Color::Blue);
    pausedText.setString("Paused");
    pausedText.setPosition(70, 100);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(70, 150);
    gameOverText.setString("Game Over!\nScore: " + std::to_string(score) + "\nLevel: " + std::to_string(level) +"\nPress R to Retry\nPress ESC to Exit");

    sf::RectangleShape border(sf::Vector2f(N * 16, M * 16));
    border.setPosition(offset);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(2);

    auto spawnNewBlock = [&]() {
        current = rand() % 7;
        for (int i = 0; i < 4; i++) {
            a[i].x = figures[current][i] % 2 + N / 2 - 1;
            a[i].y = figures[current][i] / 2;
        }
        if (!check()) gameOver = true;
        };

    spawnNewBlock();

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        if (!paused && !gameOver) timer += time;

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed) {
                if (gameOver) {
                    if (e.key.code == sf::Keyboard::Escape)
                        window.close();
                    else if (e.key.code == sf::Keyboard::R) {
                        resetGame(score, linesCleared, level, field, gameOver, delay);
                        spawnNewBlock();
                    }
                }
                else {
                    if (e.key.code == sf::Keyboard::P)
                        paused = !paused;
                    if (!paused) {
                        if (e.key.code == sf::Keyboard::Up) rotate = true;
                        else if (e.key.code == sf::Keyboard::Left) dx = -1;
                        else if (e.key.code == sf::Keyboard::Right) dx = 1;
                    }
                }
            }
        }

        if (!paused && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05f;

        if (!paused && !gameOver) {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

            if (rotate) {
                Point p = a[1];
                for (int i = 0; i < 4; i++) {
                    int x = a[i].y - p.y;
                    int y = a[i].x - p.x;
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }
                if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
            }

            if (timer > delay) {
                for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

                if (!check()) {
                    for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = current + 1;
                    spawnNewBlock();
                }
                timer = 0;
            }

            int k = M - 1;
            for (int i = M - 1; i >= 0; i--) {
                int count = 0;
                for (int j = 0; j < N; j++) {
                    if (field[i][j]) count++;
                    field[k][j] = field[i][j];
                }
                if (count < N) k--;
                else {
                    linesCleared++;
                    score += 100;
                    if (linesCleared % 10 == 0) {
                        level++;
                        delay = getDelayForLevel(level);
                    }
                }
            }
        }

        dx = 0;
        rotate = false;
        delay = getDelayForLevel(level);

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

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) continue;
                block.setFillColor(getColor(field[i][j] - 1));
                block.setPosition(j * 16 + offset.x, i * 16 + offset.y);
                window.draw(block);
            }
        }

        if (paused && !gameOver) {
            window.draw(pausedText);
        }

        if (!gameOver) {
            for (int i = 0; i < 4; i++) {
                block.setFillColor(getColor(current));
                block.setPosition(a[i].x * 16 + offset.x, a[i].y * 16 + offset.y);
                window.draw(block);
            }
        }
        else {
            window.clear(sf::Color::White);
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
