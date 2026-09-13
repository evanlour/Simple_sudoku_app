#pragma once
#include <wx/colour.h>

enum ThemeID{
    THEME_LIGHT = 20000,
    THEME_DARK
};

struct Theme {
    ThemeID ID;
    wxString name;
    wxColour background;
    wxColour Line;
    wxColour cellSelected;
    wxColour textPrimaryDefault;
    wxColour textSecondaryDefault;
    wxColour textLocked;

    static Theme Light() {
        return {
            THEME_LIGHT,
            "Light",
            wxColour(200, 200, 200),
            wxColour(30, 30, 30),
            wxColour(187, 222, 251),
            wxColour(20, 20, 20),
            wxColour(255, 0, 0),
            wxColour(127, 127, 127)
        };
    }

    static Theme Dark() {
        return {
            THEME_DARK,
            "Dark",
            wxColour(30, 30, 30),
            wxColour(220, 220, 220),
            wxColour(50, 75, 110),
            wxColour(240, 240, 240),
            wxColour(255, 0, 0),
            wxColour(127, 127, 127)
        };
    }

    static std::vector<Theme> getAvailableThemes(){
        return {Light(), Dark()};
    }
};