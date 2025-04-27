#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>
#include "sudoku_board.hpp"
#include "genetic_algorithm.hpp"

using namespace std;

int main(){

    vector<vector<int>> sudokuBoard = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    int population_size = 500;
    int nb_generations = 5000;
    int selection_rate = 60;
    int elitism_rate = 20;
    int mutation_rate = 20;

    runGeneticAlgorithm(sudokuBoard, population_size, nb_generations, selection_rate, elitism_rate, mutation_rate);
    
    //backtracking(sudokuBoard);
    //printBoard(sudokuBoard);

    return 0;
    
}
