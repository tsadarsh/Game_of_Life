#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

int WINDOW_WIDTH = 1036;
int WINDOW_HEIGHT = 569;
int CELL_SIZE = 10;
std::default_random_engine rng;
std::uniform_int_distribution<int> col_r(0, 255);
std::uniform_int_distribution<int> col_g(0, 255);
std::uniform_int_distribution<int> col_b(0, 255);
std::uniform_int_distribution<int> col_a(0, 255);
std::uniform_int_distribution<int> bw(0, 1);

int main(int argc, char* argv[])
{
    for (int i=0; i< argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-n")
        {
            // set threads
        }
        else if (arg == "-c")
        {
            if (i+1 < argc)
                CELL_SIZE = std::stoi(argv[i+1]);
        }
        else if (arg == "-x")
        {
            if (i+1 < argc)
                WINDOW_WIDTH = std::stoi(argv[i+1]);
        }
        else if (arg == "-y")
        {
            if (i+1 < argc)
                WINDOW_HEIGHT = std::stoi(argv[i+1]);
        }
        else if (arg == "-t")
        {
            // process
        }
    }
    // create the window
    sf::Texture texture;
    if (!texture.loadFromFile("white.png", sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE)))
    {
        std::cout << "No texture found!";
    }
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    int columns = (WINDOW_WIDTH-CELL_SIZE) / CELL_SIZE;
    int rows = (WINDOW_HEIGHT-CELL_SIZE) / CELL_SIZE;
    std::vector<std::vector<sf::Sprite>> cells;
    for (int iRow=0; iRow < rows; iRow++)
    {
        std::vector<sf::Sprite> cells_row;
        for (int iCol=0; iCol < columns; iCol++)
        {
            sf::Sprite cell(texture);
            cell.setPosition(iCol*CELL_SIZE, iRow*CELL_SIZE);
            // cell.setColor(sf::Color(col_r(rng), col_g(rng), col_b(rng), col_a(rng)));
            cells_row.push_back(cell);
        }
        cells.push_back(cells_row);
    }

    std::vector<std::vector<int>> cgl_grid;
    // +2 for row and cols to have a boundry of 0s
    for (int i_row=0; i_row < rows+2; i_row++)
    {
        std::vector<int> cgl_grid_row;
        for (int i_col=0; i_col < columns+2; i_col++)
        {
            int val = bw(rng);
            // std::cout << val;
            cgl_grid_row.push_back(val);
        }
        cgl_grid.push_back(cgl_grid_row);
        // std::cout << std::endl;
    }
    for (int i = 0; i < columns+2; i++) 
    {
        cgl_grid[0][i] = 0;
        cgl_grid[rows+1][i] = 0;
    }
    for (int i = 0; i < rows+2; i++)
    {
        cgl_grid[i][0] = 0;
        cgl_grid[i][columns+1] = 0;
    }

    // std::cout << "Grid rows: " << cgl_grid.size();
    // std::cout << "Grid cols: " << cgl_grid[0].size();
    sf::Clock Clock;
    Clock.restart();


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int Time = Clock.getElapsedTime().asMilliseconds();
	    if (Time >= 300) {
            Clock.restart();

            // draw everything here...
            
            // for (int iCells = 0; iCells < cells.size(); iCells++)
            // {
            //     window.draw(cells[iCells]);
            // }
            for (int i_row=1; i_row < cgl_grid.size()-1; i_row++)
            {
                for (int i_col=1; i_col < cgl_grid[i_row].size()-1; i_col++)
                {
                    int nearby_score = cgl_grid[i_row - 1][i_col - 1] + cgl_grid[i_row - 1][i_col] + cgl_grid[i_row - 1][i_col + 1] + cgl_grid[i_row][i_col - 1] \
                            + cgl_grid[i_row][i_col + 1] + cgl_grid[i_row + 1][i_col - 1] + cgl_grid[i_row + 1][i_col + 1];

                    // dead cell: check if birth possible
                    if (cgl_grid[i_row][i_col] == 0)
                    {
                        if( nearby_score == 3)
                            cgl_grid[i_row][i_col] = 1;
                    }

                    // alive cell: check death by isolation, death by overcrowding, or survival
                    else 
                    {
                        // death by isolation
                        if (nearby_score <= 1)
                            cgl_grid[i_row][i_col] = 0;
                        
                        // death by overpopulation
                        else if ( nearby_score >= 4)
                            cgl_grid[i_row][i_col] = 0;
                        
                        else{
                            // cell survives, so do nothing
                        }
                    }

                    // if(cgl_grid[i_row][i_col])
                    // {
                    //     window.draw(cells[i_row-1][i_col-1]);
                    //     // std::cout << cgl_grid[i_row][i_col];
                    // }
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        for (int i_row = 1; i_row < cgl_grid.size()-1; i_row++)
        {
            for (int i_col = 1; i_col < cgl_grid[i_row].size()-1; i_col++)
            {
                if (cgl_grid[i_row][i_col])
                    window.draw(cells[i_row-1][i_col-1]);
            }
        }
        // window.draw(sprite);

        // end the current frame
        window.display();
    }

    return 0;
}