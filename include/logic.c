#include "logic.h"
#include "graphics.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

extern piece_state g_board[6][7];


int win_game(){
    /**
     * returns 1 if there are 4 
     * pieces in line, 0 otherwise
     * 
     */

    int i;
    int j;

    // Vertical check
    for(i=0; i<3; i++){
        for(j=0; j<7; j++){
            if(   g_board[i][j]   == EMPTY
               || g_board[i+1][j] == EMPTY
               || g_board[i+2][j] == EMPTY
               || g_board[i+3][j] == EMPTY)
               continue;
            if(   g_board[i][j]   == g_board[i+1][j]
               && g_board[i+1][j] == g_board[i+2][j]
               && g_board[i+2][j] == g_board[i+3][j])
               return 1;
        }
    }

    // Horizontal check
    for(i=0; i<6; i++){
        for(j=0; j<4; j++){
            if(   g_board[i][j]   == EMPTY
               || g_board[i][j+1] == EMPTY
               || g_board[i][j+2] == EMPTY
               || g_board[i][j+3] == EMPTY)
               continue;
            if(   g_board[i][j]   == g_board[i][j+1]
               && g_board[i][j+1] == g_board[i][j+2]
               && g_board[i][j+2] == g_board[i][j+3])
               return 1;
        }
    }

    // south-east check
    for(i=0; i<3; i++){
        for(j=0; j<4; j++){
            if(   g_board[i][j]     == EMPTY
               || g_board[i+1][j+1] == EMPTY
               || g_board[i+2][j+2] == EMPTY
               || g_board[i+3][j+3] == EMPTY)
               continue;
            if(   g_board[i][j]     == g_board[i+1][j+1]
               && g_board[i+1][j+1] == g_board[i+2][j+2]
               && g_board[i+2][j+2] == g_board[i+3][j+3])
               return 1;
        }
    }

    // south-west check
    for(i=0; i<3; i++){
        for(j=4; j<7; j++){
            if(   g_board[i][j]     == EMPTY
               || g_board[i+1][j-1] == EMPTY
               || g_board[i+2][j-2] == EMPTY
               || g_board[i+3][j-3] == EMPTY)
               continue;
            if(   g_board[i][j]     == g_board[i+1][j-1]
               && g_board[i+1][j-1] == g_board[i+2][j-2]
               && g_board[i+2][j-2] == g_board[i+3][j-3])
               return 1;
        }
    }

    return 0;
}

int win_condition(int player){
    /**
     * Returns in binary, the columns
     * where, if played, the player would
     * win the game (in 1) 0100100
     * 
     */

    int response = 0;

    for(int i=0; i<7; i++){
        int row;

        row = put_piece(i, player);
        if(row == -1) // If column was full
            continue;

        if(win_game())
            response |= (1 << (6-i));
        g_board[row][i] = EMPTY;
    }

    return response;
}

int select_move(int player){
    /**
     * Returns a number from 0 to 6 with
     * the next game move acording to the
     * actual g_board
     * 
     */

    int enemy;
    int win_cond = 0;
    static int win_column = 0;


    if(player == PLAYER_1)
        enemy = PLAYER_2;
    else
        enemy = PLAYER_1;


    // If win condition
    win_cond = win_condition(player);
    if(win_cond)
        return rand_position(win_cond);

    // If enemy would win otherwise
    win_cond = win_condition(enemy);
    if(win_cond)
        return rand_position(win_cond);
    
    // Forward lose condition
    win_cond = 0;
    for(int i=0; i<7; i++){
        int row;

        row = put_piece(i, player);
        if(row == -1){
            win_cond |= (1 << (6-i));
            continue;
        }
        if(win_condition(enemy))
            win_cond |= (1 << (6-i));
        g_board[row][i] = EMPTY; 
    }

    // Choose the less worst
    win_cond = 127 - win_cond;
    if(win_cond)
        return rand_position(win_cond);

}

int put_piece(int column, int player){
    /**
     * puts the piece on the selected column,
     * returns the row where the piece was put
     * -1 if the column is full.
     * 
     */

    int row_count = 0;

    if(g_board[0][column] != EMPTY)
        return -1;
    else{
        while(row_count<5 && g_board[row_count+1][column] == EMPTY){
            row_count++;
        }
    }

    if(player == PLAYER_1)
        g_board[row_count][column] = P1;
    else if(player == PLAYER_2)
        g_board[row_count][column] = P2;
    else
        g_board[row_count][column] = EMPTY;

    return row_count;
}


int rand_position(int available){
    /**
     * Returns a random position from the
     * available vector 0011010 (example)
     * would return 2, 3 or 5
     * 
     */

    static int flag = 1;
    int positions = 0;
    int choice = -1;

    // Do it once
    if(flag){
        srand(time(0));
        flag = 0;
    }

    if(available == 0)
        return rand()%6;

    for(int c=0; c<7; c++){
        if(available & (1 << (6-c)))
            positions++;
    }

    choice = rand()%positions;

    positions = 0;
    for(int c=0; c<7; c++){
        if(available & (1 << (6-c)))
            positions++;
        if(positions-1 == choice)
            return c;
    }
}
