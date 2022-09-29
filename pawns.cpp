#include "pawns.h"

// ------------------------------ Pawns ------------------------------ //

Pawns::Pawns() : position_x(0), position_y(0), circle(1), queen(0) {};

void Pawns::set(int x, int y, float r, float delta_x, float delta_y, float length, float delta_r)
{
    position_x = x;
    position_y = y;
    circle.setRadius(r);
    circle.setPosition(delta_x + delta_r + position_x * length, delta_y + delta_r + position_y * length);
}

void Pawns::set_position(int new_i, int new_j, float delta_x, float delta_y, float length, float delta_r)
{
    position_x = new_i;
    position_y = new_j;
    circle.setPosition(delta_x + delta_r + position_x * length, delta_y + delta_r + position_y * length);
}

sf::CircleShape Pawns::get_circle() {return circle;}

float Pawns::get_circle_x() {return circle.getPosition().x;}

float Pawns::get_circle_y() {return circle.getPosition().y;}

bool Pawns::get_queen() {return queen;}

// ------------------------------ WhitePawns ------------------------------ //

const sf::Color WhitePawns::color = sf::Color::White;

WhitePawns::WhitePawns() : Pawns() 
{
    circle.setFillColor(color);
}

bool WhitePawns::get_c()
{
    return c;
}

void WhitePawns::upgrade_to_queen()
{
    if(queen == 0)
    {
        circle.setFillColor(sf::Color::White);
        queen_texture.loadFromFile("./textures/crownWhite.png");
        circle.setTexture(&queen_texture);
    }
    
    queen = 1;
}

// ------------------------------ BlackPawns ------------------------------ //

const sf::Color BlackPawns::color = sf::Color::Black;

BlackPawns::BlackPawns() : Pawns()
{
    circle.setFillColor(color);
}

bool BlackPawns::get_c()
{
    return c;
}

void BlackPawns::upgrade_to_queen()
{
    if(queen == 0)
    {
        circle.setFillColor(sf::Color::White);
        queen_texture.loadFromFile("./textures/crownBlack.png");
        circle.setTexture(&queen_texture);
    }
    
    queen = 1;
}