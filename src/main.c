#include <ncurses.h>
#include "graphics.h"
#include "logic.h"


extern piece_state g_board[6][7];

int main(void)
{
    cursor_t cursor;

    int sel_column = 1;
    int command = '0';
    int player = PLAYER_1;
    int com_play;


    move_cursor(&cursor, 1, 1);

    start_graphics();
    
    print_board(g_board);
    print_select(cursor, PLAYER_1);

    
    refresh();

    // Selecting the column
    do { // Press esc to exit
        com_play = -1;
        if(player == PLAYER_2) //COM COM COM
            com_play = select_move(player);

        
        command = getch();

        if(command==KEY_RIGHT || command==KEY_LEFT){ // If the first value is esc
            if(command == KEY_RIGHT)
                sel_column++;
            if(command == KEY_LEFT)
                sel_column--;
            
            if(sel_column < 1)
                sel_column = 7;
            else if(sel_column > 7)
                sel_column = 1;

            erase_select(cursor);            
            move_cursor(&cursor, 1, 1+9*(sel_column-1));
            print_select(cursor, player);

            refresh();
        }

        // If selected a column
        if(command == '\n' || command == ' ' || com_play != -1){
            int row;
            int put_piece_column;

            // Put the piece in the g_board
            if(com_play != -1)
                put_piece_column = com_play;
            else
                put_piece_column = sel_column-1;

            row = put_piece(put_piece_column, player);

            // If column was full
            if(row == -1) 
                continue;
            
            fall_animation(1+put_piece_column*9, player, 3*(row+1));
            
            if(win_game()){
                int tempx;
                int tempy;

                tempy = cursor.y;
                tempx = cursor.x;

                move_cursor(&cursor, 4, 65);
                if(player == PLAYER_1)
                    win_message(cursor, "RED\nWINS", PLAYER_1);
                else if(player == PLAYER_2)
                    win_message(cursor, "BLUE\nWINS", PLAYER_2);
                move_cursor(&cursor, tempy, tempx);
                player = PLAYER_1;
            }
            
            else if(player == PLAYER_1)
                player = PLAYER_2;
            else if(player == PLAYER_2)
                player = PLAYER_1;
            

            print_select(cursor, player);
        }
    } while(command != 27);

    end_graphics();
    return 0;
}

