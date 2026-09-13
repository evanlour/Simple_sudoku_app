#include "Sudoku.hpp"

bool Sudoku::OnInit()
{
    frame = new SudokuFrame();
    frame->Show(true);
    return true;
}