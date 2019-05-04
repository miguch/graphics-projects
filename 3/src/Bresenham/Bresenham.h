//
// Created by Miguel Chan on 2019-03-19.
//

#ifndef DRAWLINE_BRESENHAM_H
#define DRAWLINE_BRESENHAM_H

#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../shader.h"

class Bresenham {
public:
    template <class T>
    struct Point {
        T x, y;
    };
protected:
    int windowWidth, windowHeight;
    GLFWwindow* window;
    Shader shader;
    GLuint VAO, VBO;

public:
    explicit Bresenham(GLFWwindow* window);
    virtual void draw() = 0;
    void useShader();
    bool updateWindowSize();
    virtual ~Bresenham();
};



#endif //DRAWLINE_BRESENHAM_H
