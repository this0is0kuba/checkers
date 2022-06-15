#include "board.h"

// ------------------------------ Board ------------------------------ //

Board::Board()
{
    
    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            if((i + j) % 2 == 0)
                items[i][j] = nullptr;
            else
            {
                items[i][j] = new BlackPawns;
            }
        }
    }

    for(int j = 3; j < 5; j++)
    {
        for(int i = 0; i < 8; i++)
            items[i][j] = nullptr;
    }
    
    for(int j = 5; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            if((i + j) % 2 == 0)
                items[i][j] = nullptr;
            else
            {
                items[i][j] = new WhitePawns;
            }
        }
    }
}

Board::~Board()
{
    for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            if(items[i][j] != nullptr)
                delete items[i][j];
        }   
    }
        
}

void Board::set_lengths(float r, float dx, float dy, float len, float dr)
{
    radius = r;
    delta_x = dx;
    delta_y = dy;
    length = len;
    delta_r = dr;
}

void Board::set_pawns()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(items[i][j] != nullptr)
                items[i][j]->set(i, j, radius, delta_x, delta_y, length, delta_r);
}

bool Board::check_field(int i, int j)
{
    if(items[i][j] != nullptr)
        return 1;
    return 0;
}

Pawns* Board::get(int i, int j)
{
    return items[i][j];
}

float Board::get_radius() {return radius;}
float Board::get_delta_x() {return delta_x;}
float Board::get_delta_y() {return delta_y;}
float Board::get_length() {return length;}
float Board::get_delta_r() {return delta_r;}

void Board::swap_fields(int old_i, int old_j, int new_i, int new_j)
{
    items[new_i][new_j] = items[old_i][old_j];
    items[old_i][old_j] = nullptr; 

    items[new_i][new_j]->set_position(new_i, new_j, delta_x, delta_y, length, delta_r);
}

void Board::upgrade(int position_x, int position_y)
{
    items[position_x][position_y]->upgrade_to_queen();
}

void Board::delete_item(int position_x, int position_y)
{
    delete items[position_x][position_y];
    items[position_x][position_y] = nullptr;
}