#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>

#ifndef SUDOKU_H
#define SUDOKU_H

using namespace std;
#define N 9
#define S 3


void printBoard(const vector<vector<int>>& board){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            cout << ((board[i][j]==0) ? "." : to_string(board[i][j])) << " ";
        }
        cout << endl;
    }
}

bool isValid(vector<vector<int>>& board, int num, int row, int col){
    if (board[row][col] != 0) return false;

    for (int i = 0; i < N; i++){
        if (board[i][col] == num){
            return false;
        }
    }

    for (int j = 0; j < N; j++){
        if (board[row][j] == num){
            return false;
        }
    }

    int grid_row = row/3;
    int grid_col = col/3;
    grid_row *= 3;
    grid_col *= 3;
    for (int i = grid_row; i < grid_row + 3; i++){
        for (int j = grid_col; j < grid_col + 3; j++){
            if (board[i][j] == num){
                return false;
            }
        }
    }
    return true;
}

bool findEmptyCell(vector<vector<int>>& board, int& row, int& col){
    for (row = 0; row < N; ++row){
        for (col = 0; col < N; ++col){
            if (board[row][col] == 0){
                return true;
            }
        }
    }
    return false;
}

bool backtracking(vector<vector<int>>& board){
    int row, col;
    if (!findEmptyCell(board, row, col)){
        return true;
    }
    for (int num = 1; num <= 9; ++num){
        if (isValid(board, num, row, col)){
            board[row][col] = num;
            if (backtracking(board)){
                return true;
            }
            board[row][col] = 0;
        }
    } 
    return false;
}

int emptyCellsCount(const vector<vector<int>>& board){
    int count = 0;
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            if (board[i][j] == 0){
                count += 1;
            }
        }
    }
    return count;
}

#endif
