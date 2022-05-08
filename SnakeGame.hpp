#pragma once
#include <curses.h>
#include <Snake.hpp>

class SnakeGame {
    Board board;
    bool game_over;
    int height, width; 
    int speed;

    Snake snake;
    // Food position
    int food_x, food_y;

public:
    SnakeGame(int speed = 125, int height = 30, int width = 30) : 
        speed(speed),  height(height), width(width) {
        snake = Snake(height/2, width/2);
        board.init(height, width);

        // Initialize the tail with two tail pieces below
        for (int i=0; i<2; i++) {
            SnakePiece nextPiece(snake.mY+i+1, snake.mX);
            
            snake.addPiece(nextPiece);
            board.draw(snake.mY+i+1, snake.mX, '#');
            
            board.refresh();
        }
    }


};

class Board {
    WINDOW* wboard;

public:  
    int mheight, mwidth;

    void init(int height = 30, int width = 30) {
        mheight = height;
        mwidth = width;
        wboard = newwin(height, width, 0, 0);
        box(wboard, 0, 0);
    }

    ~Board() {
        delwin(wboard);
    }

    void draw(int y, int x, char c){
        mvwprintw(wboard, y, x, &c);
    }

    void refresh() {
        wrefresh(wboard);
    }

    chtype getChar(int y, int x) {
        return mvwinch(wboard, y, x);
    }
};