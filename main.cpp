#include <raylib.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "constants.hpp"
#include "magnet.hpp"
#include "physics.hpp"
#include "field.hpp"
#include "particles.hpp"
#include "compass.hpp"
#include "renderer.hpp"
#include "ui.hpp"

int main() {
    srand((unsigned)time(nullptr));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS_TARGET);

    setupGuiStyle();
    Font font = GetFontDefault();

    int sw = SCREEN_WIDTH;
    int sh = SCREEN_HEIGHT;

    std::vector<Magnet> magnets;
    Magnet* selected = nullptr;
    AppState state;

    // Default scene
    magnets.push_back(Magnet(sw * 0.38f, sh * 0.5f, 0.0f));
    magnets.push_back(Magnet(sw * 0.62f, sh * 0.5f, PI));

    IronFilings filings(sw, sh, state.filingCount);
    CompassField compasses(sw, sh);

    std::vector<FieldLine> cachedLines;
    std::vector<ArrowVec> cachedArrows;

    bool dragging = false;
    bool rotating = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;
        dt *= state.timeScale;

        // Handle resize
        if (IsWindowResized()) {
            sw = GetScreenWidth();
            sh = GetScreenHeight();
            filings.resize(sw, sh);
            compasses.resize(sw, sh);
        }

        // --- Input ---
        Vector2 mouse = GetMousePosition();
        UIRects layout = computeLayout(sw, sh);

        // Only interact with canvas if mouse is not over UI
        bool overUI = CheckCollisionPointRec(mouse, layout.toolbar) ||
                      CheckCollisionPointRec(mouse, layout.panel) ||
                      CheckCollisionPointRec(mouse, layout.statusbar);

        if (!overUI) {
            // Left click: select / start drag
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                bool hit = false;
                for (auto it = magnets.rbegin(); it != magnets.rend(); ++it) {
                    if (it->containsPoint(mouse)) {
                        if (selected) selected->selected = false;
                        selected = &(*it);
                        selected->selected = true;
                        selected->startDrag(mouse);
                        dragging = true;
                        hit = true;
                        break;
                    }
                }
                if (!hit) {
                    if (selected) selected->selected = false;
                    selected = nullptr;
                }
            }

            // Right click: rotate
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                for (auto it = magnets.rbegin(); it != magnets.rend(); ++it) {
                    if (it->containsPoint(mouse)) {
                        if (selected) selected->selected = false;
                        selected = &(*it);
                        selected->selected = true;
                        selected->rotateToward(mouse);
                        rotating = true;
                        break;
                    }
                }
            }

            // Scroll: adjust strength
            float wheel = GetMouseWheelMove();
            if (wheel != 0.0f && selected) {
                selected->adjustStrength(wheel * STRENGTH_STEP);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
            for (auto& m : magnets) m.stopDrag();
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            rotating = false;
        }

        if (dragging && selected) {
            selected->dragTo(mouse);
        }
        if (rotating && selected) {
            selected->rotateToward(mouse);
        }

        // Keyboard shortcuts
        if (IsKeyPressed(KEY_SPACE)) state.paused = !state.paused;
        if (IsKeyPressed(KEY_R)) {
            magnets.clear();
            magnets.push_back(Magnet(layout.canvas.width * 0.38f, layout.canvas.height * 0.5f + TOOLBAR_HEIGHT, 0.0f));
            magnets.push_back(Magnet(layout.canvas.width * 0.62f, layout.canvas.height * 0.5f + TOOLBAR_HEIGHT, PI));
            selected = nullptr;
            filings.reset(sw, sh, state.filingCount);
        }
        if (IsKeyPressed(KEY_N)) {
            float offset = 40.0f * (magnets.size() % 5);
            magnets.push_back(Magnet(layout.canvas.width * 0.5f + offset, layout.canvas.height * 0.5f + TOOLBAR_HEIGHT + offset));
            if (selected) selected->selected = false;
            selected = &magnets.back();
            selected->selected = true;
        }
        if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
            if (selected) {
                for (auto it = magnets.begin(); it != magnets.end(); ++it) {
                    if (&(*it) == selected) {
                        magnets.erase(it);
                        selected = nullptr;
                        break;
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_F)) state.showArrows = !state.showArrows;
        if (IsKeyPressed(KEY_I)) {
            state.showFilings = !state.showFilings;
            if (state.showFilings) filings.reset(sw, sh, state.filingCount);
        }
        if (IsKeyPressed(KEY_C)) state.showCompasses = !state.showCompasses;
        if (IsKeyPressed(KEY_H)) state.showHelp = !state.showHelp;

        // --- Update ---
        for (auto& m : magnets) {
            m.clampToBounds(sw, sh);
        }

        if (!state.paused) {
            computeAndApplyForces(magnets, dt);
        }

        if (state.showFilings) {
            filings.update(magnets, dt);
        }
        if (state.showCompasses) {
            compasses.update(magnets, dt);
        }

        // Recompute field lines every frame for real-time feel
        cachedLines = traceFieldLines(magnets, sw, sh);
        if (state.showArrows) {
            cachedArrows = generateArrowGrid(magnets, sw, sh);
        }

        // --- Draw ---
        BeginDrawing();

        drawBackground(sw, sh);

        // Clip to canvas area (don't draw under UI panels)
        // Actually, let field lines extend under the panel for visual continuity
        // but draw magnets on top properly

        if (state.showFieldLines) drawFieldLines(cachedLines);
        if (state.showArrows) drawArrowGrid(cachedArrows);
        if (state.showFilings) drawFilings(filings);
        if (state.showCompasses) drawCompasses(compasses);

        for (const auto& m : magnets) {
            drawMagnet(m, font);
        }

        // UI layers
        drawToolbar(layout, state, magnets, selected);
        drawSidePanel(layout, state, selected);
        drawStatusBar(layout, GetFPS(), (int)magnets.size(), selected);

        if (state.showHelp) {
            drawHelpOverlay(sw, sh);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
