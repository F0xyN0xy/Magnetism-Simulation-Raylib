#pragma once

#include <raylib.h>
#include <vector>
#include "magnet.hpp"

struct Compass {
    Vector2 pos;
    float angle;
};

struct CompassField {
    std::vector<Compass> compasses;
    int screenW, screenH;

    CompassField(int sw, int sh);
    void rebuild(int sw, int sh);
    void update(const std::vector<Magnet>& magnets, float dt);
    void resize(int sw, int sh);
};
