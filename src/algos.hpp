#pragma once
#include <fstream>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>
#include <array>
#include <bit>
#include <vector>
#include <filesystem>
#include <wx/stdpaths.h>
#include <wx/filename.h>

/**
 * @brief Helper function that prints a 1D array with size 81 as a 9x9 array in the terminal. Used for debugging.
 * 
 * @param arr  The array that will be printed.
 */
void printArr(std::array<int, 81> arr);
/**
 * @brief Loads a game from a .txt file location.
 * 
 * @param fileLoc  The location of the game.
 * @param gameNum  The game(row) that will be loaded.
 * @param target   The target array that the game will be loaded to.
 */
void LoadGame(std::string fileLoc, int gameNum, std::array<int, 81>& target);
/**
 * @brief Saves a game to a .txt file location.
 * 
 * @param fileLoc  The location that the game will be saved.
 * @param target   The target array/game that will be saved.
 */
void SaveGame(std::string fileLoc, std::array<int, 81>& grid);
/**
 * @brief Deletes the .txt game of the file given.
 * 
 * @param fileLoc  The location of the game that will be deleted.
 */
void DeleteGame(std::string filename);
/**
 * @brief Updates the relevant locations (row, column, 3x3 square) for a specific grid slot. Used when placing a value in the sudoku grid.
 * 
 * @param grid     The sudoku grid that will be used as a reference.
 * @param gridPos  The specific grid slot that will update the relevant cells, ranges from [0-80] (1D array).
 * @param counts   The array that stores the number of available spots for all cells.
 * @param choices  The array that stores the valid choices for all cells.
 */
void UpdateGridSlot(std::array<int, 81> &grid, int gridPos, std::array<int, 81> &counts, std::array<int, 81> &choices);
/**
 * @brief Initializes the counts and choices arrays based on the given sudoku grid.
 * 
 * @param grid    The sudoku grid that will be used as a reference.
 * @param counts  The array that stores the number of available spots for all cells.
 * @param choices The array that stores the valid choices for all cells.
 */
void InitState(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices);
/**
 * @brief Solves a 9x9 Sudoku board using recursive backtracking and constraint propagation.
 * 
 * Specifically it works the following way: It initializes 2 arrays that store the amount and the available choices for quick access and
 * afterwards it loops through by either placing all available immediate solutions, and if there is none, randomly placing the first choice on the cell with the least available
 * choices and recursively calling itself again, until a number of solutions is found.
 * 
 * @param grid             The sudoku grid that will be solved.
 * @param counts           The array that stores the number of available spots for all cells.
 * @param choices          The array that stores the valid choices for all cells.
 * @param solution         Output array where the completed valid board is written.
 * @param currentSolutions Recursion accumulator tracking the number of valid solutions found.
 * 
 * @return Total number of distinct solutions (int).
 */
int SolveSudoku(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices, std::array<int, 81> &solution, int currentSolutions = 0);
/**
 * @brief Creates a new sudoku puzzle by randomly removing values from a solved sudoku until the desired amount of clues is reached. This variant works from 
 * fetching a specific puzzle from a .txt with solutions.
 * 
 * @param fileLoc        The location of the .txt file containing the puzle
 * @param target         The sudoku grid that will be modified.
 * @param solvedTarget   A copy of the original solved grid
 * @param clues          The number of clues that the created sudoku will have    
 * @param maxNum         The max number (row) that can randomly be chosen, usually set to the number of puzzles the file has.
 */
void CreateSudokuFromTarget(std::string fileLoc, std::array<int, 81>& target, std::array<int, 81>& solvedTarget, int clues = 40, unsigned int maxNum = 256);
/**
 * @brief Creates a new sudoku puzzle by randomly removing values from a solved sudoku until the desired amount of clues is reached. This variant works directly on a 1D sudoku grid.
 * 
 * @param target         The sudoku grid that will be modified.
 * @param solvedTarget   A copy of the original solved grid
 * @param clues          The number of clues that the created sudoku will have  
 */
void CreateSudokuFromTarget(std::array<int, 81>& target, std::array<int, 81>& solvedTarget, int clues);
/**
 * @brief Generates a fully populated valid sudoku grid. Afterwards, CreateSudokuFromTarget can create a unique sudoku from this solved grid.
 * 
 * @param grid      The sudoku grid that will save the full solved grid.
 * @param counts    The array that stores the number of available spots for all cells.
 * @param choices   The array that stores the valid choices for all cells.
 */
bool CreateSudokuFromScratch(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices);
/**
 * @brief Helper function that helps find a relative file location.
 * 
 * @param filename The file location.
 */
std::string GetDataPath(std::string filename);