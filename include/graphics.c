#include "graphics.h"
#include <ncurses.h>


// Global variables definitions

piece_state g_board[6][7] = {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                             {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                             {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                             {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                             {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                             {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}};


// Functions

void start_graphics(){
    initscr();
    start_color();
    
    init_pair(SPACE_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(PLAYER_1, COLOR_RED, COLOR_BLACK);
    init_pair(PLAYER_2, COLOR_BLUE, COLOR_BLACK);

    curs_set(0); // Invisible cursor
    noecho(); // Dont print getch()
    keypad(stdscr, TRUE); // Get key
}

void end_graphics(){
    endwin();
}

void print_space(cursor_t position){
    /*
        Prints a "space" in the entered position
    */

    attrset(A_NORMAL | COLOR_PAIR(SPACE_COLOR));

    move(position.y, position.x);
    addstr(" ___");

    move(position.y+1, position.x);
    addstr("/   \\");

    move(position.y+2, position.x);
    addstr("\\___/");
}

void print_piece(cursor_t position, int player){
    /*
        Prints a "piece" in the entered position
        with the cikir if the respective player
        PLAYER_1 or PLAYER_2*
    */

    attrset(A_NORMAL | COLOR_PAIR(player) | A_BOLD);

    move(position.y, position.x);
    addstr(" ___");

    move(position.y+1, position.x);
    addstr("d888b");

    move(position.y+2, position.x);
    addstr("q");
    attron(A_UNDERLINE);
    addstr("888");
    attroff(A_UNDERLINE);
    addstr("p");
}

void move_cursor(cursor_t *cursor, int y, int x ){
    /*
        Move the cursor to the entered position
    */
    cursor -> x = x;
    cursor -> y = y;
}
    
void print_board(piece_state board[6][7]){
    /*
        Print the board!
    */

    int i;
    int j;
    int xpos = 1;
    int ypos = 4;

    cursor_t cursor;

    for(i=0; i<6; i++){
        for(j=0; j<7; j++){
            move_cursor(&cursor, ypos, xpos);
            
            if(board[i][j] == EMPTY)
                print_space(cursor);
            else if(board[i][j] == P1)
                print_piece(cursor, PLAYER_1);
            else if(board[i][j] == P2)
                print_piece(cursor, PLAYER_2);
            
            xpos += 9;
        }
        xpos = 1;
        ypos += 3;
    }
}
        
void print_select(cursor_t position, int player){
    /*
        Prints a blinking piece
    */

    attrset(A_NORMAL | COLOR_PAIR(player) | A_BOLD | A_BLINK);

    move(position.y, position.x);
    addstr(" ___");

    move(position.y+1, position.x);
    addstr("d888b");

    move(position.y+2, position.x);
    addstr("q");
    attron(A_UNDERLINE);
    addstr("888");
    attroff(A_UNDERLINE);
    addstr("p");
}

void erase_select(cursor_t position){
    /*
        Erases an 3x5 section (piece size)
    */

   attrset(A_NORMAL);

    move(position.y, position.x);
    addstr("     ");

    move(position.y+1, position.x);
    addstr("     ");

    move(position.y+2, position.x);
    addstr("     ");
}
       
void fall_animation(int pos_x, int player, int distance){
    /**
     * Creates a piece fall animation
     * 
     */

    cursor_t cur;
    cursor_t target;
    int cont;
    int xpos = pos_x;
    int ypos = 1;
    
    move_cursor(&cur, ypos, xpos);
    move_cursor(&target, distance+ypos, xpos);

    for(cont=0; cont<distance; cont++){
        erase_select(cur);
        print_board(g_board);
        ypos += 1;
        move_cursor(&cur, ypos, xpos);
        print_space(target);
        print_piece(cur, player);

        refresh();
        napms(50); // Animation frame rate
    }
}

void win_message(cursor_t position, char *message, int color){
    /**
     * Prints a message
     * 
     */

    char *m;
    m = message;
    int n = 1;
    int sp = 0;

    attrset(A_NORMAL | COLOR_PAIR(color) | A_BOLD);
    move(position.y, position.x);
    addstr("************");
    for(int i=0; i<5; i++){
        move(position.y+i+1, position.x);
        addstr("*          *");
    }
    move(position.y+6, position.x);
    addstr("************");
    move(position.y+2, position.x+2);
    while(*m){
        if(*m == '\n' || sp > 6){
            move(position.y+2+n++, position.x+2);
            sp = 0;
        }
        else{
            sp++;
            addch(*m);
        }
        m++;       
    }
    refresh();
}