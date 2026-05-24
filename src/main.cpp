#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <chrono>

#include "graphic.h"

#include "particle.h"
#include "renderparticles.h"

using namespace std;
using namespace glm;
using namespace chrono_literals;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "Hiraya Buan - GDPHYSX - S41", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    GLuint shader = buildProgram("shaders/default.vert", "shaders/default.frag");

    glEnable(GL_DEPTH_TEST);

    myCamera cam(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 1000.0f,
             vec3(0.0f, 0.0f, 500.0f),
             vec3(0.0f, 0.0f,   0.0f),
             vec3(0.0f, 1.0f,   0.0f));

    Model sphereModel;
    if (!sphereModel.load("3D/sphere.obj")) {
        cerr << "Failed to load sphere.obj\n";
        return -1;
    }

    PHYSX::Particle particle;
    particle.Velocity = vec3(100, 0, 0);
    particle.Acceleration = vec3(-30, 0, 0);

    list<RenderParticle> RenderParticles;
    RenderParticles.emplace_back(&particle, &sphereModel, vec3(1.0f, 0.0f, 0.0f), vec3(100.0f));

    constexpr chrono::nanoseconds timestep(64ms);
    using Clock = chrono::high_resolution_clock;
    auto curr_time = Clock::now();
    auto prev_time = curr_time;
    chrono::nanoseconds curr_ns(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        curr_time = Clock::now();
        auto dur  = chrono::duration_cast<chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;
        if (curr_ns >= timestep) {
            constexpr float timestep_sec = timestep.count() / (float)(1E09);
            particle.Update(timestep_sec);
            curr_ns -= timestep;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &rp : RenderParticles)
            rp.Draw(shader, cam);

        glfwSwapBuffers(window);
    }
    sphereModel.cleanup();
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}