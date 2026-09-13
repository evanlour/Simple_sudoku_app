#include "algos.hpp"

int main(){
    std::array<int, 81> correctArr{};
    std::array<int, 81> givenArr{};
    std::array<int, 81> solvedArr{};
    std::array<int, 81> counts{};
    std::array<int, 81> choices{};
    // Simulating loading and solving a sudoku
    LoadGame("/home/evanlour/Projects/widget_proto/hello_world/data/puzzles.txt", 3, givenArr);
    LoadGame("/home/evanlour/Projects/widget_proto/hello_world/data/solutions.txt", 3, correctArr);
    InitState(givenArr, counts, choices);
    int solutions = SolveSudoku(givenArr, counts, choices, solvedArr);
    if(solutions == 0){
        std::cout << "No valid solution found.\n";
    }else if(solutions == 1){
        std::cout << "Sudoku has one clear solution.\n";
    }else{
        std::cout << "Sudoku is not valid, multiple solutions detected.\n";
    }

    printf("The given array is:\n");
    printArr(givenArr);
    printf("The solved array is:\n");
    printArr(solvedArr);
    printf("The correct array is:\n");
    printArr(correctArr);
    //Creating one from another board
    std::array<int, 81> createdSudoku{};
    std::array<int, 81> createdSudokuSolution{};
    std::array<int, 81> createdSudokuCorrect{};
    std::array<int, 81> createdSudokuChoices{};
    std::array<int, 81> createdSudokuCounts{};

    // Creating one from scratch
    std::array<int, 81> generatedSudoku{};
    std::array<int, 81> generatedSudokuSolution{};
    std::array<int, 81> generatedSudokuCorrect{};
    std::array<int, 81> generatedSudokuCounts{};
    std::array<int, 81> generatedSudokuChoices{};

    
    CreateSudokuFromTarget("/home/evanlour/Projects/widget_proto/hello_world/data/solutions.txt", createdSudoku, createdSudokuCorrect, 30);
    printf("The created sudoku is:\n");
    printArr(createdSudoku);
    printf("The created sudoku solution is:\n");
    InitState(createdSudoku, createdSudokuCounts, createdSudokuChoices);
    SolveSudoku(createdSudoku, createdSudokuCounts, createdSudokuChoices, createdSudokuSolution);
    printArr(createdSudokuSolution);
    printf("The created sudoku correct board is:\n");
    printArr(createdSudokuCorrect);

    InitState(generatedSudokuCorrect, generatedSudokuCounts, generatedSudokuChoices);
    CreateSudokuFromScratch(generatedSudokuCorrect, generatedSudokuCounts, generatedSudokuChoices);
    printf("The generated sudoku is:\n");
    printArr(generatedSudokuCorrect);
    SaveGame("./test_sol.txt", generatedSudokuCorrect);
    generatedSudokuCounts = {};
    generatedSudokuChoices = {};
    CreateSudokuFromTarget("./test_sol.txt", generatedSudoku, generatedSudokuSolution, 30, 0);
    printf("The generated unsolved sudoku is:\n");
    printArr(generatedSudoku);
    printf("The current correct is:(crosscheck with the previous)\n");
    printArr(generatedSudokuSolution);
    InitState(generatedSudoku, generatedSudokuCounts, generatedSudokuChoices);
    SolveSudoku(generatedSudoku, generatedSudokuCounts, generatedSudokuChoices, generatedSudokuSolution);
    printf("The generated sudoku solution is:\n");
    printArr(generatedSudokuSolution);
    DeleteGame("./test_sol.txt");
    return 0;
}