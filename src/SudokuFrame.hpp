#pragma once
#include <iostream>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include "SudokuPanel.hpp"
#include "Themes.h"

class SudokuFrame : public wxFrame
{
public:
    /**
     * @brief Constructs the sudoku frame, containing the menu bar and the panel.
     */
    SudokuFrame();
 
private:
    wxMenu *menuFile;
    wxMenu *menuHelp;
    wxMenu *menuTheme;
    wxMenu *subMenuFile;
    wxMenuBar *menuBar;
    SudokuPanel *panel;
    wxBoxSizer* sizer;
    std::vector<Theme> themes;
    /**
     * @brief Submenu that toggles a pop up window in order to start a puzzle from 5 presets, from very easy to very hard.
     * @param event The click event that toggles the whole process.
     */
    void NewPreset(wxCommandEvent& event);
     /**
     * @brief Submenu that toggles a pop up window in order to start a custom puzzle with 2 methods, from a pre-existing puzzle or a generated one. Instead of difficulty, it uses the number of clues that it will have.
     * @param event The click event that toggles the whole process.
     */
    void NewCustom(wxCommandEvent& event);
    /**
     * @brief Prompts the user to select a saved .txt game to load using the file explorer.
     * @param event The click event that toggles the whole process.
     */
    void Load(wxCommandEvent& event);
    /**
     * @brief Prompts the user to save the game as a .txt file using the file explorer.
     * @param event The click event that toggles the whole process.
     */
    void Save(wxCommandEvent& event);
    /**
     * @brief Fires up a window asking the user if he is sure about quitting the application.
     * @param event The click event that toggles the whole process.
     */
    void OnExit(wxCommandEvent& event);
    /**
     * @brief Fires up a window with the available keyboard moves along with a brief explanation of the app.
     * @param event The click event that toggles the whole process.
     */
    void OnAbout(wxCommandEvent& event);
    /**
     * @brief Handles the behavior of the window popping up when asked to close the application.
     * @param event The click event that toggles the whole process.
     */
    void OnClose(wxCloseEvent& event);
    /**
     * @brief Handles the behavior when a theme is selected from the menu.
     * @param event The click event that toggles the whole process.
     */
    void OnThemeSelect(wxCommandEvent& event);
};