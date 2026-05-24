#pragma once

#include <glm/glm.hpp>
#include "particle.h"
#include "graphic.h"

using namespace glm;

class RenderParticle {
public:
    PHYSX::Particle *PhysicsParticle;
    Model *RenderModel;
    vec3 Color;
    vec3 Scale;

    RenderParticle(PHYSX::Particle *p, Model *m,
                   vec3 color = vec3(1.0f),
                   vec3 scale = vec3(1.0f))
        : PhysicsParticle(p), RenderModel(m),
          Color(color), Scale(scale) {}

    void Draw(GLuint shader, const myCamera &cam) const {
        RenderModel->draw(shader, cam,
                          PhysicsParticle->Position,
                          Scale, Color);
    }
};