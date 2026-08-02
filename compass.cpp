#include "compass.hpp"
#include "field.hpp"
#include "constants.hpp"
#include <cmath>

CompassField::CompassField(int sw, int sh) {
    rebuild(sw, sh);
}

void CompassField::rebuild(int sw, int sh) {
    screenW = sw; screenH = sh;
    compasses.clear();
    for (int x = COMPASS_SPACING / 2; x < sw; x += COMPASS_SPACING) {
        for (int y = COMPASS_SPACING / 2; y < sh; y += COMPASS_SPACING) {
            compasses.push_back({{(float)x, (float)y}, 0.0f});
        }
    }
}

void CompassField::update(const std::vector<Magnet>& magnets, float dt) {
    if (magnets.empty()) return;
    for (auto& c : compasses) {
        Vector2 b = computeFieldAt(c.pos, magnets);
        float magB = sqrtf(b.x*b.x + b.y*b.y);
        if (magB < 1e-6f) continue;

        float target = atan2f(b.y, b.x);
        float diff = target - c.angle;
        while (diff > PI) diff -= 2*PI;
        while (diff < -PI) diff += 2*PI;
        c.angle += diff * COMPASS_ROT_SPEED;
    }
}

void CompassField::resize(int sw, int sh) {
    rebuild(sw, sh);
}
