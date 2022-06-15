#include "pawns.h"

class Board
{
    Pawns* items[8][8];

    float radius;
    float delta_x;
    float delta_y;
    float length;
    float delta_r;

public:
    Board();
    ~Board();

    void set_lengths(float, float, float, float, float);
    void set_pawns();

    bool check_field(int, int);

    Pawns* get(int, int);
    float get_radius();
    float get_delta_x();
    float get_delta_y();
    float get_length();
    float get_delta_r();  

    void swap_fields(int, int, int, int);
    void upgrade(int, int);

    void delete_item(int, int);
};

