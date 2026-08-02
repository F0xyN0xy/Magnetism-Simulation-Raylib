#pragma once

#include <raylib.h>
#include <vector>

struct Magnet {
    Vector2 pos;
    float angle;          // radians, 0 = pointing right
    float strength;
    float length;
    float width;

    // Physics state
    Vector2 vel;
    float angularVel;

    // Interaction state
    bool selected;
    bool dragging;
    Vector2 dragOffset;

    Magnet(float x, float y, float a = 0.0f, float s = 22000.0f,
           float l = 100.0f, float w = 28.0f);

    Vector2 northPos() const;
    Vector2 southPos() const;
    Vector2 direction() const;

    // Returns { {northPos, +strength}, {southPos, -strength} }
    void getPoles(std::vector<std::pair<Vector2, float>>& out) const;

    bool containsPoint(Vector2 p) const;
    void startDrag(Vector2 mouse);
    void dragTo(Vector2 mouse);
    void stopDrag();
    void rotateToward(Vector2 target);
    void adjustStrength(float delta);
    void clampToBounds(int sw, int sh);
    void update();
};
