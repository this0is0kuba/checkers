#include <SFML/Graphics.hpp>

class Pawns
{
protected:
    int position_x;
    int position_y;
    sf::CircleShape circle;
    
    int queen;
    sf::Texture queen_texture;

public:
    Pawns();
    virtual ~Pawns() {};

    void set(int, int, float, float, float, float, float);
    void set_position(int, int, float, float, float, float);

    sf::CircleShape get_circle();
    float get_circle_x();
    float get_circle_y();
    virtual bool get_c() = 0;
    bool get_queen();

    virtual void upgrade_to_queen() = 0;
};

class WhitePawns : public Pawns
{
    const static sf::Color color;
    const bool c = 1;

public:
    WhitePawns();

    bool get_c();
    void upgrade_to_queen();
};

class BlackPawns : public Pawns
{
    const static sf::Color color;
    const bool c = 0;
public:
    BlackPawns();

    bool get_c();
    void upgrade_to_queen();
};