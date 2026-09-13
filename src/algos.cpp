#include "algos.hpp"

void printArr(std::array<int, 81> arr){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            std::cout << arr[i * 9 + j] << " ";
        }
        std::cout << std::endl;
    }
}

void LoadGame(std::string fileLoc, int gameNum, std::array<int, 81>& target){
    std::ifstream f(fileLoc, std::ios::binary);
    if (!f){
        std::cerr << "Failed to open file\n";
        return;
    }
    std::string firstLine;
    std::string game;
    std::getline(f, firstLine);
    std::streampos lineSize = f.tellg();
    std::streampos offset = static_cast<std::streampos>(gameNum) * lineSize;
    f.seekg(offset);
    std::getline(f, game);
    if (!game.empty() && game.back() == '\r') {
        game.pop_back();
    }
    for(int i = 0; i < 81; i++){
        target[i] = (game[i] >= '1' && game[i] <= '9') ? (game[i] - '0') : 0;
    }
}

void SaveGame(std::string fileLoc, std::array<int, 81>& grid) {
    std::ofstream f(fileLoc, std::ios::binary | std::ios::app);
    if (!f) return;

    for (int val : grid) {
        f << (val == 0 ? '.' : static_cast<char>('0' + val));
    }
    f << '\n';
}


void DeleteGame(std::string filename) {
    try {
        if (std::filesystem::remove(filename)) {
            std::cout << "File " << filename << " deleted successfully.\n";
        } else {
            std::cout << "File " << filename << " did not exist.\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
}

void UpdateGridSlot(std::array<int, 81> &grid, int gridPos, std::array<int, 81> &counts, std::array<int, 81> &choices) {
    int val = grid[gridPos];
    if (val == 0) return;

    int mask = ~(1 << (val - 1));
    int row = gridPos / 9;
    int col = gridPos % 9;
    int cellX = (row / 3) * 3;
    int cellY = (col / 3) * 3;

    for (int j = 0; j < 9; j++) {
        choices[row * 9 + j] &= mask;                           // Row
        choices[j * 9 + col] &= mask;                           // Col
        choices[(cellX + j / 3) * 9 + (cellY + j % 3)] &= mask; // 3x3 Box
    }

    // Recalculate remaining candidate counts for empty cells
    for (int i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            counts[i] = std::popcount(static_cast<unsigned int>(choices[i]));
        }
    }
}

void InitState(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices) {
    for (int i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            choices[i] = 0x1FF; // Bits 0-8 set (all numbers 1-9 possible)
            counts[i] = 9;
        } else {
            choices[i] = 0;
            counts[i] = 0;
        }
    }

    for (int i = 0; i < 81; i++) {
        if (grid[i] != 0) {
            UpdateGridSlot(grid, i, counts, choices);
        }
    }
}

int SolveSudoku(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices, std::array<int, 81> &solution, int currentSolutions) {
    // Remove pending single options
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < 81; i++) {
            if (grid[i] != 0) continue;

            if (counts[i] == 0) return currentSolutions; // Impossible to solve as there is no valid number for this spot

            if (counts[i] == 1) {
                int val = std::countr_zero(static_cast<unsigned int>(choices[i])) + 1;
                grid[i] = val;
                choices[i] = 0;
                counts[i] = 0;
                UpdateGridSlot(grid, i, counts, choices);
                changed = true;
                break;
            }
        }
    } while (changed);

    // Minimum Remaining Values (MRV) heuristic when 2 or more minimum for all remain
    int minPos = -1;
    int minCount = 10;
    for (int i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            if (counts[i] == 0) return currentSolutions; //Same as before, impossible if no available numbers
            if (counts[i] < minCount) {
                minCount = counts[i];
                minPos = i;
            }
        }
    }

    // If no zeros remain, the grid is solved
    if (minPos == -1) {
        solution = grid;
        return currentSolutions + 1;
    }

    // Backtracking for the first logged available position
    int candidateMask = choices[minPos];
    while (candidateMask != 0 && currentSolutions < 2) {
        int bit = candidateMask & -candidateMask; // Retrieve the first choice
        int val = std::countr_zero(static_cast<unsigned int>(bit)) + 1;
        candidateMask &= ~bit;

        std::array<int, 81> nextGrid = grid;
        std::array<int, 81> nextCounts = counts;
        std::array<int, 81> nextChoices = choices;

        // Setup the grid to recursively test
        nextGrid[minPos] = val;
        nextChoices[minPos] = 0;
        nextCounts[minPos] = 0;
        UpdateGridSlot(nextGrid, minPos, nextCounts, nextChoices);

        currentSolutions = SolveSudoku(nextGrid, nextCounts, nextChoices, solution, currentSolutions);
    }

    return currentSolutions;
}

void CreateSudokuFromTarget(std::string fileLoc, std::array<int, 81>& target, std::array<int, 81>& solvedTarget, int clues, unsigned int maxNum){
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, maxNum);
    unsigned int randomNumber = dist(rng);
    LoadGame(fileLoc, randomNumber, target);
    
    std::array<int, 81> cellRemoveOrder;
    std::iota(cellRemoveOrder.begin(), cellRemoveOrder.end(), 0); //{0, 1, 2 ... , 81}
    std::shuffle(cellRemoveOrder.begin(), cellRemoveOrder.end(), std::mt19937{std::random_device{}()});

    std::array<int, 81> dummyChoices;
    std::array<int, 81> dummyCounts;
    std::array<int, 81> dummySolved;
    solvedTarget = target;
    
    int cluesLeft = 81;
    for(auto pos: cellRemoveOrder){
        if (cluesLeft <= clues) break;
        int removedVal = target[pos];
        target[pos] = 0;
        std::array<int, 81> testGrid = target;
        InitState(testGrid, dummyCounts, dummyChoices);
        if (SolveSudoku(testGrid, dummyCounts, dummyChoices, dummySolved) != 1){
            // Bad sudoku, lets try the next one
            target[pos] = removedVal;
        }else{
            cluesLeft--;
        }
    }
}

void CreateSudokuFromTarget(std::array<int, 81>& target, std::array<int, 81>& solvedTarget, int clues){
    std::array<int, 81> cellRemoveOrder;
    std::iota(cellRemoveOrder.begin(), cellRemoveOrder.end(), 0); //{0, 1, 2 ... , 81}
    std::shuffle(cellRemoveOrder.begin(), cellRemoveOrder.end(), std::mt19937{std::random_device{}()});

    std::array<int, 81> dummyChoices;
    std::array<int, 81> dummyCounts;
    std::array<int, 81> dummySolved;
    solvedTarget = target;
    
    int cluesLeft = 81;
    for(auto pos: cellRemoveOrder){
        if (cluesLeft <= clues) break;
        int removedVal = target[pos];
        target[pos] = 0;
        std::array<int, 81> testGrid = target;
        InitState(testGrid, dummyCounts, dummyChoices);
        if (SolveSudoku(testGrid, dummyCounts, dummyChoices, dummySolved) != 1){
            // Bad sudoku, lets try the next one
            target[pos] = removedVal;
        }else{
            cluesLeft--;
        }
    }
}

bool CreateSudokuFromScratch(std::array<int, 81> &grid, std::array<int, 81> &counts, std::array<int, 81> &choices) {
    int minPos = -1;
    int minCount = 10;

    for (int i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            if (counts[i] == 0) return false;
            if (counts[i] < minCount) {
                minCount = counts[i];
                minPos = i;
            }
        }
    }

    if (minPos == -1) return true; // Board is full

    // Check available digits
    int candidateMask = choices[minPos];
    std::vector<int> candidates;
    while (candidateMask != 0) {
        int bit = candidateMask & -candidateMask;
        candidates.push_back(std::countr_zero(static_cast<unsigned int>(bit)) + 1);
        candidateMask &= ~bit;
    }

    std::shuffle(candidates.begin(), candidates.end(), std::mt19937{std::random_device{}()});

    for (auto val : candidates) {
        std::array<int, 81> testGrid = grid;
        std::array<int, 81> dummyCounts = counts;
        std::array<int, 81> dummyChoices = choices;

        testGrid[minPos] = val;
        dummyChoices[minPos] = 0;
        dummyCounts[minPos] = 0;
        UpdateGridSlot(testGrid, minPos, dummyCounts, dummyChoices);

        if (CreateSudokuFromScratch(testGrid, dummyCounts, dummyChoices)) {
            grid = testGrid;
            return true;
        }
    }
    return false;
}

std::string GetDataPath(std::string filename) {
    // Finds the exact directory where the running binary lives
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxFileName fn(exePath);
    
    // Build: <folder_of_exe>/data/<filename>
    fn.SetFullName("");
    fn.AppendDir("data");
    fn.SetName(filename);

    return fn.GetFullPath().ToStdString();
}