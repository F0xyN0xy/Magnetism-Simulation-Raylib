#include "field.hpp"
#include "constants.hpp"
#include <cmath>

Vector2 computeFieldAt(Vector2 p, const std::vector<Magnet>& magnets) {
    Vector2 b = {0, 0};
    for (const auto& mag : magnets) {
        std::vector<std::pair<Vector2, float>> poles;
        mag.getPoles(poles);
        for (auto& [polePos, charge] : poles) {
            float dx = p.x - polePos.x;
            float dy = p.y - polePos.y;
            float distSq = dx*dx + dy*dy;
            float dist = sqrtf(distSq);
            if (dist < 1.0f) dist = 1.0f;
            float factor = charge / (distSq * dist);
            b.x += factor * dx;
            b.y += factor * dy;
        }
    }
    return b;
}

std::vector<FieldLine> traceFieldLines(const std::vector<Magnet>& magnets, int sw, int sh) {
    std::vector<FieldLine> lines;
    if (magnets.empty()) return lines;

    // Pre-collect all south poles for termination check
    std::vector<Vector2> southPoles;
    std::vector<float> southRadii;
    for (const auto& mag : magnets) {
        southPoles.push_back(mag.southPos());
        southRadii.push_back(mag.width * 0.7f);
    }

    for (const auto& mag : magnets) {
        Vector2 n = mag.northPos();
        Vector2 d = mag.direction();
        float baseAngle = atan2f(d.y, d.x);

        for (int i = 0; i < FIELD_LINES_PER_MAGNET; ++i) {
            float t = (FIELD_LINES_PER_MAGNET > 1)
                ? (float)i / (FIELD_LINES_PER_MAGNET - 1) - 0.5f
                : 0.0f;
            float seedAngle = baseAngle + t * PI * 0.9f;
            float sx = n.x + cosf(seedAngle) * (mag.width * 0.6f);
            float sy = n.y + sinf(seedAngle) * (mag.width * 0.6f);

            FieldLine line;
            line.points.push_back({sx, sy});

            float x = sx, y = sy;
            bool active = true;

            for (int step = 0; step < FIELD_MAX_STEPS && active; ++step) {
                Vector2 b = computeFieldAt({x, y}, magnets);
                float magB = sqrtf(b.x*b.x + b.y*b.y);
                if (magB < FIELD_MIN_STRENGTH) break;

                float stepX = (b.x / magB) * FIELD_STEP_SIZE;
                float stepY = (b.y / magB) * FIELD_STEP_SIZE;
                x += stepX;
                y += stepY;
                line.points.push_back({x, y});

                // Check if we reached any south pole
                for (size_t s = 0; s < southPoles.size(); ++s) {
                    float ddx = x - southPoles[s].x;
                    float ddy = y - southPoles[s].y;
                    if (ddx*ddx + ddy*ddy < southRadii[s]*southRadii[s]) {
                        active = false;
                        break;
                    }
                }

                if (x < -50 || x > sw + 50 || y < -50 || y > sh + 50) break;
            }

            if (line.points.size() > 6) {
                lines.push_back(std::move(line));
            }
        }
    }
    return lines;
}

std::vector<ArrowVec> generateArrowGrid(const std::vector<Magnet>& magnets, int sw, int sh) {
    std::vector<ArrowVec> arrows;
    for (int gx = ARROW_SPACING / 2; gx < sw; gx += ARROW_SPACING) {
        for (int gy = ARROW_SPACING / 2; gy < sh; gy += ARROW_SPACING) {
            Vector2 b = computeFieldAt({(float)gx, (float)gy}, magnets);
            float magB = sqrtf(b.x*b.x + b.y*b.y);
            if (magB > 0.5f) {
                float len = (magB / ARROW_REF_FIELD) * ARROW_MAX_LEN;
                if (len < ARROW_MIN_LEN) len = ARROW_MIN_LEN;
                if (len > ARROW_MAX_LEN) len = ARROW_MAX_LEN;
                arrows.push_back({
                    {(float)gx, (float)gy},
                    {b.x / magB, b.y / magB},
                    magB
                });
            }
        }
    }
    return arrows;
}
