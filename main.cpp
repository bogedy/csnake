#include <iostream>
#include <unistd.h>
#include <curses.h>

#define TIMEOUT 125

using namespace std;

WINDOW* board;

/*
Aaron's direction method:
int[] dx = [1, 0, -1, 0];
int[] dy = [0, 1, 0, -1];
*/

int height=10;
int width=30;

int head_x=width/2;
int head_y=height/2;

int food_x, food_y, tailn;

int tail_x[400], tail_y[400];

bool gameover=false;

enum directions{RIGHT=0, UP, LEFT, DOWN};
directions dir=UP;

void gen_food(){
    food_x=rand()%width;
    food_y=rand()%height;
}

bool is_tail(int tail_length, int position_y, int position_x){
    for (int k=0; k<tail_length; k++){
        if (position_x==tail_x[k] && position_y==tail_y[k]){
            return true;
        }
    }
    return false;
}

/*

Draw by printing to the terminal screen

So starting in the upper left corner at (0, 0)
and ending at (width-1, height-1). 

Y is inverted.

*/
void draw(){
    wclear(board);
    box(board, 0, 0);
    mvwprintw(board, head_y, head_x, "@");
    for (int i=0; i<tailn; i++){
        mvwprintw(board, tail_y[i], tail_x[i], "#");
    }
    mvwprintw(board, food_y, food_x, "*");

    wrefresh(board);
}

void logic(){

    int oldhead_x=head_x;
    int oldhead_y=head_y;

    switch (dir){
        case RIGHT:
            head_x++;
            break;
        case UP:
            head_y--;
            break;
        case LEFT:
            head_x--;
            break;
        case DOWN:
            head_y++;
            break;
    }

    if (head_x==0 || head_y==0 || head_x==width-1 || head_y==height-1 || is_tail(tailn, head_y, head_x)){
                gameover=true;
            }

    if (head_x==food_x && head_y==food_y){
        int endx=tail_x[tailn-1];
        int endy=tail_y[tailn-1];
        for (int i=tailn-1; i>0; i--){
            tail_x[i]=tail_x[i-1];
            tail_y[i]=tail_y[i-1];
        }
        tailn++;
        tail_x[tailn-1]=endx;
        tail_y[tailn-1]=endy;
        food_x = (rand()%(width-2))+1;
        food_y = (rand()%(height-2))+1;
    }   
    else{
        for (int i=tailn-1; i>0; i--){
        tail_x[i]=tail_x[i-1];
        tail_y[i]=tail_y[i-1];
        }
    }

    tail_x[0]=oldhead_x;
    tail_y[0]=oldhead_y;
}

int getTime(){
    return (int)chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void input(){



    keypad(board, TRUE);
    wtimeout(board, TIMEOUT);
    //halfdelay(10);
    int before  = getTime();
    int c=wgetch(board);
    
    int new_c=ERR;
    wtimeout(board, 0);
    while (getTime()-before<TIMEOUT){
        new_c = wgetch(board);
    }

    if (new_c!=ERR){
        c=new_c;
    }
    switch(c){
        case KEY_RIGHT:
            if (dir==LEFT){
                break;
            }
            else{
                dir=RIGHT;
            }
            break;
        case KEY_UP:
            if (dir==DOWN){
                break;
            }
            else{
                dir=UP;
            }
            break;
        case KEY_LEFT:
            if (dir==RIGHT){
                break;
            }
            else{
                dir=LEFT;
            }
            break;
        case KEY_DOWN:
            if (dir==UP){
                break;
            }
            else{
                dir=DOWN;
            }
            break;
        // press Q to quit:
        case 113:
            gameover=true;
        case ERR:
        default:
            break;
    }
}

void setup(){
/*
    food_x = rand()%width;
    food_y = rand()%width;
*/

    food_x = head_x;
    food_y = head_y-2;

    tailn=2;
    for (int i=0; i<tailn; i++){
        tail_y[i]=head_y+1+i;
        tail_x[i]=head_x;
    }
}

int main(){

    initscr();
    cbreak();
    noecho();
    curs_set(0);    
    board=newwin(height, width, 0, 0);
    WINDOW* debug=newwin(1, width, height+1, 0);
    //keypad(board, true);
    box(board, 0, 0);
    wrefresh(board);

    setup();
    draw();
    usleep(500000);
    
    while (!gameover){
        
        // cout<<"Tail positions: "<<endl;
        // for (int i=0; i<tailn; i++){
        //     cout<<"("<<tail_x[i]<<", "<<tail_y[i]<<") "<<endl;
        // }
        // cout<<"Head position: "<<endl<<"("<<head_x<<", "<<head_y<<")"<<endl;
        int start = getTime();
        input();
        int end = getTime();
        logic();
        draw();
        

        mvwprintw(board, height-1, 0, "time of last logic/draw: %d", (end-start));
        
        //cout<<input()<<endl;
        //sleep(1);

        /* for testing
        gameover=true;*/
    }

    draw();

    cout<<"~~~GAME OVER~~~"<<endl<<"Score: "<<tailn-2<<endl;

    delwin(board);
    endwin();

    return 0;
}
