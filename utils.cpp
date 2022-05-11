#include <chrono>
#include <map>
#include <ncurses.h>

#include "SnakeGame.hpp"
#include "Snake.hpp"

int getTime(){
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


int dx[5] = {1, 0, -1, 0, 0};
int dy[5] = {0, -1, 0, 1, 0};

std::map<int, direction> keyMapper = {
    {KEY_RIGHT, RIGHT},
    {KEY_UP, UP},
    {KEY_LEFT, LEFT},
    {KEY_DOWN, DOWN},
    {ERR, NONE}
};