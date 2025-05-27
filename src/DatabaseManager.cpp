#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() {
    if (sqlite3_open("tetris.db", &db) != SQLITE_OK) {
        std::cerr << "Błąd otwierania bazy danych: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        createTables();
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

void DatabaseManager::createTables() {
    const char* playerTable = R"(
        CREATE TABLE IF NOT EXISTS Player (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        );
    )";

    const char* scoresTable = R"(
        CREATE TABLE IF NOT EXISTS Scores (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            player_id INTEGER NOT NULL,
            score INTEGER NOT NULL,
            level INTEGER NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (player_id) REFERENCES Player(id)
        );
    )";

    char* errMsg = nullptr;

    if (sqlite3_exec(db, playerTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Błąd tworzenia tabeli Player: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, scoresTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Błąd tworzenia tabeli Scores: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void DatabaseManager::saveScore(const std::string& playerName, int score, int level) {
    if (!db) return;

    sqlite3_stmt* stmt = nullptr;
    int playerId = -1;

    // Sprawdź, czy gracz istnieje
    const char* selectPlayerSql = "SELECT id FROM Player WHERE name = ?";
    if (sqlite3_prepare_v2(db, selectPlayerSql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            playerId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    // Dodaj gracza, jeśli nie istnieje
    if (playerId == -1) {
        const char* insertPlayerSql = "INSERT INTO Player (name) VALUES (?)";
        if (sqlite3_prepare_v2(db, insertPlayerSql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                playerId = static_cast<int>(sqlite3_last_insert_rowid(db));
            }
            sqlite3_finalize(stmt);
        }
    }

    // Zapisz wynik
    if (playerId != -1) {
        const char* insertScoreSql = "INSERT INTO Scores (player_id, score, level) VALUES (?, ?, ?)";
        if (sqlite3_prepare_v2(db, insertScoreSql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, playerId);
            sqlite3_bind_int(stmt, 2, score);
            sqlite3_bind_int(stmt, 3, level);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Błąd przy zapisie wyniku: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_finalize(stmt);
        }
    }
}

std::pair<std::string, int> DatabaseManager::getHighScore() {
    std::pair<std::string, int> result{ "Brak", 0 };

    const char* sql = R"(
        SELECT Player.name, MAX(Scores.score)
        FROM Scores
        JOIN Player ON Player.id = Scores.player_id
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* name = sqlite3_column_text(stmt, 0);
            int score = sqlite3_column_int(stmt, 1);
            if (name) {
                result = { reinterpret_cast<const char*>(name), score };
            }
        }
        sqlite3_finalize(stmt);
    }

    return result;
}
