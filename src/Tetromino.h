#pragma once
#include <SFML/Graphics.hpp>

struct Point {
    int x, y;
};

class Tetromino {
public:
    Tetromino();
    void spawn(int type);
    void move(int dx);
    void rotate();
    void undo();

    bool check(int field[20][10]);
    static std::vector<Point> getShape(int type);

    Point a[4], b[4];
    int type;
private:
    static const int figures[7][4];
};
