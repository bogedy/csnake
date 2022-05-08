#pragma once
#include <ncurses.h>

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