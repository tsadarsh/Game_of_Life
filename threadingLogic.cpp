#include <thread>

void populateNextGen (std::vector<std::vector<int>>* curr_gen, std::vector<std::vector<int>>* next_gen, int i_row, int i_col)
{
    int nearby_score = (*curr_gen)[i_row - 1][i_col - 1] + (*curr_gen)[i_row - 1][i_col] + (*curr_gen)[i_row - 1][i_col + 1] + (*curr_gen)[i_row][i_col - 1] \
                    + (*curr_gen)[i_row][i_col + 1] + (*curr_gen)[i_row + 1][i_col - 1] + (*curr_gen)[i_row + 1][i_col + 1];

    // dead cell: check if birth possible
    if ((*curr_gen)[i_row][i_col] == 0)
    {
        if( nearby_score == 3)
            (*next_gen)[i_row][i_col] = 1;
    }

    // alive cell: check death by isolation, death by overcrowding, or survival
    else 
    {
        // death by isolation
        if (nearby_score <= 1)
            (*next_gen)[i_row][i_col] = 0;
        
        // death by overpopulation
        else if ( nearby_score >= 4)
            (*next_gen)[i_row][i_col] = 0;
        
        else{
            // cell survives, so do nothing
            (*next_gen)[i_row][i_col] = 1;
        }
    }

    (*curr_gen)[i_row][i_col] = (*next_gen)[i_row][i_col];
}

void THRD_Process(std::vector<std::vector<int>>* curr_gen, std::vector<std::vector<int>>* next_gen)
{
    std::cout << "Using threading logic" << std::endl;
    //int total_threads = (*curr_gen).size() * (*curr_gen)[0].size(); 
    std::vector<std::thread> ps;

    int thread_count = 0;
    for (int i_row=1; i_row < (*curr_gen).size()-1; i_row++)
    {
        for (int i_col=1; i_col < (*curr_gen)[i_row].size()-1; i_col++)
        {
            ps.push_back(std::thread(populateNextGen, std::ref(curr_gen), std::ref(next_gen), i_row, i_col));
            thread_count++;
        }
    }
    for (int i=0; i <= ps.size(); i++)
    {
        if(ps[i].joinable())
            ps[i].join();
    }
}