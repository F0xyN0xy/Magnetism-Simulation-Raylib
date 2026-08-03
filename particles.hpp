#pragma once

#include <raylib.h>
#include "constants.hpp"
#include <vector>
#include "magnet.hpp"

struct Filing {
    Vector2 pos;
    float angle;
};

struct IronFilings {
    std::vector<Filing> filings;
    int screenW, screenH;

    IronFilings(int sw, int sh, int count = MAX_FILINGS);
    void reset(int sw, int sh, int count);
    void update(const std::vector<Magnet>& magnets, float dt);
    void resize(int sw, int sh);
};
