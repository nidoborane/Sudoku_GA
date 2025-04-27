#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>
#include "sudoku_board.hpp"

#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

using namespace std;

void placeChromosome(vector<vector<int>>& board, vector<int>& chromosome){       
    int index = 0;  
    for (int gridRow = 0; gridRow < N; gridRow += 3) {
        for (int gridCol = 0; gridCol < N; gridCol += 3) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[gridRow + i][gridCol + j] == 0 && index < chromosome.size()) {
                        board[gridRow + i][gridCol + j] = chromosome[index++];
                    }
                }
            }
        }
    }
}

void removeChromosome(vector<vector<int>>& board, vector<int>& chromosome){
    int index = 0;  
    for (int gridRow = 0; gridRow < N; gridRow += 3) {
        for (int gridCol = 0; gridCol < N; gridCol += 3) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[gridRow + i][gridCol + j] == chromosome[index] && index < chromosome.size()) {
                        board[gridRow + i][gridCol + j] = 0;
                        index++;
                    }
                }
            }
        }
    }
}

void sliceGrid(vector<vector<int>>& board, vector<vector<int>>& subgrid, int row, int col){
    for (int i = row; i < row + 3; ++i){
        vector<int> temp;
        for (int j = col; j < col + 3; ++j){
            temp.push_back(board[i][j]);
        }
        subgrid.push_back(temp);
    }
}

void generateSegment(vector<vector<int>>& subgrid, vector<int>& segment){
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < S; ++i){
        for (int j = 0; j < S; ++j){
            if (subgrid[i][j] != 0){
                nums.erase(std::remove(nums.begin(), nums.end(), subgrid[i][j]), nums.end());
            }
        }
    }

    random_device rd;
    mt19937 g(rd());

    shuffle(nums.begin(), nums.end(), g);
    segment = nums;
}

void generateChromosome(vector<vector<int>>& board, vector<int>& chromosome){
    for (int i = 0; i < 3; ++i){       
        for (int j = 0; j < 3; ++j){
            vector<int> segment;
            vector<vector<int>> subgrid;
            sliceGrid(board, subgrid, 3 * i, 3 * j);
            generateSegment(subgrid, segment);
            chromosome.insert(chromosome.end(), segment.begin(), segment.end());
        }
    }
}

vector<vector<int>> generateInitialPopulation(vector<vector<int>>& board, int population_size){
    vector<vector<int>> population;
    for (int i = 0; i < population_size; ++i){
        vector<int> chromosome;
        generateChromosome(board, chromosome);
        population.push_back(chromosome);
    }
    return population;
}

int countDuplicates(vector<vector<int>> board, vector<int>& chromosome){
    placeChromosome(board, chromosome);
    int duplicates_row = 0;
    int duplicates_col = 0;
    for (int row = 0; row < N; ++row){
        unordered_map<int, int> freqMap;
        for (int col = 0; col < N; ++col){
            freqMap[board[row][col]]++;
        }
        for (auto pair : freqMap){
            if (pair.second > 1){
                duplicates_row += pair.second - 1;
            }
        }
    }

    for (int col = 0; col < N; ++col){
        unordered_map<int, int> freqMap;
        for (int row = 0; row < N; ++row){
            freqMap[board[row][col]]++;
        }
        for (auto pair : freqMap){
            if (pair.second > 1){
                duplicates_col += pair.second - 1;
            }
        }
    }

    return (duplicates_row + duplicates_col);
}

vector<int> costFunction(vector<vector<int>>& board, vector<vector<int>>& population){
    
    vector<int> scores;
    int population_size = static_cast<int>(population.size());
    for (int i = 0; i < population_size; i++){
        vector<int> chromosome = population[i];
        scores.push_back(countDuplicates(board, chromosome));
    }
    return scores;
}

vector<int> crossover(vector<vector<int>>& board, vector<int>& parent1, vector<int>& parent2){
    random_device rd;  
    mt19937 gen(rd()); 

    int lower{2}, upper{8};
    uniform_int_distribution<int> num_grid(lower, upper);
    int crossover_grid{num_grid(gen)};

    vector<int> child;
    int index = 0;
    int row{0}, col{0};
    int grid = 1;
    while (grid < crossover_grid){
        int gridRow = ((grid - 1) / 3) * 3;
        int gridCol = ((grid - 1) % 3) * 3;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[gridRow + i][gridCol + j] == 0) ++index;
            }
        }
        grid++;
    }
    
    child = parent1;
    
    for (int i = index; i < parent2.size(); i++){
        child[i] = parent2[i];
    } 

    return child;
}

void sortChromosomes(vector<vector<int>>& board, vector<vector<int>>& population , vector<pair<int, int>>& rankings){
    vector<int> scores{costFunction(board, population)};

    for (int i = 0; i < scores.size(); i++){
        rankings.emplace_back(i, scores[i]);
    }
    
    sort(rankings.begin(), rankings.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second < b.second;
    });
}

void mutate(vector<vector<int>>& board, vector<int>& chromosome){
    random_device rd;  
    mt19937 gen(rd()); 

    int lower = 2, upper = N - 1;
    uniform_int_distribution<int> grid_dist(lower, upper);

    int random_subgrid = grid_dist(gen);

    int current_subgrid{1};
    int subgrid_lower_index{0};
    int subgrid_upper_index{0};
 
    while ((random_subgrid - current_subgrid) >= 0) {
        int row = ((current_subgrid - 1) / 3) * 3;
        int col = ((current_subgrid - 1) % 3) * 3;
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                if (board[row + i][col + j] == 0){
                    if ((random_subgrid - current_subgrid) > 0){
                        subgrid_lower_index++;
                    }
                    else{
                        subgrid_upper_index++;
                    }
                }
            }
        }
        current_subgrid++;
    }
    subgrid_upper_index += (subgrid_lower_index - 1);

    uniform_int_distribution<int> num_dist(subgrid_lower_index, subgrid_upper_index);
    int num1 = num_dist(gen);
    int num2;
    
    do {
        num2 = num_dist(gen);
    } while (num2 == num1);

    int temp;
    temp = chromosome[num2];
    chromosome[num2] = chromosome[num1];
    chromosome[num1] = temp;
}

void generation(vector<vector<int>>& board, vector<vector<int>>& population, int selection_rate, int elitism_rate, int mutation_rate){
    
    vector<vector<int>> next_generation;
    vector<pair<int, int>> rankings;
    vector<vector<int>> parents;

    int offsprings = selection_rate * population.size() / 100;
    int elite_chromosomes = elitism_rate * population.size() /100;
    int mutations = mutation_rate * population.size() / 100;
    int random_chromosomes = (100 - (elitism_rate + selection_rate + mutation_rate)) * population.size() / 100;

    sortChromosomes(board, population, rankings); //Modify this to instead output the sorted population matrix based on the cost function

    for (int i = 0; i < elite_chromosomes; i++){
        next_generation.push_back(population[rankings[i].first]);
    }

    int lower{0}, upper{static_cast<int>(population.size() - 1)};
    random_device rd;  
    mt19937 gen(rd()); 

    uniform_int_distribution<int> chromosome_index(lower, upper);  //to generate a random chromosome (index) for crossover/mutation 

    for (int j = 0; j < offsprings; j++){
        int parent1 = chromosome_index(gen);
        int parent2;
        
        do {
            parent2 = chromosome_index(gen);
        } while (parent2 == parent1);
        
        next_generation.push_back(crossover(board, population[parent1], population[parent2]));
    }

    for (int k = 0; k < mutations; k++){
        vector<int> chromosomes_to_mutate = population[chromosome_index(gen)];
        mutate(board, chromosomes_to_mutate);
        next_generation.push_back(chromosomes_to_mutate);
    }

    for (int w = 0; w < random_chromosomes; w++){
        vector<int> chromosome;
        generateChromosome(board, chromosome);
        next_generation.push_back(chromosome);
    }
    population = next_generation;
}

pair<int, int> leastConflicts(vector<vector<int>>& board, vector<vector<int>>& population){
    vector<pair<int, int>> rankings;
    sortChromosomes(board, population, rankings);
    return rankings[0];
}

void runGeneticAlgorithm(vector<vector<int>>& board, int population_size, int nb_generations, int selection_rate, int elitism_rate, int mutation_rate){

    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> population = generateInitialPopulation(board, population_size);

    for (int gen = 0; gen < nb_generations; gen++){
        pair<int, int> bestChromosome = leastConflicts(board, population);
        cout << "Generation : " << (gen + 1) << " | " << "Conflicts : " << bestChromosome.second << endl;
        if (bestChromosome.second == 0){
            gen = nb_generations;
            cout << "Solved sudoku board :" << endl;
            placeChromosome(board, population[bestChromosome.first]);
            printBoard(board);
            //removeChromosome(board, population[bestChromosome.first]);
        }
        generation(board, population, selection_rate, elitism_rate, mutation_rate);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start).count();

    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;

    cout << "Sudoku board not solved..." << endl;
    cout << "Time elapsed : " << hours << "h:" << minutes << "m:" << seconds << "s\n";
}

#endif
