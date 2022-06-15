#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

    // Constant definitions
    #define SPACE_COLOR 0
    #define PLAYER_1 1
    #define PLAYER_2 2

    // Type definitions
    struct cursor_t {
        int x;
        int y;
    };
    typedef struct cursor_t cursor_t;

    typedef enum {
        EMPTY,
        P1,
        P2
    } piece_state;

    // Global variables declaration
    extern piece_state g_board[6][7];


    // ****************** Function prototypes ******************

    void start_graphics();
        /*
            Initializes the ncurses library
        */

    void end_graphics();
        /*
            Free your terminal
        */

    void print_space(cursor_t position);
        /*
            Prints a "space" in the entered position
        */

    void print_piece(cursor_t position, int player);
        /*
            Prints a "piece" in the entered position
            with the cikir if the respective player
            PLAYER_1 or PLAYER_2*
        */

    void move_cursor(cursor_t *cursor, int y, int x );
        /*
            Move the cursor to the entered position
        */

    void print_board(piece_state board[6][7]);
        /*
            Print the board!
        */

    void print_select(cursor_t position, int player);
        /*
            Prints a blinking piece
        */

    void erase_select(cursor_t position);
        /*
            Erases an 3x5 section (piece size)
        */

    void fall_animation(int pos_x, int player, int distance);
        /**
         * Creates a piece fall animation
         * 
         */

    void win_message(cursor_t position, char *message, int color);
        /**
         * Prints a message
         * 
         */

#endif