//
// Created by Miguel Chan on 2019-03-19.
//

#include "Bresenham.h"
#include <iostream>
#include <glad/glad.h>
#include <cmath>
#include <functional>

using namespace std;

auto vertexAttribSetter = []() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

pair<GLuint, GLuint> genVertex(int size, float *data,
                               const function<void()> &vertexAttribCallback) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (size != 0 && data != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    vertexAttribCallback();
    // Unbind
    glBindVertexArray(0);
    return make_pair(VAO, VBO);
}

Bresenham::Bresenham(GLFWwindow *window) : window(window), shader("shaders/vert.glsl", "shaders/frag.glsl") {
    updateWindowSize();
    tie(VAO, VBO) = genVertex(0, nullptr, vertexAttribSetter);
}


// Returns true if there has been a change in the window size
bool Bresenham::updateWindowSize() {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if (windowWidth != w || windowHeight != h) {
        windowWidth = w;
        windowHeight = h;

        return true;
    }
    return false;
}

Bresenham::~Bresenham() = default;

void Bresenham::useShader() {
    shader.use();
}

