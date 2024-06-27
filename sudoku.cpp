#include <iostream>
#include <fstream>
using namespace std;

#define N 9

bool isSafe(int grid[N][N], int row, int col, int num) {
    // Check if 'num' is not in the current row, column, and 3x3 subgrid
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num)
                return false;
        }
    }
    return true;
}

bool solveSudoku(int grid[N][N], int row, int col) {
    if (row == N - 1 && col == N) // If we have reached the last cell
        return true;

    if (col == N) { // If we have reached the end of the row, move to the next row
        row++;
        col = 0;
    }

    if (grid[row][col] != 0) // If the current cell is already filled, move to the next cell
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) { // Check if placing num in grid[row][col] is safe
            grid[row][col] = num;
            if (solveSudoku(grid, row, col + 1)) // Recur for the next cell
                return true;
            grid[row][col] = 0; // Backtrack if placing num in grid[row][col] doesn't lead to a solution
        }
    }
    return false; // Trigger backtracking
}

void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

void readSudokuFromFile(int grid[N][N], const char* filename) {
    ifstream file(filename);
    if (file.is_open()) {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                file >> grid[row][col];
            }
        }
        file.close();
    } else {
        cout << "Unable to open file" << endl;
    }
}

int main() {
    int grid[N][N];
    readSudokuFromFile(grid, "sudoku.txt");

    if (solveSudoku(grid, 0, 0)) {
        printGrid(grid);
    } else {
        cout << "No solution exists" << endl;
    }

    return 0;
}
