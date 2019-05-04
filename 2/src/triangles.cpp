//
// Created by Miguel Chan on 2019-03-07.
//
#include <array>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "triangles.h"

using namespace std;

const array<float, 3> RED = {1, 0, 0};
const array<float, 3> BLUE = {0, 0, 1};
const array<float, 3> GREEN = {0, 1, 0};
const array<float, 3> WHITE = {1, 1, 1};
const char *glsl_version = "#version 330";

const vector<pair<const char *, triangles::Mode>> MSG_MODE = {
        {"RGB Triangle",     triangles::Mode::RASTERIZED_TRIANGLE},
        {"Simple Triangle",  triangles::Mode::SIMPLE_TRIANGLE},
        {"Uniform Triangle", triangles::Mode::CUSTOM_TRIANGLE},
        {"EOB",              triangles::Mode::MULTIPLE_TRIANGLE},
        {"Lines",            triangles::Mode::LINES},
};

array<float, 18> rgb_vertices = {
        1, -1, 0.0f, 0, 0, 1, -1, -1, 0.0f, 0, 1, 0, 0.0f, 1, 0.0f, 1, 0, 0,
};
array<float, 18> simple_vertices = {
        0.7f, -0.7f, 0.0f, 0.56f, 0.72f, 0.33f, -0.7f, -0.7f, 0.0f,
        0.56f, 0.72f, 0.33f, 0.0f, 0.7f, 0.0f, 0.56f, 0.72f, 0.33f,
};
array<float, 9> custom_vertices = {
        0.8f, -0.8f, 0.0f, -0.8f, -0.8f, 0.0f, 0.0f, 0.8f, 0.0f,
};
array<float, 36> elements_vertices = {
        0.8, -0.8, 0.0f, 0, 1, 0, -0.8, -0.8, 0.0f, 0, 1, 0,
        0.0f, 0.8, 0.0f, 1, 0, 0, -0.4, 0, 0, 0, 0, 1,
        0.4, 0, 0, 0, 0, 1, 0, -0.8, 0, 0, 0, 1,
};
array<unsigned int, 9> tri_elements_indices = {
        1, 3, 5, 2, 3, 4, 0, 4, 5,
};
array<unsigned int, 14> line_elements_indices = {1, 3, 3, 2, 2, 4, 4,
                                                 0, 3, 4, 4, 1, 3, 0};

// Generate the vertex buffer object(VBO) and vertex array object(VAO)
pair<GLuint, GLuint> genVertex(int size, float *data,
                               const function<void()> &vertexAttribCallback) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    vertexAttribCallback();
    // Unbind
    glBindVertexArray(0);
    return make_pair(VAO, VBO);
}

GLuint genEBO(GLuint VAO, int size, unsigned int *data) {
    GLuint EBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindVertexArray(0);
    return EBO;
}

void triangles::loop() {

    while (!glfwWindowShouldClose(window)) {
        auto start = clock();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Background
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (mode == Mode::RASTERIZED_TRIANGLE) {
            shader->use();
            glBindVertexArray(RGB_VAO);
            {
                static int pos = 0;
                ImGui::Begin("Change Triangle Color!");
                ImGui::Text("Change: ");
                if (ImGui::Button("Top")) {
                    pos = 3;
                }
                ImGui::SameLine();
                if (ImGui::Button("Left")) {
                    pos = 2;
                }
                ImGui::SameLine();
                if (ImGui::Button("Right")) {
                    pos = 1;
                }
                if (pos != 0) {
                    ImGui::ColorEdit3("", rgb_vertices.data() + ((pos - 1) * 6 + 3));
                }
                ImGui::End();
            }
            glBindBuffer(GL_ARRAY_BUFFER, RGB_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rgb_vertices), rgb_vertices.data(),
                         GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } else if (mode == Mode::SIMPLE_TRIANGLE) {
            shader->use();
            glBindVertexArray(simple_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } else if (mode == Mode::CUSTOM_TRIANGLE) {
            uniformShader->use();
            static array<float, 3> customColor = WHITE;
            // color picker GUI for custom triangle
            {
                ImGui::Begin("Choose Triangle Color!");
                ImGui::ColorEdit3("", customColor.data());

                ImGui::End();
            }
            glBindVertexArray(custom_VAO);
            int colorLocation =
                    glGetUniformLocation(uniformShader->getProgram(), "uniColor");
            glUniform4f(colorLocation, customColor[0], customColor[1], customColor[2],
                        1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } else if (mode == Mode::LINES) {
            shader->use();
            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glBindVertexArray(simple_VAO);

            //Draw lines of a triangle
            glDrawArrays(GL_LINE_LOOP, 0, 3);
            glBindVertexArray(element_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_EBO);
            glDrawElements(GL_LINES, 14, GL_UNSIGNED_INT, 0);
        } else if (mode == Mode::MULTIPLE_TRIANGLE) {
            shader->use();
            glBindVertexArray(element_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_EBO);
            glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        }

        // Main GUI
        {
            ImGui::Begin("Triangles!");
            ImGui::Text("Show Me The: ");

            int index = 0;
            for (const auto &kv : MSG_MODE) {
                if (ImGui::Button(kv.first)) {
                    mode = kv.second;
                }
                if (++index % 2 == 1) {
                    ImGui::SameLine();
                }
            }
            if (ImGui::Button("Exit")) {
                exit(0);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glfwPollEvents();

        auto waitTime = static_cast<long long>(
                1000.0 / 60 - double(clock() - start) / CLOCKS_PER_SEC * 1000);
        // Limit fps
        this_thread::sleep_for(chrono::milliseconds(waitTime));
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

triangles::triangles(GLFWwindow *window) : window(window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    auto vertexAttributeSetter = []() {
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *) 0);
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    };
    auto uniformAttributeSetter = []() {
        // Only position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *) 0);
        glEnableVertexAttribArray(0);
    };

    tie(RGB_VAO, RGB_VBO) = genVertex(sizeof(rgb_vertices), rgb_vertices.data(),
                                      vertexAttributeSetter);
    tie(simple_VAO, simple_VBO) = genVertex(
            sizeof(simple_vertices), simple_vertices.data(), vertexAttributeSetter);
    tie(custom_VAO, custom_VBO) = genVertex(
            sizeof(custom_vertices), custom_vertices.data(), uniformAttributeSetter);
    tie(element_VAO, element_VBO) =
            genVertex(sizeof(elements_vertices), elements_vertices.data(),
                      vertexAttributeSetter);

    tri_EBO = genEBO(element_VAO, sizeof(tri_elements_indices),
                     tri_elements_indices.data());
    line_EBO = genEBO(element_VAO, sizeof(line_elements_indices),
                      line_elements_indices.data());

    shader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");
    uniformShader = std::make_unique<Shader>("shaders/vert.glsl",
                                             "shaders/uniform_frag.glsl");

    mode = Mode::RASTERIZED_TRIANGLE;
}
