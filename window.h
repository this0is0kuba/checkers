#include "board.h"

class Window
{
    int copy_board[8][8]; // 0 - czarny, 1 - bialy, 2 - puste pole

    int max; //ile jest aktualnie najwiecej mozliwych bic
    int max_capture_board[8][8]; //ile bic ma jaki pionek (pionki ktore aktualnie nie maja tury maja ustawione 0)
    int chosen_i; //aktualnie wybrane pole
    int chosen_j;

    int round; // 1 - runda bialego, 0 - runda czarnego
    int block; // 1 - jakis pionek musi dokonczyc swoje bicia, 0 - mozna kontynuowac dowolnym pionkiem
    int block_pion[8][8]; // nie zezwa na to abysmy ruszyli sie innym pionkiem dwa razy niz poczatkowo wyralismy

    int white; //ilosc bialych pionkow
    int black; //ilosc czarnych pionkow
    int end_game; // 0 - gra toczy sie dalej, 1 - wygral gracz bialy, 2 - wygral gracz czarny, 3 - remis(nastepuje gdy gracze wykonaja po 15 ruchow damkami bez zmniejszania liczby pionow(damek lub pionkow))
    int moves_with_no_capturing; 

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

    void check_captures_all_pawns(int);
    void max_capturing_queen(int, int, int, int, int);
    void max_capturing(int, int, int, int, int);

    void move();

    int change_pawn(int, int); //"proba zmiany pionka" zwraca true gdy mamy wybrany pionek i klikamy na inny, dodatkowo zmienia na inny pionek jesli jest tego samego co poprzednio wybrany
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
    void round_change();

    void end();
    void check_the_end_of_the_game(int);
    int possible_moves(int);
};
