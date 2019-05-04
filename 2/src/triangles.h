//
// Created by Miguel Chan on 2019-03-07.
//

#ifndef TRIANGLE_GUI_TRIANGLES_H
#define TRIANGLE_GUI_TRIANGLES_H

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class triangles {
public:
    //Different display mode, according to the requirements.
    enum Mode {
        RASTERIZED_TRIANGLE,
        SIMPLE_TRIANGLE,
        CUSTOM_TRIANGLE,
        MULTIPLE_TRIANGLE,
        LINES
    };

    explicit triangles(GLFWwindow *window);

    void loop();

private:
    GLFWwindow *window;
    GLuint RGB_VAO, RGB_VBO, simple_VAO, simple_VBO, custom_VBO, custom_VAO, element_VBO, element_VAO, tri_EBO, line_EBO;
    Mode mode;
    std::unique_ptr<Shader> shader, uniformShader;
};


#endif //TRIANGLE_GUI_TRIANGLES_H
