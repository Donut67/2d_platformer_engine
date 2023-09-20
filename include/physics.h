#pragma once

#include "Precompiled.h"
#include "raylib.h"

void IntegrateForces(Body* b, real dt);
void IntegrateVelocity(Body* b, real dt);

struct PhysycsManager {
	std::vector<Body*> bodies;
	std::vector<Manifold> contacts;

	void Step(void) {
        contacts.clear();
        for (uint32 i = 0; i < bodies.size(); ++i) {
            Body* A = bodies[i];

            for (uint32 j = i + 1; j < bodies.size(); ++j) {
                Body* B = bodies[j];
                if (A->im == 0 && B->im == 0) continue;

                Manifold m(A, B);
                m.Solve();
                if (m.contact_count) contacts.emplace_back(m);
            }
        }

        for (uint32 i = 0; i < bodies.size(); ++i) IntegrateForces(bodies[i], GetFrameTime());
        for (uint32 i = 0; i < contacts.size(); ++i) contacts[i].Initialize();
        for (uint32 i = 0; i < contacts.size(); ++i) contacts[i].ApplyImpulse();
        for (uint32 i = 0; i < bodies.size(); ++i) IntegrateVelocity(bodies[i], GetFrameTime());
        for (uint32 i = 0; i < contacts.size(); ++i) contacts[i].PositionalCorrection();
        for (uint32 i = 0; i < bodies.size(); ++i) {
            Body* b = bodies[i];
            b->force.Set(0, 0);
            b->torque = 0;
        }
    }

	Body* Add(Shape* shape, uint32 x, uint32 y) {
        assert(shape);
        Body* b = new Body(shape, x, y);
        bodies.push_back(b);
        return b;
    }

    void Clear(void) {}
}; 

void IntegrateForces(Body* b, real dt) {
    if (b->im == 0.0f) return;

    b->velocity += (b->force * b->im + gravity) * (dt / 2.0f);
    b->angularVelocity += b->torque * b->iI * (dt / 2.0f);
}

void IntegrateVelocity(Body* b, real dt) {
    if (b->im == 0.0f) return;

    b->position += b->velocity * dt;
    b->orient += b->angularVelocity * dt;
    b->SetOrient(b->orient);
    IntegrateForces(b, dt);
}