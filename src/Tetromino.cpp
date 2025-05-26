#include "Tetromino.h"

const int Tetromino::figures[7][4] = {
    1,3,5,7, 2,4,5,7, 3,5,4,6, 3,5,4,7,
    2,3,5,7, 3,5,7,6, 2,3,4,5
};

Tetromino::Tetromino() {
    spawn(rand() % 7);
}

void Tetromino::spawn(int t) {
    type = t;
    auto shape = getShape(type);
    for (int i = 0; i < 4; i++) {
        a[i].x = shape[i].x + 3;
        a[i].y = shape[i].y;
    }
}


void Tetromino::move(int dx) {
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }
}

void Tetromino::rotate() {
    Point p = a[1];
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        int x = a[i].y - p.y;
        int y = a[i].x - p.x;
        a[i].x = p.x - x;
        a[i].y = p.y + y;
    }
}

void Tetromino::undo() {
    for (int i = 0; i < 4; i++) a[i] = b[i];
}

bool Tetromino::check(int field[20][10]) {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= 10 || a[i].y >= 20) return false;
        if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

std::vector<Point> Tetromino::getShape(int type) {
    static const std::vector<std::vector<Point>> shapes = {
        {{0,1}, {1,1}, {2,1}, {3,1}}, // I
        {{0,1}, {1,1}, {1,0}, {2,0}}, // Z
        {{0,0}, {1,0}, {1,1}, {2,1}}, // S
        {{0,1}, {1,1}, {2,1}, {1,0}}, // T
        {{0,0}, {0,1}, {1,1}, {2,1}}, // L
        {{0,1}, {1,1}, {2,1}, {2,0}}, // J
        {{0,0}, {1,0}, {0,1}, {1,1}}  // O
    };

    return shapes[type % shapes.size()];
}
