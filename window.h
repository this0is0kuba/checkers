#include "board.h"

class Window
{
    int chosen_i;
    int chosen_j;
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

    int range();
    void choose_pawn();
    int forced_capture();
    void move();
    int change_pawn(int, int);

    int legal(int, int);
    int legal_white(int, int);
    int legal_black(int, int);
    int capturing_a_pawn(int, int);
    int upgrade_to_queen(int, int);

    int legal_queen(int, int);
    int queen_legal_move(int, int);
    int capturing_a_queen(int, int);

    void delete_pawn(int, int);
    void selected_field(int, int);
};
