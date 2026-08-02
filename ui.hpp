#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include "magnet.hpp"

struct AppState {
    bool showFieldLines = true;
    bool showArrows = false;
    bool showFilings = false;
    bool showCompasses = false;
    bool paused = false;
    bool showHelp = true;

    // Sliders (0-1 normalized, mapped to actual ranges)
    float strengthSlider = 0.5f;
    float dampingSlider = 0.5f;
    float filingCountSlider = 0.75f;
    float simSpeedSlider = 1.0f;

    // Derived values
    float magnetStrength = 22000.0f;
    float linearDamping = 0.90f;
    int filingCount = 1500;
    float timeScale = 1.0f;
};

struct UIRects {
    Rectangle toolbar;
    Rectangle statusbar;
    Rectangle panel;
    Rectangle canvas;
};

UIRects computeLayout(int sw, int sh);
void drawToolbar(UIRects& r, AppState& state, std::vector<Magnet>& magnets, Magnet*& selected);
void drawSidePanel(UIRects& r, AppState& state, Magnet* selected);
void drawStatusBar(UIRects& r, int fps, int magnetCount, Magnet* selected);
void drawHelpOverlay(int sw, int sh);
void setupGuiStyle();
