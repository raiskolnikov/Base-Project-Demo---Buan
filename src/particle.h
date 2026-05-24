#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

namespace PHYSX {
    class Particle {
    protected:
      void UpdatePosition(float deltaTime);
      void UpdateVelocity(float deltaTime);

    public:
      vec3 Position;
      vec3 Velocity;
      vec3 Acceleration;

      void Update(float deltaTime);

      Particle();
    };
}
