#pragma once
#include <array>
#include <wx/panel.h>
#include <wx/dcbuffer.h>
#include <wx/window.h>
#include <wx/log.h>
#include <wx/frame.h>
#include <iostream>
#include "Themes.h"
#include "algos.hpp"

struct CellPosition{
    int x;
    int y;
    static CellPosition CellInit(){
        return {0, 0};
    }
};

class SudokuPanel : public wxPanel 
{
public:
    std::array<std::array<int, 9>, 9> sudokuBoard;
    std::array<std::array<int, 9>, 9> sudokuCorrect;
    std::array<std::array<bool, 9>, 9> sudokuDefault;
    /**
     * @brief Initializes the panel and all processes inside.
     * 
     * @param parent The parent frame.
     */
    SudokuPanel(wxWindow* parent);
    /**
     * @brief Default constructor, unused for this app.
     */
    SudokuPanel();
    /**
     * @brief Sets the appropriate colors from the given theme struct.
     * 
     * @param theme The theme struct that holds all the color values.
     */
    void SetTheme(const Theme& theme);
    /**
     * @brief Loads a game from a .txt file.
     * 
     * @param index If there are multiple rows(games) inside the file, choose the appropriate row.
     */
    void LoadGameFromFile(std::string fileLoc, int index = 0);
    /**
     * @brief Saves a sudoku game as a .txt file to the given location.
     * 
     * @param fileLoc The location that the game will be saved.
     */
    void SaveGameToFile(std::string fileLoc);
    /**
     * @brief Generates a sudoku game based on the GenerateSudokuFromTarget function.
     * 
     * @param clues The number of clues the game will have.
     */
    void GenerateGameFromFile(int clues);
    /**
     * @brief Generates a sudoku game based on the GenerateSudokuFromScratch function.
     * 
     * @param clues The number of clues the game will have.
     */
    void GenerateGameFromScratch(int clues);
private:
    bool solved = false;
    bool noteMode = false;
    CellPosition cellPos;
    Theme currentTheme;
    wxPen outerBorderPen;
    wxPen innerBorderPen;
    wxBrush backgroundBrush;
    wxBrush selectedBrush;
    wxFont primaryTextFont;
    wxFont secondaryTextFont;
    wxBrush lockedBrush;
    std::array<std::array<int, 9>, 9> sudokuNotes;
    /**
     * @brief Handles mouse behavior for the panel, mainly used for choosing a grid to type.
     * 
     * @param event The event that triggers this function.
     */
    void OnMouseDown(wxMouseEvent& event);
    /**
     * @brief Handles keyboard behavior for the panel. Includes changing to note mode, numbers 1-9 etc.
     * 
     * @param event The event that triggers this function.
     */
    void OnKeyDown(wxKeyEvent& event);
    /**
     * @brief Determines what and how is repainted after a Refresh() call. Updates board, handles default clues etc.
     * 
     * @param event The event that triggers this function.
     */
    void RefreshPaint(wxPaintEvent& event);
    /**
     * @brief Handles window behavior when the window is resized from the user.
     * 
     * @param event The event that triggers this function.
     */
    void OnResize(wxSizeEvent& event);
    /**
     * @brief Checks if the board is solved correctly after each input.
     */
    void CheckCorrect();
    /**
     * @brief Called after every load and save in order to properly reset the board.
     */
    void ResetBoard();
};