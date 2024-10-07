#include <thread>
#include <vector>
#include <iostream>

void populateNextGen(std::vector<std::vector<int>>* curr_gen, std::vector<std::vector<int>>* next_gen, std::vector<std::vector<int>> quota)
{
    for (int iCell = 0; iCell < quota.size(); iCell++)
    {
        int i_row = quota[iCell][0];
        int i_col = quota[iCell][1];

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
    }
}

void updateCurrGen(std::vector<std::vector<int>>* curr_gen, std::vector<std::vector<int>>* next_gen, std::vector<std::vector<int>> quota)
{
    for (int iCell = 0; iCell < quota.size(); iCell++)
    {
        int i_row = quota[iCell][0];
        int i_col = quota[iCell][1];

        (*curr_gen)[i_row][i_col] = (*next_gen)[i_row][i_col];
    }
}

void THRD_Process(std::vector<std::vector<int>>* curr_gen, std::vector<std::vector<int>>* next_gen, int num_of_threads, std::vector<std::vector<std::vector<int>>>* quotas)
{
    std::vector<std::thread> thread_pool;

    for (int iThread = 0; iThread < num_of_threads; iThread++)
    {
        // std::vector<std::vector<int>>> q = &((*quotas)[iThread]);
        thread_pool.push_back(std::thread(populateNextGen, std::ref(curr_gen), std::ref(next_gen), (*quotas)[iThread]));
    }
    std::cout << "Finsihsed creating threads" << std::endl;
    for (int iThread = 0; iThread < thread_pool.size(); iThread++)
    {
        if (thread_pool[iThread].joinable())
            thread_pool[iThread].join();
    }

    thread_pool.resize(0);
    for (int iThread = 0; iThread < num_of_threads; iThread++)
    {
        thread_pool.push_back(std::thread(updateCurrGen, std::ref(curr_gen), std::ref(next_gen), (*quotas)[iThread]));
    }
    for (int iThread = 0; iThread < thread_pool.size(); iThread++)
    {
        if (thread_pool[iThread].joinable())
            thread_pool[iThread].join();
    }
}