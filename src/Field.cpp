#include "Field.h"
#include <algorithm>

Field::Field() {
    reset();
}

void Field::reset() {
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 10; j++)
            data[i][j] = 0;
}

int Field::clearLines(int& score, int& linesCleared, int& level) {
    int k = 19;
    int lines = 0;
    for (int i = 19; i >= 0; i--) {
        int count = 0;
        for (int j = 0; j < 10; j++) {
            if (data[i][j]) count++;
            data[k][j] = data[i][j];
        }
        if (count < 10) k--;
        else {
            lines++;
            linesCleared++;
            score += 100;
            if (linesCleared % 10 == 0) {
                level++;
            }
        }
    }
    return lines;
}
