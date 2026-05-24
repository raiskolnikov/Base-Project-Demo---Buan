#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

GLuint buildProgram(const string &vertPath, const string &fragPath) {
    ifstream vFile(vertPath), fFile(fragPath);
    if (!vFile.is_open()) {
        cerr << "Failed to open: " << vertPath << "\n";
        return 0;
    }
    if (!fFile.is_open()) {
        cerr << "Failed to open: " << fragPath << "\n";
        return 0;
    }

    stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    const string vertCode = vStream.str();
    const string fragCode = fStream.str();
    const char *vertSrc = vertCode.c_str();
    const char *fragSrc = fragCode.c_str();

    auto compile = [](GLenum type, const char *src) {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        int ok;
        char log[512];
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            glGetShaderInfoLog(s, 512, nullptr, log);
            cerr << "Shader error: " << log << "\n";
        }
        return s;
    };

    GLuint vs = compile(GL_VERTEX_SHADER,   vertSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragSrc);
    GLuint p  = glCreateProgram();

    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return p;
}

class myCamera {
public:
    vec3 position;
    vec3 target;
    vec3 up;

    float left, right, bottom, top, nearPlane, farPlane;

    myCamera(float left   = -5.0f, float right  = 5.0f,
             float bottom = -5.0f, float top    = 5.0f,
             float nearP  =  0.1f, float farP   = 100.0f,
             vec3  pos    = vec3(0.0f, 0.0f, 10.0f),
             vec3  target = vec3(0.0f, 0.0f,  0.0f),
             vec3  up     = vec3(0.0f, 1.0f,  0.0f))
        : left(left), right(right), bottom(bottom), top(top),
          nearPlane(nearP), farPlane(farP),
          position(pos), target(target), up(up) {}

    mat4 getView()       const { return lookAt(position, target, up); }
    mat4 getProjection() const { return ortho(left, right, bottom, top, nearPlane, farPlane); }
};

class Model {
public:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    int indexCount = 0;

    bool load(const string &path) {
        tinyobj::attrib_t attrib;
        vector<tinyobj::shape_t>    shapes;
        vector<tinyobj::material_t> materials;
        string warn, err;

        bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials,
                                   &warn, &err, path.c_str());
        if (!warn.empty()) cout << "[tinyobj] " << warn << "\n";
        if (!err.empty())  cerr << "[tinyobj] " << err  << "\n";
        if (!ok) return false;

        vector<float> vertices;
        vector<unsigned int> indices;

        for (auto &shape : shapes) {
            for (auto &idx : shape.mesh.indices) {
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                }

                indices.push_back((unsigned int)indices.size());
            }
        }

        indexCount = (int)indices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(float),
                     vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                              6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        return true;
    }

    void draw(GLuint shader, const myCamera &cam,
              vec3 position, vec3 scale, vec3 color) const {
        glUseProgram(shader);

        mat4 model = translate(mat4(1.0f), position);
        model = glm::scale(model, scale);

        glUniformMatrix4fv(glGetUniformLocation(shader, "model"),      1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"),       1, GL_FALSE, value_ptr(cam.getView()));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(cam.getProjection()));

        glUniform3fv(glGetUniformLocation(shader, "objectColor"), 1, value_ptr(color));
        glUniform3f (glGetUniformLocation(shader, "lightColor"),  1.0f, 1.0f, 1.0f);
        glUniform3f (glGetUniformLocation(shader, "lightPos"),    250.0f, 250.0f, 250.0f);
        glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, value_ptr(cam.position));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};