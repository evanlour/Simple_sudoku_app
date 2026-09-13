#include "SudokuPanel.hpp"

const int BORDER_SIZE = 10;
const int INNER_BORDER_SIZE = 3;
const int FONT_SIZE = 10;
const int NOTE_SIZE = 6;

SudokuPanel::SudokuPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    currentTheme = Theme::Dark();
    noteMode = false;
    ResetBoard();
    cellPos = CellPosition::CellInit();
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    outerBorderPen = wxPen(wxColor(0, 0, 0), BORDER_SIZE);
    SetTheme(Theme::Dark());
    SetWindowStyleFlag(GetWindowStyleFlag() | wxWANTS_CHARS);

    Bind(wxEVT_PAINT, &SudokuPanel::RefreshPaint, this);
    Bind(wxEVT_LEFT_DOWN, &SudokuPanel::OnMouseDown, this);
    Bind(wxEVT_RIGHT_DOWN, &SudokuPanel::OnMouseDown, this);
    Bind(wxEVT_KEY_DOWN, &SudokuPanel::OnKeyDown, this);
    Refresh();
}

void SudokuPanel::SetTheme(const Theme& theme){
    currentTheme = theme;
    outerBorderPen = wxPen(currentTheme.Line, BORDER_SIZE);
    innerBorderPen = wxPen(currentTheme.Line, INNER_BORDER_SIZE);
    selectedBrush = wxBrush(currentTheme.cellSelected);
    backgroundBrush = wxBrush(currentTheme.background);
    lockedBrush = wxBrush(currentTheme.textLocked);
    Refresh();
}

void SudokuPanel::OnMouseDown(wxMouseEvent& event){
    SetFocus();
    wxPoint pos = event.GetPosition();
    wxSize windowSize = GetClientSize();
    int horizontalSize = windowSize.x - 2 * BORDER_SIZE;
    int verticalSize = windowSize.y - 2 * BORDER_SIZE;
    double horizontalCellSize = static_cast<double>((horizontalSize / 9.0));
    double verticalCellSize = static_cast<double>((verticalSize / 9.0));
    cellPos.x = static_cast<int>((pos.x - BORDER_SIZE) / horizontalCellSize);
    cellPos.y  = static_cast<int>((pos.y - BORDER_SIZE) / verticalCellSize);
    // Check if everything is in bounds
    if (cellPos.x > 8){cellPos.x = 8;};
    if (cellPos.y > 8){cellPos.y = 8;};
    if (cellPos.x < 0){cellPos.x = 0;};
    if (cellPos.y < 0){cellPos.y = 0;};

    // std::cout << "Mouse click on: X = " << pos.x << " Y = " << pos.y << std::endl; //DEBUG
    Refresh();
}

void SudokuPanel::OnResize(wxSizeEvent& event){
    Refresh();
    event.Skip();
}

void SudokuPanel::RefreshPaint(wxPaintEvent& event){
    wxAutoBufferedPaintDC dc(this);
    if(!solved){CheckCorrect();};

    // Background
    dc.SetBackground(backgroundBrush);
    dc.Clear();

    wxSize windowSize = GetClientSize();
    int horizontalSize = windowSize.x - 2 * BORDER_SIZE;
    int verticalSize = windowSize.y - 2 * BORDER_SIZE;
    double horizontalCellSize = static_cast<double>(horizontalSize) / 9.0;
    double verticalCellSize = static_cast<double>(verticalSize) / 9.0;
    double HALF_BORDER = static_cast<double>(BORDER_SIZE) / 2.0;

    //Inner borders
    dc.SetPen(innerBorderPen);
    for(int i = 1; i < 9; i++){
        dc.DrawLine(BORDER_SIZE, BORDER_SIZE + i * verticalCellSize, windowSize.x - BORDER_SIZE, BORDER_SIZE + i * verticalCellSize);   //Horizontal lines
        dc.DrawLine(BORDER_SIZE + i * horizontalCellSize, BORDER_SIZE, BORDER_SIZE + i * horizontalCellSize, windowSize.y - BORDER_SIZE); //Vertical lines
    }

    //Outer borders
    dc.SetPen(outerBorderPen);
    dc.DrawLine(HALF_BORDER, 0.0f, HALF_BORDER, windowSize.y);
    dc.DrawLine(windowSize.x - HALF_BORDER, 0.0f, windowSize.x - HALF_BORDER, windowSize.y);
    dc.DrawLine(0.0f, HALF_BORDER, windowSize.x, HALF_BORDER);
    dc.DrawLine(0.0f, windowSize.y - HALF_BORDER, windowSize.x, windowSize.y - HALF_BORDER);

    // Selected shell
    int x1 = BORDER_SIZE + static_cast<int>(std::round(cellPos.x * horizontalCellSize));
    int x2 = BORDER_SIZE + static_cast<int>(std::round((cellPos.x + 1) * horizontalCellSize));
    int y1 = BORDER_SIZE + static_cast<int>(std::round(cellPos.y * verticalCellSize));
    int y2 = BORDER_SIZE + static_cast<int>(std::round((cellPos.y + 1) * verticalCellSize));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(selectedBrush);
    dc.DrawRectangle(x1, y1, x2 - x1, y2 - y1);
    
    // Printing the numbers jason, what do they mean?
    primaryTextFont = wxFontInfo(static_cast<int>(verticalCellSize * 0.5)).Bold();
    secondaryTextFont = wxFontInfo(static_cast<int>(verticalCellSize * 0.25)).Bold();
    
    dc.SetBrush(lockedBrush);
    for(int row = 0; row < 9; row++){
        for(int col = 0; col < 9; col++){
            x1 = BORDER_SIZE + static_cast<int>(std::round(col * horizontalCellSize));
            x2 = BORDER_SIZE + static_cast<int>(std::round((col + 1) * horizontalCellSize));
            y1 = BORDER_SIZE + static_cast<int>(std::round(row * verticalCellSize));
            y2 = BORDER_SIZE + static_cast<int>(std::round((row + 1) * verticalCellSize));
            
            if (sudokuDefault[col][row] == true){
                dc.DrawRectangle(x1, y1, x2 - x1, y2 - y1);
            }

            if (sudokuBoard[col][row] != 0){
                dc.SetFont(primaryTextFont);
                dc.SetTextForeground(currentTheme.textPrimaryDefault);
                wxString primaryStr = wxString::Format("%d", sudokuBoard[col][row]);
                wxSize strExtent = dc.GetTextExtent(primaryStr);
                dc.DrawText(primaryStr, x1 + ((x2 - x1 - strExtent.x) / 2), y1 + ((y2 - y1 - strExtent.y) / 2));
            }

            if (sudokuNotes[col][row] != 0){
                dc.SetFont(secondaryTextFont);
                dc.SetTextForeground(currentTheme.textSecondaryDefault);
                wxString secondaryStr = wxString::Format("%d", sudokuNotes[col][row]);
                wxSize strExtent = dc.GetTextExtent(secondaryStr);
                dc.DrawText(secondaryStr, x1 + 4, y1 - 2);
            }
        }
    }  
}

void SudokuPanel::OnKeyDown(wxKeyEvent& event){
    int key = static_cast<int>(event.GetKeyCode()); // Returns uppercase letters
    switch(key){
        case 'W': case WXK_UP:    if(cellPos.y > 0){cellPos.y--;} break;
        case 'A': case WXK_LEFT:  if(cellPos.x > 0){cellPos.x--;} break;
        case 'S': case WXK_DOWN:  if(cellPos.y < 8){cellPos.y++;} break;
        case 'D': case WXK_RIGHT: if(cellPos.x < 8){cellPos.x++;} break;
        case 'N':
            noteMode = !noteMode;
            if (auto* parentFrame = dynamic_cast<wxFrame*>(wxGetTopLevelParent(this)))
                parentFrame->SetStatusText(noteMode ? "Mode: Notes" : "Mode: Normal");
            break;
        default:
            break;
    }
    // If immutable cell, do not try to modify
    if (sudokuDefault[cellPos.x][cellPos.y] == true){
        event.Skip();
        Refresh();
        return;
    }

    switch(key){
        case '0': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 0;} else {sudokuBoard[cellPos.x][cellPos.y] = 0;} break;
        case '1': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 1;} else {sudokuBoard[cellPos.x][cellPos.y] = 1;} break;
        case '2': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 2;} else {sudokuBoard[cellPos.x][cellPos.y] = 2;} break;
        case '3': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 3;} else {sudokuBoard[cellPos.x][cellPos.y] = 3;} break;
        case '4': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 4;} else {sudokuBoard[cellPos.x][cellPos.y] = 4;} break;
        case '5': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 5;} else {sudokuBoard[cellPos.x][cellPos.y] = 5;} break;
        case '6': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 6;} else {sudokuBoard[cellPos.x][cellPos.y] = 6;} break;
        case '7': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 7;} else {sudokuBoard[cellPos.x][cellPos.y] = 7;} break;
        case '8': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 8;} else {sudokuBoard[cellPos.x][cellPos.y] = 8;} break;
        case '9': if(noteMode){sudokuNotes[cellPos.x][cellPos.y] = 9;} else {sudokuBoard[cellPos.x][cellPos.y] = 9;} break;
        default:
            event.Skip();
            break;
    }
    Refresh();
}

void SudokuPanel::CheckCorrect(){
    int correctVals = 0;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(sudokuBoard[i][j] == sudokuCorrect[i][j]){correctVals++;};
        }
    }
    if (correctVals >= 81){
        wxLogMessage("Congratulations you managed to solve it! Mpinos would be proud!");
        solved = true;
        for(int i = 0; i < 81; i++){
            for (auto& row: sudokuDefault){
                row.fill(true);
            }
        }
    }
}

void SudokuPanel::LoadGameFromFile(std::string fileLoc, int index){
    std::array<int, 81> tempArr{};
    std::array<int, 81> tempCounts{};
    std::array<int, 81> tempChoices{};
    std::array<int, 81> tempCorrect{};
    
    LoadGame(static_cast<std::string>(fileLoc), index, tempArr);
    ResetBoard();
    for(int i = 0; i < 81; i++){
        sudokuBoard[i / 9][i % 9] = tempArr[i];
        if (tempArr[i] != 0){
            sudokuDefault[i / 9][i % 9] = true;
        }
    }
    InitState(tempArr, tempCounts, tempChoices);
    int solutions = SolveSudoku(tempArr, tempCounts, tempChoices, tempCorrect);
    if(solutions == 0){
        wxLogMessage("Given Sudoku has no possible solutions detected...");
        return;
    }else if(solutions >= 2){
        wxLogMessage("Sudoku has more than one solution, therefore it is incorrect...");
        return;
    }else{
        wxLogMessage("Sudoku with single solution, game loaded!");
        solved=false;
    }

    for(int i = 0; i < 81; i++){
        sudokuCorrect[i / 9][i % 9] = tempCorrect[i];
    }
    Refresh();
}

void SudokuPanel::SaveGameToFile(std::string fileLoc){
    std::array<int, 81> tempArr{};
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            tempArr[(i * 9) + j] = sudokuBoard[i][j];
        }
    }
    SaveGame(fileLoc, tempArr);
}

void SudokuPanel::GenerateGameFromFile(int clues){
    ResetBoard();
    std::array<int, 81> createdSudoku{};
    std::array<int, 81> createdSudokuSolution{};
    CreateSudokuFromTarget(GetDataPath("solutions.txt"), createdSudoku, createdSudokuSolution, clues, 2200);
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            sudokuBoard[i][j] = createdSudoku[i * 9 + j];
            if(createdSudoku[i * 9 + j] != 0){
                sudokuDefault[i][j] = true;
            }
            sudokuCorrect[i][j] = createdSudokuSolution[i * 9 + j];
        }
    }
    wxLogMessage("Loaded!");
}

void SudokuPanel::GenerateGameFromScratch(int clues){
    ResetBoard();
    std::array<int, 81> createdSudokuSolution{};
    std::array<int, 81> createdSudokuCorrect{};
    std::array<int, 81> createdSudokuCounts{};
    std::array<int, 81> createdSudokuChoices{};
    InitState(createdSudokuCorrect, createdSudokuCounts, createdSudokuChoices);
    CreateSudokuFromScratch(createdSudokuCorrect, createdSudokuCounts, createdSudokuChoices);
    createdSudokuSolution = createdSudokuCorrect;
    CreateSudokuFromTarget(createdSudokuSolution, createdSudokuCorrect, clues);
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            sudokuBoard[i][j] = createdSudokuSolution[i * 9 + j];
            if(createdSudokuSolution[i * 9 + j] != 0){
                sudokuDefault[i][j] = true;
            }
            sudokuCorrect[i][j] = createdSudokuCorrect[i * 9 + j];
        }
    }
    wxLogMessage("Loaded!");
}

void SudokuPanel::ResetBoard(){
    for (auto& row: sudokuBoard){
        row.fill(0);
    }
    for (auto& row: sudokuNotes){
        row.fill(0);
    }
    for (auto& row: sudokuCorrect){
        row.fill(67);
    }
    for (auto& row: sudokuDefault){
        row.fill(false);
    }
}