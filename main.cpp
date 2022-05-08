#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <chrono>
#include "SnakeGame.hpp"
#include "Snake.hpp"
#define TIMEOUT 125
#define HEIGHT 15
#define WIDTH 30

using namespace std;

int main(){

    initscr();
    refresh();
    cbreak();
    noecho();
    curs_set(0);

    SnakeGame game(TIMEOUT, HEIGHT, WIDTH);

    game.playGame();

    endwin();

    return 0;
}
