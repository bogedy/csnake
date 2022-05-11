#pragma once
#include <queue>

extern int dx[5];
extern int dy[5];
// For quick directions 

enum direction{RIGHT=0, UP, LEFT, DOWN, NONE};

struct SnakePiece {
    int y, x;

    SnakePiece(int y = 1, int x = 1) :
        y(y), x(x) {}
};

class Snake {
    std::queue<SnakePiece> tail;

public:
    direction dir = UP;
    SnakePiece head;
    // Head position
    Snake(int y=1, int x=1) {
        head.y = y;
        head.x = x;
    }

    void addTailPiece(SnakePiece next){
        tail.push(next);
    }

    void popEnd() {
        tail.pop();
    }

    void setDirection(direction d){
        if (d == NONE or (dx[d] + dx[dir] == 0 && dy[d] + dy[dir] == 0)){
            return;
        }
        else dir = d;
    }

    SnakePiece getNext() {
        return SnakePiece(head.y + dy[dir], head.x + dx[dir]);
    }

    SnakePiece getTail() {
        return tail.front();
    }

    SnakePiece getNeck() {
        return tail.back();
    }

    int getLength() {
        return tail.size() + 1;
    }

};