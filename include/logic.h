#ifndef _LOGIC_H_
#define _LOGIC_H_

    // Constant definitions

    // Type definitions

    // Global variables

    // Function prototypes
    int win_game();
        /**
         * returns 1 if there are 4 
         * pieces in line, 0 otherwise
         * 
         */

    int win_condition(int player);
        /**
         * Returns in binary, the columns
         * where, if played, the player would
         * win the game
         * 
         */
    
    int select_move(int player);
        /**
         * Returns a number from 0 to 6 with
         * the next game move acording to the
         * actual g_board
         * 
         */

    int put_piece(int column, int player);
        /**
         * puts the piece on the selected column,
         * returns the row where the piece was put
         * -1 if the column is full.
         * 
         */

    int rand_position(int available);
        /**
         * Returns a random position from the
         * available vector 0011010 (example)
         * would return 2, 3 or 5
         * 
         */

#endif