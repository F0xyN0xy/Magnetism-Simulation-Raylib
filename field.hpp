#pragma once

#include <raylib.h>
#include <vector>
#include "magnet.hpp"

struct FieldLine {
    std::vector<Vector2> points;
};

struct ArrowVec {
    Vector2 pos;
    Vector2 dir;
    float magnitude;
};

Vector2 computeFieldAt(Vector2 p, const std::vector<Magnet>& magnets);
std::vector<FieldLine> traceFieldLines(const std::vector<Magnet>& magnets, int sw, int sh);
std::vector<ArrowVec> generateArrowGrid(const std::vector<Magnet>& magnets, int sw, int sh);
