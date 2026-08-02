#include "magnet.hpp"
#include "constants.hpp"
#include <cmath>

Magnet::Magnet(float x, float y, float a, float s, float l, float w)
    : pos{x, y}, angle(a), strength(s), length(l), width(w),
      vel{0,0}, angularVel(0), selected(false), dragging(false), dragOffset{0,0} {}

Vector2 Magnet::direction() const {
    return { cosf(angle), sinf(angle) };
}

Vector2 Magnet::northPos() const {
    Vector2 d = direction();
    float half = length * 0.5f;
    return { pos.x + d.x * half, pos.y + d.y * half };
}

Vector2 Magnet::southPos() const {
    Vector2 d = direction();
    float half = length * 0.5f;
    return { pos.x - d.x * half, pos.y - d.y * half };
}

void Magnet::getPoles(std::vector<std::pair<Vector2, float>>& out) const {
    out.push_back({northPos(), +strength});
    out.push_back({southPos(), -strength});
}

bool Magnet::containsPoint(Vector2 p) const {
    float dx = p.x - pos.x;
    float dy = p.y - pos.y;
    float ca = cosf(-angle), sa = sinf(-angle);
    float lx = dx * ca - dy * sa;
    float ly = dx * sa + dy * ca;
    return (fabsf(lx) <= length * 0.5f + 4.0f) && (fabsf(ly) <= width * 0.5f + 4.0f);
}

void Magnet::startDrag(Vector2 mouse) {
    dragging = true;
    dragOffset = { pos.x - mouse.x, pos.y - mouse.y };
    vel = {0, 0};
    angularVel = 0;
}

void Magnet::dragTo(Vector2 mouse) {
    if (!dragging) return;
    float targetX = mouse.x + dragOffset.x;
    float targetY = mouse.y + dragOffset.y;
    pos.x += (targetX - pos.x) * 0.45f;
    pos.y += (targetY - pos.y) * 0.45f;
}

void Magnet::stopDrag() {
    dragging = false;
}

void Magnet::rotateToward(Vector2 target) {
    angle = atan2f(target.y - pos.y, target.x - pos.x);
    angularVel = 0;
}

void Magnet::adjustStrength(float delta) {
    strength += delta;
    if (strength < MIN_STRENGTH) strength = MIN_STRENGTH;
    if (strength > MAX_STRENGTH) strength = MAX_STRENGTH;
}

void Magnet::clampToBounds(int sw, int sh) {
    float margin = length * 0.5f;
    if (pos.x < margin) { pos.x = margin; vel.x = fabsf(vel.x) * 0.3f; }
    if (pos.x > sw - margin) { pos.x = sw - margin; vel.x = -fabsf(vel.x) * 0.3f; }
    if (pos.y < margin) { pos.y = margin; vel.y = fabsf(vel.y) * 0.3f; }
    if (pos.y > sh - margin) { pos.y = sh - margin; vel.y = -fabsf(vel.y) * 0.3f; }
}

void Magnet::update() {
    if (dragging) {
        vel = {0, 0};
        angularVel = 0;
        return;
    }
    pos.x += vel.x;
    pos.y += vel.y;
    angle += angularVel;
    vel.x *= LINEAR_DAMPING;
    vel.y *= LINEAR_DAMPING;
    angularVel *= ANGULAR_DAMPING;
}
