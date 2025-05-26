#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

inline sf::Color getColor(int shape) {
    switch (shape) {
    case 0: return sf::Color::Cyan;
    case 1: return sf::Color::Red;
    case 2: return sf::Color::Green;
    case 3: return sf::Color::Magenta;
    case 4: return sf::Color(255, 165, 0);
    case 5: return sf::Color::Blue;
    case 6: return sf::Color::Yellow;
    default: return sf::Color::White;
    }
}

inline float getDelayForLevel(int level) {
    return std::max(0.05f, 0.5f - (level - 1) * 0.05f);
}
