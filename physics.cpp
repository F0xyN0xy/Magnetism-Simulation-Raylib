#include "physics.hpp"
#include "constants.hpp"
#include <cmath>

static Vector2 pairwiseForce(Vector2 posA, float chargeA, Vector2 posB, float chargeB) {
    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;
    float distSq = dx*dx + dy*dy;
    float dist = sqrtf(distSq);
    if (dist < MIN_DISTANCE) dist = MIN_DISTANCE;

    float magnitude = (chargeA * chargeB) / (dist * dist);
    if (magnitude > MAX_FORCE) magnitude = MAX_FORCE;
    if (magnitude < -MAX_FORCE) magnitude = -MAX_FORCE;

    float ux = dx / dist;
    float uy = dy / dist;
    return { ux * magnitude, uy * magnitude };
}

void computeAndApplyForces(std::vector<Magnet>& magnets, float dt) {
    int n = (int)magnets.size();
    std::vector<Vector2> netForce(n, {0,0});
    std::vector<float> netTorque(n, 0.0f);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            std::vector<std::pair<Vector2, float>> polesA, polesB;
            magnets[i].getPoles(polesA);
            magnets[j].getPoles(polesB);

            for (auto& [pa, qa] : polesA) {
                for (auto& [pb, qb] : polesB) {
                    Vector2 f = pairwiseForce(pa, qa, pb, qb);

                    netForce[i].x += f.x;
                    netForce[i].y += f.y;
                    netForce[j].x -= f.x;
                    netForce[j].y -= f.y;

                    float leverAX = pa.x - magnets[i].pos.x;
                    float leverAY = pa.y - magnets[i].pos.y;
                    netTorque[i] += leverAX * f.y - leverAY * f.x;

                    float leverBX = pb.x - magnets[j].pos.x;
                    float leverBY = pb.y - magnets[j].pos.y;
                    netTorque[j] += leverBX * (-f.y) - leverBY * (-f.x);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (magnets[i].dragging) continue;

        magnets[i].vel.x = (magnets[i].vel.x + netForce[i].x / MASS * dt) * LINEAR_DAMPING;
        magnets[i].vel.y = (magnets[i].vel.y + netForce[i].y / MASS * dt) * LINEAR_DAMPING;
        magnets[i].angularVel = (magnets[i].angularVel + netTorque[i] * 0.00002f * dt) * ANGULAR_DAMPING;

        magnets[i].pos.x += magnets[i].vel.x * dt;
        magnets[i].pos.y += magnets[i].vel.y * dt;
        magnets[i].angle += magnets[i].angularVel * dt;
    }
}
