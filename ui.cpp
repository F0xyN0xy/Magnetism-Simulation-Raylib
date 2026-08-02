#include "ui.hpp"
#include "constants.hpp"
#include <cmath>
#include <cstdio>

// raygui is header-only; define implementation in ONE .cpp file
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

UIRects computeLayout(int sw, int sh) {
    UIRects r;
    r.toolbar = { 0, 0, (float)sw, (float)TOOLBAR_HEIGHT };
    r.statusbar = { 0, (float)(sh - STATUSBAR_HEIGHT), (float)sw, (float)STATUSBAR_HEIGHT };
    r.panel = { (float)(sw - PANEL_WIDTH - PANEL_MARGIN), (float)(TOOLBAR_HEIGHT + PANEL_MARGIN),
                (float)PANEL_WIDTH, (float)(sh - TOOLBAR_HEIGHT - STATUSBAR_HEIGHT - PANEL_MARGIN * 2) };
    r.canvas = { 0, (float)TOOLBAR_HEIGHT, (float)(sw - PANEL_WIDTH - PANEL_MARGIN * 2), (float)(sh - TOOLBAR_HEIGHT - STATUSBAR_HEIGHT) };
    return r;
}

void setupGuiStyle() {
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(CLR_PANEL_BG));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt({45, 45, 52, 255}));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt({60, 60, 70, 255}));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt({80, 80, 95, 255}));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(CLR_UI_BORDER));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(CLR_UI_TEXT));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(CLR_UI_ACCENT));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 14);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({55, 55, 65, 255}));
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, ColorToInt({40, 40, 50, 255}));
    GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, ColorToInt({70, 70, 85, 255}));
    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, ColorToInt({50, 50, 60, 255}));
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, ColorToInt(CLR_UI_TEXT));
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt({45, 45, 55, 255}));
}

void drawToolbar(UIRects& r, AppState& state, std::vector<Magnet>& magnets, Magnet*& selected) {
    DrawRectangleRec(r.toolbar, CLR_TOOLBAR_BG);
    DrawLine(0, TOOLBAR_HEIGHT, r.toolbar.width, TOOLBAR_HEIGHT, CLR_UI_BORDER);

    float x = 12;
    float y = 8;
    float btnW = 80;
    float btnH = 32;

    // Play/Pause
    const char* playLabel = state.paused ? "#131# Play" : "#132# Pause";
    if (GuiButton({x, y, btnW, btnH}, playLabel)) {
        state.paused = !state.paused;
    }
    x += btnW + 8;

    // Reset
    if (GuiButton({x, y, btnW, btnH}, "#211# Reset")) {
        magnets.clear();
        magnets.push_back(Magnet(r.canvas.width * 0.38f, r.canvas.height * 0.5f + TOOLBAR_HEIGHT, 0.0f));
        magnets.push_back(Magnet(r.canvas.width * 0.62f, r.canvas.height * 0.5f + TOOLBAR_HEIGHT, PI));
        selected = nullptr;
    }
    x += btnW + 8;

    // Add Magnet
    if (GuiButton({x, y, btnW + 10, btnH}, "#142# Add Magnet")) {
        float offset = 40.0f * (magnets.size() % 5);
        magnets.push_back(Magnet(r.canvas.width * 0.5f + offset, r.canvas.height * 0.5f + TOOLBAR_HEIGHT + offset));
        selected = &magnets.back();
        selected->selected = true;
    }
    x += btnW + 18;

    // Remove selected
    bool canDelete = (selected != nullptr);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, canDelete ? ColorToInt(CLR_UI_TEXT) : ColorToInt({100,100,100,255}));
    if (GuiButton({x, y, btnW + 20, btnH}, "#143# Remove") && canDelete) {
        for (auto it = magnets.begin(); it != magnets.end(); ++it) {
            if (&(*it) == selected) {
                magnets.erase(it);
                selected = nullptr;
                break;
            }
        }
    }
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(CLR_UI_TEXT));
    x += btnW + 30;

    // Title
    DrawText("Magnetic Field Simulator", (int)x, (int)y + 6, 18, CLR_UI_ACCENT);

    // Help toggle (right side)
    float helpX = r.toolbar.width - 100;
    if (GuiButton({helpX, y, 90, btnH}, state.showHelp ? "#191# Help" : "#190# Help")) {
        state.showHelp = !state.showHelp;
    }
}

void drawSidePanel(UIRects& r, AppState& state, Magnet* selected) {
    // Panel background
    DrawRectangleRec(r.panel, CLR_PANEL_BG);
    DrawRectangleLinesEx(r.panel, 1, CLR_UI_BORDER);

    float x = r.panel.x + 12;
    float y = r.panel.y + 12;
    float w = r.panel.width - 24;
    float h = 24;

    // --- Visualization Section ---
    DrawText("VISUALIZATION", (int)x, (int)y, 14, CLR_UI_ACCENT);
    y += 22;

    GuiCheckBox({x, y, 16, 16}, "Field Lines", &state.showFieldLines);
    y += 26;
    GuiCheckBox({x, y, 16, 16}, "Vector Arrows", &state.showArrows);
    y += 26;
    GuiCheckBox({x, y, 16, 16}, "Iron Filings", &state.showFilings);
    y += 26;
    GuiCheckBox({x, y, 16, 16}, "Compasses", &state.showCompasses);
    y += 36;

    // --- Physics Section ---
    DrawText("PHYSICS", (int)x, (int)y, 14, CLR_UI_ACCENT);
    y += 22;

    GuiSlider({x, y, w, h}, "Weak", "Strong", &state.strengthSlider, 0.0f, 1.0f);
    state.magnetStrength = MIN_STRENGTH + state.strengthSlider * (MAX_STRENGTH - MIN_STRENGTH);
    DrawText(TextFormat("Strength: %.0f", state.magnetStrength), (int)x, (int)(y + h + 2), 12, CLR_UI_DIM);
    y += 44;

    GuiSlider({x, y, w, h}, "Low", "High", &state.dampingSlider, 0.0f, 1.0f);
    state.linearDamping = 0.80f + state.dampingSlider * 0.18f;
    DrawText(TextFormat("Damping: %.2f", state.linearDamping), (int)x, (int)(y + h + 2), 12, CLR_UI_DIM);
    y += 44;

    GuiSlider({x, y, w, h}, "Slow", "Fast", &state.simSpeedSlider, 0.1f, 2.0f);
    state.timeScale = state.simSpeedSlider;
    DrawText(TextFormat("Speed: %.1fx", state.timeScale), (int)x, (int)(y + h + 2), 12, CLR_UI_DIM);
    y += 44;

    // --- Particles Section ---
    DrawText("PARTICLES", (int)x, (int)y, 14, CLR_UI_ACCENT);
    y += 22;

    GuiSlider({x, y, w, h}, "Few", "Many", &state.filingCountSlider, 0.0f, 1.0f);
    state.filingCount = (int)(200 + state.filingCountSlider * 2800);
    DrawText(TextFormat("Count: %d", state.filingCount), (int)x, (int)(y + h + 2), 12, CLR_UI_DIM);
    y += 44;

    // --- Selected Magnet ---
    if (selected) {
        DrawLine(x, y, x + w, y, CLR_UI_BORDER);
        y += 14;
        DrawText("SELECTED MAGNET", (int)x, (int)y, 14, CLR_UI_ACCENT);
        y += 22;

        float angleDeg = fmodf(selected->angle * RAD2DEG + 360.0f, 360.0f);
        DrawText(TextFormat("Strength: %.0f", selected->strength), (int)x, (int)y, 13, CLR_UI_TEXT);
        y += 18;
        DrawText(TextFormat("Angle: %.1f deg", angleDeg), (int)x, (int)y, 13, CLR_UI_TEXT);
        y += 18;
        DrawText(TextFormat("Velocity: %.1f", sqrtf(selected->vel.x*selected->vel.x + selected->vel.y*selected->vel.y)),
                 (int)x, (int)y, 13, CLR_UI_TEXT);
        y += 26;

        if (GuiButton({x, y, w, 28}, "Apply Global Strength")) {
            selected->strength = state.magnetStrength;
        }
    }
}

void drawStatusBar(UIRects& r, int fps, int magnetCount, Magnet* selected) {
    DrawRectangleRec(r.statusbar, CLR_STATUS_BG);
    DrawLine(0, (int)r.statusbar.y, (int)r.statusbar.width, (int)r.statusbar.y, CLR_UI_BORDER);

    int y = (int)r.statusbar.y + 8;
    DrawText(TextFormat("FPS: %d", fps), 12, y, 14, CLR_UI_TEXT);
    DrawText(TextFormat("Magnets: %d", magnetCount), 100, y, 14, CLR_UI_TEXT);

    if (selected) {
        DrawText(TextFormat("Selected: Strength %.0f | Angle %.0f deg",
            selected->strength, fmodf(selected->angle * RAD2DEG + 360.0f, 360.0f)),
            220, y, 14, CLR_UI_ACCENT);
    } else {
        DrawText("Click a magnet to select", 220, y, 14, CLR_UI_DIM);
    }

    // Active modes indicator
    const char* modes = "";
    // We'll skip complex mode string building for simplicity
    DrawText("Left-drag: move  |  Right-click: rotate  |  Scroll: strength", (int)r.statusbar.width - 420, y, 12, CLR_UI_DIM);
}

void drawHelpOverlay(int sw, int sh) {
    const char* lines[] = {
        "CONTROLS",
        "",
        "Left Click + Drag     Move selected magnet",
        "Right Click           Rotate magnet toward cursor",
        "Mouse Wheel           Change magnet strength",
        "",
        "Space                 Pause / Resume",
        "R                     Reset scene",
        "N                     Add new magnet",
        "Delete                Remove selected magnet",
        "F                     Toggle field arrows",
        "I                     Toggle iron filings",
        "C                     Toggle compasses",
        "H                     Toggle help",
        "",
        "Use the right panel for sliders and toggles."
    };
    int lineCount = sizeof(lines) / sizeof(lines[0]);
    int lineH = 18;
    int pad = 20;
    int panelW = 380;
    int panelH = lineCount * lineH + pad * 2 + 10;

    int px = (sw - panelW) / 2;
    int py = (sh - panelH) / 2;

    DrawRectangle(px - 4, py - 4, panelW + 8, panelH + 8, {0, 0, 0, 180});
    DrawRectangle(px, py, panelW, panelH, CLR_PANEL_BG);
    DrawRectangleLines(px, py, panelW, panelH, CLR_UI_ACCENT);

    for (int i = 0; i < lineCount; ++i) {
        Color c = (i == 0) ? CLR_UI_ACCENT : CLR_UI_TEXT;
        int sz = (i == 0) ? 16 : 13;
        DrawText(lines[i], px + pad, py + pad + i * lineH, sz, c);
    }
}
