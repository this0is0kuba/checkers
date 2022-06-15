#include "window.h"
#include <cmath>

// ------------------------------ Window ------------------------------ //

Window::Window() : application_window(sf::VideoMode(1920, 1080, 32), "Checkers", sf::Style::Fullscreen), chosen_i(-1), chosen_j(-1) {}

void Window::display_board()
{
    beggining_parameters();
    window_rendering();
}

void Window::window_rendering()
{
    while(application_window.isOpen())
    {
        texture();
        circles_display();
        events();

        application_window.display();
    }
}

void Window::beggining_parameters()
{
    sf::Vector2u vector = application_window.getSize();

    float length = std::min(vector.x, vector.y) / 8.0;
    float delta_x = (vector.x - length * 8) * 0.5;
    float delta_y = (vector.y - length * 8) * 0.5;

    float radius = length * 0.4;
    float delta_r = (length - 2 * radius) / 2;
    
    board.set_lengths(radius, delta_x, delta_y, length, delta_r);
    board.set_pawns();
}

void Window::events()
{
    sf::Event event;

    while(application_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                application_window.close();
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                application_window.close();
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    if(range())
                    {
                        if(chosen_i == -1 && chosen_j == -1)
                            choose_pawn();
                        else
                            move();
                    }
                }
        }
}

void Window::texture()
{
    application_window.clear(sf::Color::Black);

    sf::RectangleShape fields[8][8];
    sf::Vector2u vector = application_window.getSize();

    float length = std::min(vector.x, vector.y) / 8.0;
    float delta_x = (vector.x - length * 8) * 0.5;
    float delta_y = (vector.y - length * 8) * 0.5;

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            fields[i][j].setFillColor((i + j) % 2 ? sf::Color(150, 75, 20) : sf::Color(200, 190, 180));
            fields[i][j].setSize(sf::Vector2f(length, length));
            fields[i][j].setPosition(i * length + delta_x, j * length + delta_y);

            application_window.draw(fields[i][j]);
        }
    }

    if(chosen_i != -1 && chosen_j != -1)
        selected_field(chosen_i, chosen_j);
}

void Window::circles_display()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(board.check_field(i, j))
                application_window.draw(board.get(i, j)->get_circle());
}

int Window::range()
{
    sf::Vector2i mouse_vector = sf::Mouse::getPosition();

    int len_x = mouse_vector.x;
    int len_y = mouse_vector.y;

    if(len_x <= board.get_delta_x() || len_x >= 8 * board.get_length() + board.get_delta_x() || 
       len_y <= board.get_delta_y() || len_y >= 8 * board.get_length() + board.get_delta_y())
       {
           return 0;
       }
    return 1;
}

void Window::choose_pawn()
{
    sf::Vector2i mouse_vector = sf::Mouse::getPosition();

    int len_x = mouse_vector.x;
    int len_y = mouse_vector.y;

    len_x -= board.get_delta_x();
    len_y -= board.get_delta_y();

    for(int i = 1; i <= 8; i++)
    {
        if(len_x <= board.get_length() * i)
        {
            chosen_i = i - 1;
            break;
        }
    }

    for(int i = 1; i <= 8; i++)
    {
        if(len_y <= board.get_length() * i)
        {
            chosen_j = i - 1;
            break;
        }
    }
}

void Window::move()
{
    sf::Vector2i mouse_vector = sf::Mouse::getPosition();

    int move_x = -1;
    int move_y = -1;

    int len_x = mouse_vector.x;
    int len_y = mouse_vector.y;

    len_x -= board.get_delta_x();
    len_y -= board.get_delta_y();

    for(int i = 1; i <= 8; i++)
    {
        if(len_x <= board.get_length() * i)
        {
            move_x = i - 1;
            break;
        }
    }

    for(int i = 1; i <= 8; i++)
    {
        if(len_y <= board.get_length() * i)
        {
            move_y = i - 1;
            break;
        }
    }

    if(change_pawn(move_x, move_y) && legal(move_x, move_y))
        {
            board.swap_fields(chosen_i, chosen_j, move_x, move_y);

            chosen_i = -1;
            chosen_j = -1;

            if(upgrade_to_queen(move_x, move_y))
            {
                board.upgrade(move_x, move_y);
            }  
        } 
}

int Window::upgrade_to_queen(int move_x, int move_y)
{
    if(board.get(move_x, move_y)->get_c() == 1 && move_y == 0)
            return 1;

    if(board.get(move_x, move_y)->get_c() == 0 && move_y == 7)
            return 1;

    return 0;
}

int Window::change_pawn(int move_x, int move_y)
{
    if(board.get(move_x, move_y) != nullptr)
    {
        chosen_i = move_x;
        chosen_j = move_y;
        return 0;
    }
    else
        return 1;
}

int Window::legal(int move_x, int move_y)
{
    if((move_x + move_y) % 2)
    {
        if(board.get(chosen_i, chosen_j)->get_queen() == 0)
        {
            if(board.get(chosen_i, chosen_j)->get_c() == 1)
            {
                if(legal_white(move_x, move_y))
                    return 1;
            }
            else
            {
                if(legal_black(move_x, move_y))
                    return 1;
            }
        }
        else
        {
            if(legal_queen(move_x, move_y))
                return 1;
        }
    }
    return 0;
}

int Window::legal_white(int move_x, int move_y)
{
    if(abs(move_x - chosen_i) == 1 && move_y - chosen_j == -1)
        return 1;
    else
        if(capturing_a_pawn(move_x, move_y))
            return 1;
        
    return 0;
}

int Window::legal_black(int move_x, int move_y)
{
    if(abs(move_x - chosen_i) == 1 && move_y - chosen_j == 1)
        return 1;
    else
        if(capturing_a_pawn(move_x, move_y))
            return 1;
    
    return 0;
}

int Window::capturing_a_pawn(int move_x, int move_y)
{
    if(abs(move_x - chosen_i) == 2 && abs(move_y - chosen_j) == 2)
        if(board.get((chosen_i + move_x) / 2, (chosen_j + move_y) / 2) != nullptr)
            if(board.get(chosen_i, chosen_j)->get_c() != board.get((chosen_i + move_x) / 2, (chosen_j + move_y) / 2)->get_c())
            {
                delete_pawn((chosen_i + move_x) / 2, (chosen_j + move_y) / 2);
                return 1;
            }
    
    return 0;
}

int Window::legal_queen(int move_x, int move_y)
{
    if(queen_legal_move(move_x, move_y))
        return 1;
    else
        if(capturing_a_queen(move_x, move_y))
            return 1;
    
    return 0;
}

int Window::capturing_a_queen(int move_x, int move_y)
{
    int i = chosen_i;
    int j = chosen_j;

    int aux_x = -1;
    int aux_y = -1;
    int how_many_captures = 0;
    if(abs(move_x - chosen_i) == abs(move_y - chosen_j))
    {
        while(i != move_x)
        {
            if(move_x - chosen_i > 0)
                i++;
            else
                i--;
            
            if(move_y - chosen_j > 0)
                j++;
            else 
                j--;

            if(board.check_field(i, j))
            {   if(board.get(chosen_i, chosen_j)->get_c() == board.get(i, j)->get_c())
                    return 0;
                else
                {
                    how_many_captures++;
                    aux_x = i;
                    aux_y = j;
                }
                
            }
                
        }
    }
    else
        return 0;
    
    if(how_many_captures == 1)
    {
        delete_pawn(aux_x, aux_y);
        return 1;
    }
        
    return 0;
}

int Window::queen_legal_move(int move_x, int move_y)
{
    int i = chosen_i;
    int j = chosen_j;

    if(abs(move_x - chosen_i) == abs(move_y - chosen_j))
    {
        while(i != move_x)
        {
            if(move_x - chosen_i > 0)
                i++;
            else
                i--;
            
            if(move_y - chosen_j > 0)
                j++;
            else 
                j--;

            if(board.check_field(i, j))
                return 0;
        }
    }
    else
        return 0;
    
    return 1;
}

void Window::delete_pawn(int position_x, int position_y)
{
    board.delete_item(position_x, position_y);
}

void Window::selected_field(int i, int j)
{
    sf::RectangleShape field;

        field.setFillColor(sf::Color::Red);
        field.setSize(sf::Vector2f(board.get_length(), board.get_length()));
        field.setPosition(i * board.get_length() + board.get_delta_x(), j * board.get_length() + board.get_delta_y());

        application_window.draw(field);
}

