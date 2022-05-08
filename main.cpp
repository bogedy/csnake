#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <chrono>
#include "SnakeGame.hpp"
#include "Snake.hpp"
#define TIMEOUT 125

using namespace std;

int main(){

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    SnakeGame game;

    game.playGame();

    endwin();

    return 0;
}
