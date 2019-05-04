//
// Created by Miguel Chan on 2019-04-02.
//

#include "Utils.h"
#include <glad/glad.h>
#include <functional>
#include <iostream>
#include <shared_mutex>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

namespace Utils {

    void enableDepthTest() {
        glEnable(GL_DEPTH_TEST);
    }

    void disableDepthTest() {
        glDisable(GL_DEPTH_TEST);
    }

    struct ScreenSize {
        GLuint screenWidth, screenHeight;
        mutable shared_mutex mutex;
    } screenSize;
    std::pair<GLuint, GLuint> getScreenSize() {
        shared_lock<shared_mutex> guard(screenSize.mutex);
        return {screenSize.screenWidth, screenSize.screenHeight};
    }

    void setScreenSize(GLuint w, GLuint h) {
        unique_lock<shared_mutex> guard(screenSize.mutex);
        screenSize.screenWidth = w;
        screenSize.screenHeight = h;
    }

    void printMat4(const glm::mat4& mat) {
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 4; k++) {
                cout << mat[k][i] << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }

    void printVec3(const glm::vec3 &vec) {
        cout << vec.x << ' ' << vec.y << ' ' << vec.z << endl;
    }


    GLToolsException::GLToolsException(const char *msg)  : msg(msg) {}

    const char *GLToolsException::what() const noexcept {
        return msg.c_str();
    }

    auto vertexAttribSetter = []() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        //Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    };

    void VertexBuffer::bind() {
        glBindVertexArray(VAO);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    VertexBuffer::VertexBuffer(int size, float *data, const std::function<void()> &vertexAttribCallback) {
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
    }

    ElementBuffer::~ElementBuffer() {
        glDeleteBuffers(1, &EBO);
    }

    ElementBuffer::ElementBuffer(GLuint VAO, int size, unsigned int *data) {
        glBindVertexArray(VAO);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        if (size != 0 && data != nullptr) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }
        glBindVertexArray(0);
    }
}