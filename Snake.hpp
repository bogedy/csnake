#pragma once
#include <queue>

int dx[5] = {1, 0, -1, 0, 1};
int dy[5] = {0, -1, 0, 1, 1};
// For quick directions 

class Snake {
    std::queue<SnakePiece> tail;

public:
    direction dir = UP;
    int mY, mX;
    // Head position
    Snake(int y=1, int x=1) :
        mY(y), mX(x) {} 

    void addPiece(SnakePiece next){
        tail.push(next);
    }

    void popEnd() {
        tail.pop();
    }

    void setDirection(direction d){
        if (dx[d] + dx[dir] == 0 && dy[d] + dy[dir] == 0){
            return;
        }
        else dir = d;
    }

    SnakePiece getNext() {
        return SnakePiece(mY + dy[dir], mX + dx[dir]);
    }

};

struct SnakePiece {
    int y, x;

    SnakePiece(int y, int x) :
        y(y), x(x) {}
};