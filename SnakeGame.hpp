#pragma once
#include <curses.h>
#include "Snake.hpp"
#include "board.hpp"
#include <chrono>
#include <map>
#include <unistd.h>

#define HALF_SECOND 500000

int getTime(){
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

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
    int height, width, speed;

    Snake snake;
    SnakePiece food;
    // Food position

public:
    SnakeGame(int speed = 125, int height = 15, int width = 30) : 
        speed(speed),  height(height), width(width) {
        

        // set random seed
        srand(getTime());

        snake = Snake(height/2, width/2);
        board.init(height, width);
        keypad(board.getAddress(), true);

        if (board.getCharAt(snake.head.y - 3, snake.head.x) == ' '){
            food.y = snake.head.y - 3;
            food.x  = snake.head.x;
        }

        // Initialize the tail with two tail pieces below
        for (int i=2; i>0; i--) {
            SnakePiece nextPiece(snake.head.y+i, snake.head.x);
            
            snake.addTailPiece(nextPiece);
            board.draw(nextPiece.y, nextPiece.x, '#');
            
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
            food = genNewFood();
        }
        else {
            game_over = true;
        }
    }

    int getScore() {
        return snake.getLength() - 2;
    }

    SnakePiece genNewFood() {
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
        board.draw(snake.head.y, snake.head.x, '@');
        SnakePiece neck = snake.getNeck();
        board.draw(neck.y, neck.x, '#');
        board.draw(food.y, food.x, '*');
        board.refresh();
    }

    void playGame(){
        draw();
        usleep(HALF_SECOND);
        while (!game_over){
            int input = getInput();
            logic(input);
            draw();
        }
    }


};