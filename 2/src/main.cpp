#include <memory>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#else
#if defined(_MSC_VER)
#define NDEBUG
#endif
#endif

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "triangles.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    try {
        triangles tri(window);

        tri.loop();
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        glfwTerminate();
        return 1;
    }


    // clear GLFW resources
    glfwTerminate();
    return 0;
}
