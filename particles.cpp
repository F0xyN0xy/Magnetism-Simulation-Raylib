#include "particles.hpp"
#include "field.hpp"
#include "constants.hpp"
#include <cmath>
#include <cstdlib>

IronFilings::IronFilings(int sw, int sh, int count) {
    screenW = sw; screenH = sh;
    reset(sw, sh, count);
}

void IronFilings::reset(int sw, int sh, int count) {
    screenW = sw; screenH = sh;
    filings.clear();
    filings.reserve(count);
    for (int i = 0; i < count; ++i) {
        Filing f;
        f.pos.x = (float)(rand() % sw);
        f.pos.y = (float)(rand() % sh);
        f.angle = (float)(rand() % 360) * DEG2RAD;
        filings.push_back(f);
    }
}

void IronFilings::update(const std::vector<Magnet>& magnets, float dt) {
    if (magnets.empty()) return;
    for (auto& f : filings) {
        Vector2 b = computeFieldAt(f.pos, magnets);
        float magB = sqrtf(b.x*b.x + b.y*b.y);
        if (magB < 1e-6f) continue;

        float target = atan2f(b.y, b.x);
        float diff = target - f.angle;
        while (diff > PI) diff -= 2*PI;
        while (diff < -PI) diff += 2*PI;
        f.angle += diff * FILING_ROT_SPEED;

        if (magB > FILING_SETTLE) {
            float drift = fminf(1.0f, magB / ARROW_REF_FIELD) * FILING_DRIFT * dt;
            f.pos.x += cosf(f.angle) * drift * 0.15f;
            f.pos.y += sinf(f.angle) * drift * 0.15f;
            if (f.pos.x < 0) f.pos.x = 0;
            if (f.pos.x > screenW) f.pos.x = screenW;
            if (f.pos.y < 0) f.pos.y = 0;
            if (f.pos.y > screenH) f.pos.y = screenH;
        }
    }
}

void IronFilings::resize(int sw, int sh) {
    screenW = sw; screenH = sh;
}
