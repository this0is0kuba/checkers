#include "board.h"

class Window
{
    int copy_board[8][8]; // 0 - czarny, 1 - bialy, 2 - puste pole

    int max; //ile jest aktualnie najwiecej mozliwych bic
    int max_capture_board[8][8]; //ile bic ma jaki pionek (pionki ktore aktualnie nie maja tury maja ustawione 0)
    int chosen_i; //aktualnie wybrane pole
    int chosen_j;

    Board board; //plansza
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

    void check_captues_all_pawns(int);
    void max_capturing_queen(int, int, int, int, int);
    void max_capturing(int, int, int, int, int);

    void move();

    int change_pawn(int, int);
    int upgrade_to_queen(int, int);

    int legal(int, int);
    int legal_white(int, int);
    int legal_black(int, int);
    int capturing_a_pawn(int, int);
    int choose_max();
    int is_it_max(int, int);

    int is_it_max_queen(int, int);
    int legal_queen(int, int);
    int queen_legal_move(int, int);
    int capturing_a_queen(int, int);

    void delete_pawn(int, int);
    void selected_field(int, int);

    void copy_restart();
};
