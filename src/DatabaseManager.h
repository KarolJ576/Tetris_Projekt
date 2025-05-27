#pragma once
#include <string>
#include <utility> // for std::pair
#include <sqlite3.h>

class DatabaseManager {
public:
    DatabaseManager();                     // domyślny konstruktor
    ~DatabaseManager();                    // destruktor zamykający bazę

    void createTables();                   // tworzy tabele, jeśli nie istnieją
    void saveScore(const std::string& playerName, int score, int level);  // zapisuje wynik
    std::pair<std::string, int> getHighScore();  // zwraca najlepszy wynik (nazwa, punkty)

    // zakaz kopiowania
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    sqlite3* db = nullptr;
};
