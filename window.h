#include "board.h"

class Window
{
    int copy_board[8][8]; // a board showing the current situation in the game (0 - black pawns, 1 - white pawns, 2 - empty fields)

    int max; // how many captures is now possible 
    int max_capture_board[8][8]; //how many captures every of pawns have (opponent's pwans have 0)

    int chosen_i; // the coordinates of the currently selected pawn
    int chosen_j;

    bool round; // 1 - round of white, 0 - round of black
    bool block; // 1 - some pawn have to finish your move, 0 - any pawns can make a move
    int block_pion[8][8]; // if "block = 1" block_pion board shows which pion is blocked

    int white; // how many white pawns there are currently
    int black; // how many black pawns there are currently
    int end_game; // 0 - the game is continues, 1 - player white wins, 2 - player black wins
                  // 3 - draw (draw is when both players have made 15 moves of queens each and there was no capturing durning this time)

    int moves_with_no_capturing; 

    Board board;
    sf::RenderWindow application_window;

public:
    Window();

    void display_board();
    void window_rendering();

    void beggining_parameters();
    void events();
    void texture();

    void circles_display();

    bool range();
    void choose_pawn();

    void check_captures_all_pawns(int);
    void max_capturing_queen(int, int, int, int, int);
    void max_capturing(int, int, int, int, int);

    void move();

    bool change_pawn(int, int);
    bool upgrade_to_queen(int, int);

    bool legal(int, int);
    bool legal_white(int, int);
    bool legal_black(int, int);
    bool capturing_a_pawn(int, int);
    bool choose_max();
    bool is_it_max(int, int);

    bool is_it_max_queen(int, int);
    bool legal_queen(int, int);
    bool queen_legal_move(int, int);
    bool capturing_a_queen(int, int);

    void delete_pawn(int, int);
    void selected_field(int, int);

    void copy_restart();
    void round_change();

    void end();
    void check_the_end_of_the_game(bool);
    bool possible_moves(bool);
};
