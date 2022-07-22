#include "window.h"
#include <cmath>

// ------------------------------ Window ------------------------------ //

Window::Window() : application_window(sf::VideoMode(1920, 1080, 32), "Checkers", sf::Style::Fullscreen), chosen_i(-1), chosen_j(-1), max(0) 
{
    copy_restart();
}

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
                        {
                            choose_pawn();
                        }
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
    if(!(board.check_field(chosen_i, chosen_j)))
    {
        chosen_i = -1;
        chosen_j = -1;
    }
    else
        check_captues_all_pawns(board.get(chosen_i,chosen_j)->get_c());
}

void Window::check_captues_all_pawns(int r)
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(board.check_field(i, j))
                if(board.get(i, j)->get_c() == r)
                    if(board.get(i, j)->get_queen() == 0)
                    {
                        max_capturing(i, j, 0, i, j);
                    }
                    else
                    {
                        max_capturing_queen(i, j, 0, i, j);
                    }
}

void Window::max_capturing_queen(int ch_i, int ch_j, int count, int x, int y)
{
    int aux_x = ch_i;
    int aux_y = ch_j;

    while(aux_x - 2 >= 0 && aux_y - 2 >= 0)
    {
        if(copy_board[aux_x - 1][aux_y - 1] != 2)
        {
            if(copy_board[aux_x - 1][aux_y - 1] != copy_board[ch_i][ch_j] && copy_board[aux_x - 2][aux_y - 2] == 2) //bicie
            {
                    count++; 
                    if(count > max_capture_board[x][y])
                        max_capture_board[x][y] = count;

                    if(count > max)
                        max = count; //ile max bicia

                    int jump_x = aux_x - 2;
                    int jump_y = aux_y - 2;

                    while(jump_x >= 0 && jump_y >= 0 && copy_board[jump_x][jump_y] == 2) //pozwalamy damce zatrzymac sie na dowolnym polu (nie tylko bezpośrednio za zbitym pionkiem)
                    {
                        copy_board[jump_x][jump_y] = copy_board[ch_i][ch_j];
                        copy_board[ch_i][ch_j] = 2; //zbicie

                        int aux_pawns = copy_board[aux_x - 1][aux_y - 1];
                        copy_board[aux_x - 1][aux_y - 1] = 2; //usuniecie zbitego

                        max_capturing_queen(jump_x, jump_y, count, x, y); //reurencyjne przegladanie kolejnych bic

                        copy_board[aux_x - 1][aux_y - 1] = aux_pawns; //powrot do poprzedniego ustawienia pionkow

                        copy_board[ch_i][ch_j] = copy_board[jump_x][jump_y];
                        copy_board[jump_x][jump_y] = 2;

                        jump_x--;
                        jump_y--;
                    }
                    count--;

                    break; // po biciu nie zezwalamy na dalsze poszukiwania bic w danym kierunku (pamietajmy ze przeskoczylismy juz przez jeden pionek
                           // i ustawilismy go na miejsce, nie chcemy, aby doszlo do bicia pionka przesakujac przez dwa lub wiecej)
            }
            else
                break; //oznacza ze natrafilismy na swojego pionka
        } 
        else
        {
            aux_x--;
            aux_y--;
        }
    }

    aux_x = ch_i;
    aux_y = ch_j;

    while(aux_x - 2 >= 0 && aux_y + 2 <= 7)
    {
        if(copy_board[aux_x - 1][aux_y + 1] != 2)
        {
            if(copy_board[aux_x - 1][aux_y + 1] != copy_board[ch_i][ch_j] && copy_board[aux_x - 2][aux_y + 2] == 2) //bicie
            { 
                count++; 
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count; //ile max bicia

                int jump_x = aux_x - 2;
                int jump_y = aux_y + 2;

                while(jump_x >= 0 && jump_y <= 7 && copy_board[jump_x][jump_y] == 2) //pozwalamy damce zatrzymac sie na dowolnym polu (nie tylko bezpośrednio za zbitym pionkiem)
                {
                    copy_board[jump_x][jump_y] = copy_board[ch_i][ch_j];
                    copy_board[ch_i][ch_j] = 2; //zbicie

                    int aux_pawns = copy_board[aux_x - 1][aux_y + 1];
                    copy_board[aux_x - 1][aux_y + 1] = 2; //usuniecie zbitego

                    max_capturing_queen(jump_x, jump_y, count, x, y); //reurencyjne przegladanie kolejnych bic

                    copy_board[aux_x - 1][aux_y + 1] = aux_pawns; //powrot do poprzedniego ustawienia pionkow

                    copy_board[ch_i][ch_j] = copy_board[jump_x][jump_y];
                    copy_board[jump_x][jump_y] = 2;

                    jump_x--;
                    jump_y++;
                }
                count--;

                break;
            }
            else
                break; //oznacza ze natrafilismy na swojego pionka
        } 
        else
        {
            aux_x--;
            aux_y++;
        }
    }


    aux_x = ch_i;
    aux_y = ch_j;

    while(aux_x + 2 <= 7 && aux_y + 2 <= 7)
    {
        if(copy_board[aux_x + 1][aux_y + 1] != 2)
        {
            if(copy_board[aux_x + 1][aux_y + 1] != copy_board[ch_i][ch_j] && copy_board[aux_x + 2][aux_y + 2] == 2) //bicie
            { 
                count++; 
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count; //ile max bicia

                int jump_x = aux_x + 2;
                int jump_y = aux_y + 2;

                while(jump_x <= 7 && jump_y <= 7 && copy_board[jump_x][jump_y] == 2) //pozwalamy damce zatrzymac sie na dowolnym polu (nie tylko bezpośrednio za zbitym pionkiem)
                {
                    copy_board[jump_x][jump_y] = copy_board[ch_i][ch_j];
                    copy_board[ch_i][ch_j] = 2; //zbicie

                    int aux_pawns = copy_board[aux_x + 1][aux_y + 1];
                    copy_board[aux_x + 1][aux_y + 1] = 2; //usuniecie zbitego

                    max_capturing_queen(jump_x, jump_y, count, x, y); //reurencyjne przegladanie kolejnych bic

                    copy_board[aux_x + 1][aux_y + 1] = aux_pawns; //powrot do poprzedniego ustawienia pionkow

                    copy_board[ch_i][ch_j] = copy_board[jump_x][jump_y];
                    copy_board[jump_x][jump_y] = 2;

                    jump_x++;
                    jump_y++;
                }

                count--;
                
                break;
            }
            else
                break; //oznacza ze natrafilismy na swojego pionka
        } 
        else
        {
            aux_x++;
            aux_y++;
        }
    }


    aux_x = ch_i;
    aux_y = ch_j;

    while(aux_x + 2 <= 7 && aux_y - 2 >= 0)
    {
        if(copy_board[aux_x + 1][aux_y - 1] != 2)
        {
            if(copy_board[aux_x + 1][aux_y - 1] != copy_board[ch_i][ch_j] && copy_board[aux_x + 2][aux_y - 2] == 2) //bicie
            { 
                count++; 
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count; //ile max bicia

                int jump_x = aux_x + 2;
                int jump_y = aux_y - 2;

                while(jump_x <= 7 && jump_y >= 0 && copy_board[jump_x][jump_y] == 2) //pozwalamy damce zatrzymac sie na dowolnym polu (nie tylko bezpośrednio za zbitym pionkiem)
                {
                    copy_board[jump_x][jump_y] = copy_board[ch_i][ch_j];
                    copy_board[ch_i][ch_j] = 2; //zbicie

                    int aux_pawns = copy_board[aux_x + 1][aux_y - 1];
                    copy_board[aux_x + 1][aux_y - 1] = 2; //usuniecie zbitego

                    max_capturing_queen(jump_x, jump_y, count, x, y); //reurencyjne przegladanie kolejnych bic

                    copy_board[aux_x + 1][aux_y - 1] = aux_pawns; //powrot do poprzedniego ustawienia pionkow

                    copy_board[ch_i][ch_j] = copy_board[jump_x][jump_y];
                    copy_board[jump_x][jump_y] = 2;

                    jump_x++;
                    jump_y--;
                }
                count--;

                break;
            }
            else
                break; //oznacza ze natrafilismy na swojego pionka
        } 
        else
        {
            aux_x++;
            aux_y--;
        }
    }
}

void Window::max_capturing(int ch_i, int ch_j, int count, int x, int y)
{   
    if(ch_i - 2 >= 0 && ch_j - 2 >= 0)
        if(copy_board[ch_i - 2][ch_j - 2] == 2 && copy_board[ch_i - 1][ch_j - 1] != 2)
            if(copy_board[ch_i - 1][ch_j - 1] != copy_board[ch_i][ch_j])
            {
                count++;    //zliczanie glebokosci
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count;

                copy_board[ch_i - 2][ch_j - 2] = copy_board[ch_i][ch_j]; //zamiana
                int aux = copy_board[ch_i - 1][ch_j - 1];
                copy_board[ch_i - 1][ch_j - 1] = 2; //usuwanie zbitego pionka
                copy_board[ch_i][ch_j] = 2; //zamiana

                max_capturing(ch_i - 2, ch_j - 2, count, x, y);

                copy_board[ch_i][ch_j] = copy_board[ch_i - 2][ch_j - 2]; //ustawienie zbitego pionka na miesjce
                copy_board[ch_i - 1][ch_j - 1] = aux; 
                copy_board[ch_i - 2][ch_j - 2] = 2; 

                count--; //zerowanie glebokosci po powrocie
            }

    if(ch_i - 2 >= 0 && ch_j + 2 <= 7)
        if(copy_board[ch_i - 2][ch_j + 2] == 2 && copy_board[ch_i - 1][ch_j + 1] != 2)
            if(copy_board[ch_i - 1][ch_j + 1] != copy_board[ch_i][ch_j])
            {
                count++;    //zliczanie glebokosci
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count;

                copy_board[ch_i - 2][ch_j + 2] = copy_board[ch_i][ch_j]; //zamiana
                int aux = copy_board[ch_i - 1][ch_j + 1];
                copy_board[ch_i - 1][ch_j + 1] = 2; //usuwanie zbitego pionka
                copy_board[ch_i][ch_j] = 2; //zamiana

                max_capturing(ch_i - 2, ch_j + 2, count, x, y);

                copy_board[ch_i][ch_j] = copy_board[ch_i - 2][ch_j + 2]; //ustawienie zbitego pionka na miesjce
                copy_board[ch_i - 1][ch_j + 1] = aux; 
                copy_board[ch_i - 2][ch_j + 2] = 2; 

                count--; //zerowanie glebokosci po powrocie
            }

    if(ch_i + 2 <= 7 && ch_j + 2 <= 7)
        if(copy_board[ch_i + 2][ch_j + 2] == 2 && copy_board[ch_i + 1][ch_j + 1] != 2)
            if(copy_board[ch_i + 1][ch_j + 1] != copy_board[ch_i][ch_j])
            {
                count++;    //zliczanie glebokosci
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count;

                copy_board[ch_i + 2][ch_j + 2] = copy_board[ch_i][ch_j]; //zamiana
                int aux = copy_board[ch_i + 1][ch_j + 1];
                copy_board[ch_i + 1][ch_j + 1] = 2; //usuwanie zbitego pionka
                copy_board[ch_i][ch_j] = 2; //zamiana

                max_capturing(ch_i + 2, ch_j + 2, count, x, y);

                copy_board[ch_i][ch_j] = copy_board[ch_i + 2][ch_j + 2]; //ustawienie zbitego pionka na miesjce
                copy_board[ch_i + 1][ch_j + 1] = aux; 
                copy_board[ch_i + 2][ch_j + 2] = 2; 

                count--; //zerowanie glebokosci po powrocie
            }

    if(ch_i + 2 <= 7 && ch_j - 2 >= 0)
        if(copy_board[ch_i + 2][ch_j - 2] == 2 && copy_board[ch_i + 1][ch_j - 1] != 2)
            if(copy_board[ch_i + 1][ch_j - 1] != copy_board[ch_i][ch_j])
            {
                count++;    //zliczanie glebokosci
                if(count > max_capture_board[x][y])
                    max_capture_board[x][y] = count;

                if(count > max)
                    max = count;

                copy_board[ch_i + 2][ch_j - 2] = copy_board[ch_i][ch_j]; //zamiana
                int aux = copy_board[ch_i + 1][ch_j - 1];
                copy_board[ch_i + 1][ch_j - 1] = 2; //usuwanie zbitego pionka
                copy_board[ch_i][ch_j] = 2; //zamiana

                max_capturing(ch_i + 2, ch_j - 2, count, x, y);

                copy_board[ch_i][ch_j] = copy_board[ch_i + 2][ch_j - 2]; //ustawienie zbitego pionka na miesjce
                copy_board[ch_i + 1][ch_j - 1] = aux; 
                copy_board[ch_i + 2][ch_j - 2] = 2; 

                count--; //zerowanie glebokosci po powrocie
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
    
    if(!(change_pawn(move_x, move_y)) && legal(move_x, move_y))
        {
            board.swap_fields(chosen_i, chosen_j, move_x, move_y);

            chosen_i = -1;
            chosen_j = -1;

            if(upgrade_to_queen(move_x, move_y))
            {
                board.upgrade(move_x, move_y);
            }
            copy_restart();  
        } 
}

int Window::change_pawn(int move_x, int move_y)
{
    if(board.get(move_x, move_y) != nullptr)
    {            
        chosen_i = move_x;
        chosen_j = move_y;
        return 1;
    }
    else
        return 0;
}

int Window::upgrade_to_queen(int move_x, int move_y)
{
    if(board.get(move_x, move_y)->get_c() == 1 && move_y == 0)
            return 1;

    if(board.get(move_x, move_y)->get_c() == 0 && move_y == 7)
            return 1;

    return 0;
}

int Window::legal(int move_x, int move_y)
{
    if((move_x + move_y) % 2)
    {
        if(board.get(chosen_i, chosen_j)->get_queen() == 0)
        {
            if(board.get(chosen_i, chosen_j)->get_c() == 1)
            {
                if(choose_max() && legal_white(move_x, move_y))            
                    return 1;
            }
            else
            {
                if(choose_max() && legal_black(move_x, move_y))
                    return 1;
            }
        }
        else
        {
            if(choose_max() && legal_queen(move_x, move_y))
                return 1;
        }
    }
    return 0;
}

int Window::choose_max()
{
    if(max_capture_board[chosen_i][chosen_j] == max)
        return 1;
    return 0;
}

int Window::legal_white(int move_x, int move_y)
{
    if(abs(move_x - chosen_i) == 1 && move_y - chosen_j == -1 && max == 0)
        return 1;
    else
        if(capturing_a_pawn(move_x, move_y))
            return 1;
        
    return 0;
}

int Window::legal_black(int move_x, int move_y)
{
    if(abs(move_x - chosen_i) == 1 && move_y - chosen_j == 1 && max == 0)
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
                if(is_it_max(move_x, move_y))
                {
                    delete_pawn((chosen_i + move_x) / 2, (chosen_j + move_y) / 2);
                    return 1;
                }
    
    return 0;
}

int Window::is_it_max(int move_x, int move_y)
{
    copy_board[move_x][move_y] = copy_board[chosen_i][chosen_j];
    copy_board[chosen_i][chosen_j] = 2; // symualacja bicia

    int x = (chosen_i + move_x) / 2;
    int y = (chosen_j + move_y) / 2;
    int aux = copy_board[x][y];
    copy_board[x][y] = 2;

    int aux_max = max;
    max = 0;
    max_capturing(move_x, move_y, 0, move_x, move_y);

    if(max == aux_max - 1)
    {
        return 1;
    }
    else
    {
        copy_board[chosen_i][chosen_j] = copy_board[move_x][move_y];
        copy_board[move_x][move_y] = 2;
        copy_board[x][y] = aux; //wstawienie pionka na miejscea
        
        max = aux_max;
        return 0;
    }
}

int Window::is_it_max_queen(int move_x, int move_y) 
{
    copy_board[move_x][move_y] = copy_board[chosen_i][chosen_j];
    copy_board[chosen_i][chosen_j] = 2; // symualacja bicia

    int x, y;

    if(move_x > chosen_i)
        x = chosen_i + 1;
    else
        x = chosen_i - 1;

    if(move_y > chosen_j)
        y = chosen_j + 1;
    else
        y = chosen_j - 1;

    while(copy_board[x][y] == 2)
    {
        if(move_x > chosen_i)
            x++;
        else
            x--;

        if(move_y > chosen_j)
            y++;
        else
            y--;
    }

    int aux = copy_board[x][y];
    copy_board[x][y] = 2;

    int aux_max = max;
    max = 0;
    max_capturing_queen(move_x, move_y, 0, move_x, move_y);

    if(max == aux_max - 1)
    {
        return 1;
    }
    else
    {
        copy_board[chosen_i][chosen_j] = copy_board[move_x][move_y];
        copy_board[move_x][move_y] = 2;
        copy_board[x][y] = aux; //wstawienie pionka na miejscea
        
        max = aux_max;
        return 0;
    }
}

int Window::legal_queen(int move_x, int move_y)
{
    if(queen_legal_move(move_x, move_y) && max == 0)
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
    
    if(how_many_captures == 1 && is_it_max_queen(move_x, move_y))
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

void Window::copy_restart()
{
    for(int j = 0; j < 8 ; j++)
        for(int i = 0; i < 8; i++)
            {
                if(board.check_field(i,j))
                    copy_board[i][j] = board.get(i, j)->get_c();
                else
                    copy_board[i][j] = 2;

                max_capture_board[i][j] = 0;
            }
    max = 0;
}
