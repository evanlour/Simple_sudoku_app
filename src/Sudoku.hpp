#pragma once
#include <wx/app.h>
#include "SudokuFrame.hpp"

class Sudoku : public wxApp
{
public:
    bool OnInit() override;
private:
    SudokuFrame *frame;
};