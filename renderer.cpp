#include "renderer.hpp"
#include "constants.hpp"
#include <cmath>

void drawBackground(int sw, int sh) {
    ClearBackground(CLR_BG);
    // Subtle grid
    for (int x = 0; x < sw; x += 80) {
        DrawLine(x, 0, x, sh, CLR_GRID);
    }
    for (int y = 0; y < sh; y += 80) {
        DrawLine(0, y, sw, y, CLR_GRID);
    }
}

void drawFieldLines(const std::vector<FieldLine>& lines) {
    for (const auto& line : lines) {
        if (line.points.size() < 2) continue;
        for (size_t i = 0; i < line.points.size() - 1; ++i) {
            // Fade toward tail
            float alpha = 1.0f - (float)i / (float)line.points.size();
            Color c = CLR_FIELD_LINE;
            c.a = (unsigned char)(c.a * alpha * 0.6f + 40);
            DrawLineV(line.points[i], line.points[i+1], c);
        }
        // Arrowhead at midpoint
        size_t mid = line.points.size() / 2;
        if (mid > 0 && mid < line.points.size()) {
            Vector2 p0 = line.points[mid - 1];
            Vector2 p1 = line.points[mid];
            float angle = atan2f(p1.y - p0.y, p1.x - p0.x);
            Vector2 left = {
                p1.x - 6.0f * cosf(angle - 0.5f),
                p1.y - 6.0f * sinf(angle - 0.5f)
            };
            Vector2 right = {
                p1.x - 6.0f * cosf(angle + 0.5f),
                p1.y - 6.0f * sinf(angle + 0.5f)
            };
            DrawTriangle(p1, left, right, CLR_FIELD_LINE);
        }
    }
}

void drawArrowGrid(const std::vector<ArrowVec>& arrows) {
    for (const auto& a : arrows) {
        Vector2 end = {
            a.pos.x + a.dir.x * ARROW_MAX_LEN,
            a.pos.y + a.dir.y * ARROW_MAX_LEN
        };
        DrawLineV(a.pos, end, CLR_ARROW);
        float angle = atan2f(a.dir.y, a.dir.x);
        Vector2 left = {
            end.x - 4.0f * cosf(angle - 0.5f),
            end.y - 4.0f * sinf(angle - 0.5f)
        };
        Vector2 right = {
            end.x - 4.0f * cosf(angle + 0.5f),
            end.y - 4.0f * sinf(angle + 0.5f)
        };
        DrawTriangle(end, left, right, CLR_ARROW);
    }
}

void drawFilings(const IronFilings& filings) {
    float half = FILING_LENGTH;
    for (const auto& f : filings.filings) {
        Vector2 d = { cosf(f.angle) * half, sinf(f.angle) * half };
        DrawLineV(
            { f.pos.x - d.x, f.pos.y - d.y },
            { f.pos.x + d.x, f.pos.y + d.y },
            CLR_FILING
        );
    }
}

void drawCompasses(const CompassField& compasses) {
    for (const auto& c : compasses.compasses) {
        DrawCircleV(c.pos, COMPASS_RADIUS, CLR_COMPASS_BODY);
        DrawCircleLinesV(c.pos, COMPASS_RADIUS, {110, 110, 118, 255});
        Vector2 n = {
            c.pos.x + cosf(c.angle) * COMPASS_RADIUS * 0.85f,
            c.pos.y + sinf(c.angle) * COMPASS_RADIUS * 0.85f
        };
        Vector2 s = {
            c.pos.x - cosf(c.angle) * COMPASS_RADIUS * 0.85f,
            c.pos.y - sinf(c.angle) * COMPASS_RADIUS * 0.85f
        };
        DrawLineV(s, n, CLR_COMPASS_N);
        DrawCircleV(n, 2.0f, CLR_COMPASS_N);
        DrawCircleV(s, 2.0f, CLR_COMPASS_S);
    }
}

void drawMagnet(const Magnet& mag, Font font) {
    float hw = mag.length * 0.5f;
    float hh = mag.width * 0.5f;
    float ca = cosf(mag.angle);
    float sa = sinf(mag.angle);

    auto rot = [&](float lx, float ly) -> Vector2 {
        return {
            mag.pos.x + lx * ca - ly * sa,
            mag.pos.y + lx * sa + ly * ca
        };
    };

    // Selection glow
    if (mag.selected) {
        Vector2 glow[4] = {
            rot(-hw - 4, -hh - 4), rot(hw + 4, -hh - 4),
            rot(hw + 4, hh + 4), rot(-hw - 4, hh + 4)
        };
        DrawTriangleFan(glow, 4, CLR_SELECT_GLOW);
    }

    // North half (red)
    Vector2 north[4] = {
        rot(0, -hh), rot(hw, -hh),
        rot(hw, hh), rot(0, hh)
    };
    DrawTriangleFan(north, 4, CLR_NORTH);

    // South half (blue)
    Vector2 south[4] = {
        rot(-hw, -hh), rot(0, -hh),
        rot(0, hh), rot(-hw, hh)
    };
    DrawTriangleFan(south, 4, CLR_SOUTH);

    // Border
    Vector2 border[4] = {
        rot(-hw, -hh), rot(hw, -hh),
        rot(hw, hh), rot(-hw, hh)
    };
    Color borderClr = mag.selected ? CLR_SELECT_GLOW : CLR_MAGNET_BORDER;
    int borderThick = mag.selected ? 3 : 2;
    for (int i = 0; i < 4; ++i) {
        DrawLineV(border[i], border[(i+1)%4], borderClr);
    }

    // Labels
    if (font.baseSize > 0) {
        Vector2 nPos = rot(hw * 0.55f, 0);
        Vector2 sPos = rot(-hw * 0.55f, 0);
        DrawTextEx(font, "N", {nPos.x - 6, nPos.y - 8}, 16, 1, WHITE);
        DrawTextEx(font, "S", {sPos.x - 6, sPos.y - 8}, 16, 1, WHITE);
    } else {
        Vector2 nPos = rot(hw * 0.55f, 0);
        Vector2 sPos = rot(-hw * 0.55f, 0);
        DrawText("N", (int)nPos.x - 6, (int)nPos.y - 8, 16, WHITE);
        DrawText("S", (int)sPos.x - 6, (int)sPos.y - 8, 16, WHITE);
    }
}
