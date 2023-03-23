#include <ncurses.h>
#include "graphics.h"
#include "logic.h"

extern piece_state g_board[6][7];

int select_option (char** options, int n)
    /**
     * @brief Do a select option tui
     * 
     * @param options text with options
     * @param n number of options to select
     * 
     * @return the index of the selected option
     */
{
    int i;
    int selection = 0;
    int command = '0';

    cursor_t cursor;
    move_cursor(&cursor, 5, 5);
    
    do {
        /* Clear the terminal. */
        clear ();
        for (i = 0; i < n; i++) {
            if (selection == i)
                attrset(A_NORMAL | A_BOLD | A_BLINK);
            else
                attrset(A_NORMAL);

            move(cursor.y + (i*2), cursor.x);
            addstr(options[i]);
            
        }

        command = getch();

        if (command==KEY_UP || command==KEY_DOWN) { // If the first value is esc
            if(command == KEY_UP)
                selection++;
            if(command == KEY_DOWN)
                selection--;
            
            if(selection < 0)
                selection = n-1;
            else if(selection >= n)
                selection = 0;
            
        }
    } while (command != '\n');
    
    clear ();
    return selection;
}

int main (void) {
    cursor_t cursor;

    int sel_column = 1;
    int command = '0';
    int player = PLAYER_1;
    int com_play;
    int is_com_mode;

    char* continue_menu[2] = {
        "Play again",
        "Exit"
    };

    char* mode_menu[2] = {
        "Player vs COM",
        "Player vs Player"
    };

    move_cursor(&cursor, 1, 1);

    start_graphics();
    
    is_com_mode = (select_option(mode_menu, 2) == 0);

    print_board(g_board);
    print_select(cursor, PLAYER_1);
    
    refresh();

    // Selecting the column
    do { // Press esc to exit
        com_play = -1;
        if(player == PLAYER_2 && is_com_mode)
            com_play = select_move(player);
        else {
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
                getch();

                if (select_option (continue_menu, 2))
                    command = 27;
                else {
                    /* Clean board. */
                    int i, j;
                    for (i = 0; i < 6; i++) {
                        for (j = 0; j < 7; j++) {
                            g_board[i][j] = EMPTY;
                        }
                    }

                    /* Reset variables and print board. */
                    sel_column = 1;
                    command = '0';
                    player = PLAYER_1;

                    print_board(g_board);
                    print_select(cursor, PLAYER_1);
                }
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

