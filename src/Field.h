#pragma once

class Field {
public:
    Field();
    void reset();
    int clearLines(int& score, int& linesCleared, int& level);

    int data[20][10];
};
