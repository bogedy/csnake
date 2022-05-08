#pragma once
#include <curses.h>
#include <Snake.hpp>
#include <chrono>
#include <map>

int getTime(){
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

enum direction{RIGHT=0, UP, LEFT, DOWN, NONE};

std::map<int, direction> keyMapper = {
    {KEY_RIGHT, RIGHT},
    {KEY_UP, UP},
    {KEY_LEFT, LEFT},
    {KEY_DOWN, DOWN},
    {ERR, NONE}
};

class SnakeGame {
    Board board;
    bool game_over = false;
    int height, width; 
    int speed;

    Snake snake;
    SnakePiece food;
    // Food position

public:
    SnakeGame(int speed = 125, int height = 30, int width = 30) : 
        speed(speed),  height(height), width(width) {
        snake = Snake(height/2, width/2);
        board.init(height, width);

        // Initialize the tail with two tail pieces below
        for (int i=0; i<2; i++) {
            SnakePiece nextPiece(snake.head.y+i+1, snake.head.x);
            
            snake.addTailPiece(nextPiece);
            board.draw(snake.head.y+i+1, snake.head.x, '#');
            
            board.refresh();
        }
    }

    int getInput(){
        board.setTimeout(speed);
        int c = ERR;
        int before = getTime();
        c = board.getChIn();
        
        // Move at a constant rate
        // This code below looks for new inputs while waiting out the time
        int new_c=ERR;
        board.setTimeout(0);
        while (getTime() - before < speed){
            new_c = board.getChIn();
        }
        if (new_c != ERR){
            c = new_c;
        }
        return c;
    }

    void logic(int input) {
        snake.setDirection(keyMapper[input]);
        SnakePiece next = snake.getNext();

        if (board.getCharAt(next.y, next.x) == ' '){
            SnakePiece tail = snake.getTail();
            board.draw(tail.y, tail.x, ' ');
            snake.popEnd();
            snake.addTailPiece(snake.head);
            snake.head = next;
        }
        else if (board.getCharAt(next.y, next.x) == '*') {
            snake.addTailPiece(snake.head);
            snake.head = next;
            genNewFood();
        }
        else {
            game_over = true;
        }
    }

    int getScore() {
        return snake.getLength() - 2;
    }

    SnakePiece genNewFood() {
        // TODO on new food
        std::vector<SnakePiece> blank_spaces;
        for (int i = 1; i < height - 1; i++){
            for (int j = 1; j < width - 1; j++){
                if (board.getCharAt(i, j) == ' '){
                    blank_spaces.emplace_back(i, j);
                }
            }
        }
        int random_n = rand() % blank_spaces.size();

        return blank_spaces[random_n];

    }

    void draw() {
        // TODO: draw and call refresh
        board.draw(snake.head.y, snake.head.x, '@');
        SnakePiece neck = snake.getTail();
        board.draw(neck.y, neck.x, '#');
        board.draw(food.y, food.x, '*');
        board.refresh();
    }

    void playGame(){
        while (!game_over){
            int input = getInput();
            logic(input);
            draw();
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

    int getChIn() {
        return wgetch(wboard);
    }

    void setTimeout(int speed){
        wtimeout(wboard, speed);
    }

    void draw(int y, int x, char c){
        mvwprintw(wboard, y, x, &c);
    }

    void refresh() {
        wrefresh(wboard);
    }

    char getCharAt(int y, int x) {
        return mvwinch(wboard, y, x);
    }
};