#pragma once

#include <raylib.h>
#include <vector>
#include "magnet.hpp"
#include "field.hpp"
#include "particles.hpp"
#include "compass.hpp"

void drawBackground(int sw, int sh);
void drawFieldLines(const std::vector<FieldLine>& lines);
void drawArrowGrid(const std::vector<ArrowVec>& arrows);
void drawFilings(const IronFilings& filings);
void drawCompasses(const CompassField& compasses);
void drawMagnet(const Magnet& mag, Font font);
