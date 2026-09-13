#include "SudokuFrame.hpp"

enum
{
    ID_New = 10000,
    ID_Load,
    ID_Save,
    ID_New_Custom,
    ID_New_Preset
};

SudokuFrame::SudokuFrame()
    : wxFrame(nullptr, wxID_ANY, "Sudoku", wxDefaultPosition, wxSize(800, 600))
{
    menuFile = new wxMenu;
    subMenuFile = new wxMenu;
    subMenuFile->Append(ID_New_Preset, "&Presets", "Choose a predefined preset");
    subMenuFile->Append(ID_New_Custom, "&Custom", "Create a custom board");
    menuFile->AppendSubMenu(subMenuFile, "&New");

    menuFile->AppendSeparator();
    menuFile->Append(ID_Load, "&Load", "Load a saved game");

    menuFile->AppendSeparator();
    menuFile->Append(ID_Save, "&Save", "Save current game");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    menuTheme = new wxMenu;
    themes = Theme::getAvailableThemes();
    for (size_t i = 0; i < themes.size(); i++){
        menuTheme->AppendRadioItem(themes[i].ID, themes[i].name);
        if (i == 1){ // Dark theme supremacy
            menuTheme->Check(themes[i].ID, true);
        }
    }

    menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuTheme, "&Theme");
    SetMenuBar(menuBar);

    panel = new SudokuPanel(this);
        
    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND);
    SetSizer(sizer);
    
    CreateStatusBar();
    SetStatusText("Mode: Normal");
 
    Bind(wxEVT_MENU, &SudokuFrame::NewPreset, this, ID_New_Preset);
    Bind(wxEVT_MENU, &SudokuFrame::NewCustom, this, ID_New_Custom);
    Bind(wxEVT_MENU, &SudokuFrame::Load, this, ID_Load);
    Bind(wxEVT_MENU, &SudokuFrame::Save, this, ID_Save);
    Bind(wxEVT_MENU, &SudokuFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &SudokuFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &SudokuFrame::OnClose, this);
    Bind(wxEVT_MENU, &SudokuFrame::OnThemeSelect, this, themes.front().ID, themes.back().ID);
}
 
void SudokuFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void SudokuFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a simple sudoku game i created in order to get to grips with wxwidget.\n"
        "You can use the WASD and arrow keys to navigate the grid.\n"
        "Also, you can write a note number by changing to notes mode using the letter N.","About", wxOK | wxICON_INFORMATION);
}
 
void SudokuFrame::NewPreset(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Select Puzzle");
    auto* sizer = new wxBoxSizer(wxVERTICAL);

    const int TOTAL_PUZZLES = 2203;
    const int TIERS = 5;
    const int perTier = TOTAL_PUZZLES / TIERS; // the last 3 puzzles will be unused, bummer

    wxArrayString difficulties{"Very Easy", "Easy", "Medium", "Hard", "Very Hard"};

    auto* radio = new wxRadioBox(&dlg, wxID_ANY, "Difficulty", 
                                 wxDefaultPosition, wxDefaultSize, 
                                 difficulties, 1, wxRA_SPECIFY_COLS);

    auto* spinLabel = new wxStaticText(&dlg, wxID_ANY, "Puzzle Index:");
    auto* spin = new wxSpinCtrl(&dlg, wxID_ANY, wxEmptyString, 
                                wxDefaultPosition, wxDefaultSize, 
                                wxSP_ARROW_KEYS, 1, perTier, 1);
    spin->SetRange(1, perTier);

    sizer->Add(radio, 0, wxALL | wxEXPAND, 10);
    sizer->Add(spinLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    sizer->Add(spin, 0, wxALL | wxEXPAND, 10);
    sizer->Add(dlg.CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxEXPAND, 10);

    dlg.SetSizerAndFit(sizer);

    int response = dlg.ShowModal();
    if (response == wxID_OK) {
        int diff = radio->GetSelection();
        int localIndex = spin->GetValue() - 1;
        int globalIndex = (diff * perTier) + localIndex;
        panel->LoadGameFromFile(GetDataPath("puzzles.txt"), globalIndex);
    }else if(response == wxID_CANCEL){
        return;
    }else{
        wxLogMessage("Something went wrong, maybe try in spanish");
    }
}

void SudokuFrame::NewCustom(wxCommandEvent& event)
{
    wxDialog dlg(this, wxID_ANY, "Settings");
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxArrayString choices{"Use random pre-existing puzzle", "Generate from scratch"};
    wxRadioBox* radioBox = new wxRadioBox(&dlg, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize,
                                choices, 2, wxRA_SPECIFY_COLS);

    wxSpinCtrl* spinCtrl = new wxSpinCtrl(&dlg, wxID_ANY, wxEmptyString,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxSP_ARROW_KEYS, 20, 60, 20);

    mainSizer->Add(radioBox, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(spinCtrl, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    mainSizer->Add(dlg.CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 10);

    auto* labelAbove = new wxStaticText(&dlg, wxID_ANY, "Choose the generation method of the puzzle");
    mainSizer->Insert(0, labelAbove, 0, wxALL, 10);

    auto* labelBelow = new wxStaticText(&dlg, wxID_ANY, "Choose the amount of clues. Values outside 20-60 are not permitted");
    mainSizer->Insert(3, labelBelow, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    dlg.SetSizerAndFit(mainSizer);

    int response = dlg.ShowModal();
    if (response == wxID_OK) {
        int option = radioBox->GetSelection();
        int number = spinCtrl->GetValue();
        if(number < 20){number = 20;};
        if(number > 60){number = 60;};
        if(option == 0){
            panel->GenerateGameFromFile(number);
        }else{
            panel->GenerateGameFromScratch(number);
        }
    }else if(response == wxID_CANCEL){
        return;
    }else{
        wxLogMessage("Something went wrong, maybe try in spanish");
    }
}

void SudokuFrame::Load(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open a saved game"), "", "",
                    ".txt files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
 
    int response = openFileDialog.ShowModal();
    if (response == wxID_CANCEL){
        return; 
    } else if(response == wxID_OK){
        wxString path = openFileDialog.GetPath();
        panel->LoadGameFromFile(static_cast<std::string>(path), 0);
    } else{
        wxLogMessage("Something went wrong, maybe try in spanish...\n");
    }
}

void SudokuFrame::Save(wxCommandEvent& event){
    wxFileDialog openFileDialog(this, _("Save a game"), "", "",
                    ".txt files (*.txt)|*.txt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    int response = openFileDialog.ShowModal();
    if (response == wxID_CANCEL){
        return; 
    } else if(response == wxID_OK){
        wxString path = openFileDialog.GetPath();
        panel->SaveGameToFile(static_cast<std::string>(path));
    } else{
        wxLogMessage("Something went wrong, maybe try in spanish...\n");
    }
}

void SudokuFrame::OnClose(wxCloseEvent& event) {
    wxMessageDialog dialog(
        this,
        "Are you sure you want to quit?",
        "Exit",
        wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION
    );

    if (dialog.ShowModal() == wxID_YES) {
        Destroy();
    } else {
        if (event.CanVeto()) {
            event.Veto();
        }
    }
}

void SudokuFrame::OnThemeSelect(wxCommandEvent& event) {
    int selectedID = event.GetId();
    for (const auto& theme : themes) {
        if (theme.ID == selectedID) {
            panel->SetTheme(theme);
            break;
        }
    }
    // std::cout << "Selected theme ID: " << selectedID << std::endl; // DEBUG
}