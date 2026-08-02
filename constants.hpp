#pragma once

#include <raylib.h>

// ---------------------------------------------------------------------------
// Window
// ---------------------------------------------------------------------------
constexpr int SCREEN_WIDTH  = 1440;
constexpr int SCREEN_HEIGHT = 900;
constexpr int FPS_TARGET    = 60;
constexpr char WINDOW_TITLE[] = "Magnetic Field Simulator";

// ---------------------------------------------------------------------------
// Colors
// ---------------------------------------------------------------------------
constexpr Color CLR_BG             = { 22, 22, 28, 255 };
constexpr Color CLR_GRID           = { 35, 35, 42, 255 };
constexpr Color CLR_NORTH          = { 220, 60, 60, 255 };
constexpr Color CLR_SOUTH          = { 60, 110, 220, 255 };
constexpr Color CLR_MAGNET_BORDER  = { 140, 140, 150, 255 };
constexpr Color CLR_FIELD_LINE     = { 120, 220, 220, 200 };
constexpr Color CLR_ARROW          = { 100, 190, 190, 160 };
constexpr Color CLR_FILING         = { 220, 220, 220, 200 };
constexpr Color CLR_COMPASS_BODY   = { 60, 60, 66, 255 };
constexpr Color CLR_COMPASS_N      = { 50, 200, 50, 255 };
constexpr Color CLR_COMPASS_S      = { 200, 50, 50, 255 };
constexpr Color CLR_SELECT_GLOW    = { 255, 220, 80, 180 };
constexpr Color CLR_UI_BG          = { 28, 28, 34, 240 };
constexpr Color CLR_UI_BORDER      = { 60, 60, 70, 255 };
constexpr Color CLR_UI_TEXT        = { 230, 230, 230, 255 };
constexpr Color CLR_UI_DIM         = { 150, 150, 155, 255 };
constexpr Color CLR_UI_ACCENT      = { 90, 200, 200, 255 };
constexpr Color CLR_PANEL_BG       = { 24, 24, 30, 245 };
constexpr Color CLR_TOOLBAR_BG     = { 20, 20, 26, 255 };
constexpr Color CLR_STATUS_BG      = { 18, 18, 24, 255 };

// ---------------------------------------------------------------------------
// Magnet
// ---------------------------------------------------------------------------
constexpr float MAGNET_LENGTH      = 100.0f;
constexpr float MAGNET_WIDTH       = 28.0f;
constexpr float DEFAULT_STRENGTH   = 22000.0f;
constexpr float MIN_STRENGTH       = 2000.0f;
constexpr float MAX_STRENGTH       = 60000.0f;
constexpr float STRENGTH_STEP      = 1500.0f;
constexpr float MASS               = 1.0f;

// ---------------------------------------------------------------------------
// Physics
// ---------------------------------------------------------------------------
constexpr float MAX_FORCE          = 6000.0f;
constexpr float MIN_DISTANCE       = 8.0f;
constexpr float LINEAR_DAMPING     = 0.90f;
constexpr float ANGULAR_DAMPING  = 0.85f;

// ---------------------------------------------------------------------------
// Field lines
// ---------------------------------------------------------------------------
constexpr int   FIELD_LINES_PER_MAGNET = 16;
constexpr int   FIELD_MAX_STEPS        = 300;
constexpr float FIELD_STEP_SIZE        = 5.0f;
constexpr float FIELD_MIN_STRENGTH     = 0.4f;

// ---------------------------------------------------------------------------
// Arrow grid
// ---------------------------------------------------------------------------
constexpr int   ARROW_SPACING    = 40;
constexpr float ARROW_MAX_LEN    = 20.0f;
constexpr float ARROW_MIN_LEN    = 4.0f;
constexpr float ARROW_REF_FIELD  = 40.0f;

// ---------------------------------------------------------------------------
// Iron filings
// ---------------------------------------------------------------------------
constexpr int   MAX_FILINGS       = 2000;
constexpr float FILING_LENGTH     = 5.0f;
constexpr float FILING_ROT_SPEED  = 0.35f;
constexpr float FILING_DRIFT      = 5.0f;
constexpr float FILING_SETTLE     = 0.9f;

// ---------------------------------------------------------------------------
// Compasses
// ---------------------------------------------------------------------------
constexpr int   COMPASS_SPACING   = 90;
constexpr float COMPASS_RADIUS    = 11.0f;
constexpr float COMPASS_ROT_SPEED = 0.18f;

// ---------------------------------------------------------------------------
// UI Layout
// ---------------------------------------------------------------------------
constexpr int   TOOLBAR_HEIGHT    = 48;
constexpr int   STATUSBAR_HEIGHT  = 32;
constexpr int   PANEL_WIDTH       = 260;
constexpr int   PANEL_MARGIN      = 10;
