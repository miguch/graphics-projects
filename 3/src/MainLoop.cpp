//
// Created by Miguel Chan on 2019-03-22.
//

#include "MainLoop.h"

#include <array>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <ctime>
#include <thread>
#include <exception>

#include "shader.h"
#include "Bresenham/Bresenham.h"
#include "Bresenham/BresenhamCircle.h"
#include "Bresenham/BresenhamLine.h"
#include "Bresenham/RasterizedTriangle.h"

using namespace std;

enum MODE {
    TRIANGLE_OUTLINE, CIRCLE, TRIANGLE_FILLED
};

const char *glsl_version = "#version 150";

void MainLoop::loop(GLFWwindow *window) {

    array<GLfloat, 6> triangle_vertices = {
            0, 0.7, -0.7, -0.7, 0.7, -0.7
    };
    array<GLfloat, 3> outline_color = {
            1,1,1
    };
    array<GLfloat, 2> circle_center = {0, 0};
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    GLint circle_radius = static_cast<GLint>(0.4 * min(w, h) / 2);
    array<GLfloat, 3> circle_color = {
            1,1,1
    };
    array<GLfloat, 6> filled_triangle_vertices = {
            0, 0.7, -0.7, -0.7, 0.7, -0.7
    };
    array<GLfloat, 3> filled_color = {
            1,1,1
    };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    unordered_map<MODE, unique_ptr<Bresenham>> drawers;
    drawers[TRIANGLE_OUTLINE] = make_unique<BresenhamLine>(window, triangle_vertices);
    drawers[CIRCLE] = make_unique<BresenhamCircle>(window, circle_center[0], circle_center[1], circle_radius);
    drawers[TRIANGLE_FILLED] = make_unique<RasterizedTriangle>(window, filled_triangle_vertices);
    MODE current_mode = TRIANGLE_OUTLINE;

    while (!glfwWindowShouldClose(window)) {
        auto start = clock();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto &drawer = drawers[current_mode];
        drawer->draw();

        bool updateVertices = false;

        {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Mode")) {
                    if (current_mode != CIRCLE && ImGui::MenuItem("Circle")) {
                        current_mode = CIRCLE;
                    }
                    if (current_mode != TRIANGLE_OUTLINE && ImGui::MenuItem("Triangle Outline")) {
                        current_mode = TRIANGLE_OUTLINE;
                    }
                    if (current_mode != TRIANGLE_FILLED && ImGui::MenuItem("Triangle Filled")) {
                        current_mode = TRIANGLE_FILLED;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }
        {
            ImGui::Begin("Bresenham");
            switch (current_mode) {
                case TRIANGLE_OUTLINE:
                    ImGui::Text("Vertices Position:");
                    ImGui::Text("a: ");
                    updateVertices = ImGui::SliderFloat2("##a", &triangle_vertices[0], -1, 1) || updateVertices;
                    ImGui::Text("b: ");
                    updateVertices = ImGui::SliderFloat2("##b", &triangle_vertices[2], -1, 1) || updateVertices;
                    ImGui::Text("c: ");
                    updateVertices = ImGui::SliderFloat2("##c", &triangle_vertices[4], -1, 1) || updateVertices;
                    if (updateVertices) {
                        dynamic_cast<BresenhamLine *>(drawer.get())->setVertices(triangle_vertices);
                    }
                    ImGui::Text("Color: ");
                    if (ImGui::ColorEdit3("##Triangle-outline", outline_color.data())) {
                        dynamic_cast<BresenhamLine *>(drawer.get())->setColor(outline_color[0], outline_color[1], outline_color[2]);
                    }
                    break;
                case CIRCLE:
                    ImGui::Text("Center Position:");
                    if (ImGui::SliderFloat2("##center", &circle_center[0], -1, 1)) {
                        dynamic_cast<BresenhamCircle *>(drawer.get())->setCenter(circle_center[0], circle_center[1]);
                    }
                    ImGui::Text("Radius:");
                    glfwGetFramebufferSize(window, &w, &h);
                    if (ImGui::SliderInt("##radius", &circle_radius, 0, min(w, h) / 2)) {
                        dynamic_cast<BresenhamCircle *>(drawer.get())->setRadius(circle_radius);
                    }
                    ImGui::Text("Color: ");
                    if (ImGui::ColorEdit3("##circle-outline", circle_color.data())) {
                        dynamic_cast<BresenhamCircle *>(drawer.get())->setColor(circle_color[0], circle_color[1], circle_color[2]);
                    }
                    break;
                case TRIANGLE_FILLED:
                    ImGui::Text("Vertices Position:");
                    ImGui::Text("a: ");
                    updateVertices = ImGui::SliderFloat2("##filled-a", &filled_triangle_vertices[0], -1, 1) || updateVertices;
                    ImGui::Text("b: ");
                    updateVertices = ImGui::SliderFloat2("##filled-b", &filled_triangle_vertices[2], -1, 1) || updateVertices;
                    ImGui::Text("c: ");
                    updateVertices = ImGui::SliderFloat2("##filled-c", &filled_triangle_vertices[4], -1, 1) || updateVertices;
                    if (updateVertices) {
                        dynamic_cast<RasterizedTriangle *>(drawer.get())->setVertices(filled_triangle_vertices);
                    }
                    ImGui::Text("Color: ");
                    if (ImGui::ColorEdit3("##filled-Triangle", filled_color.data())) {
                        dynamic_cast<RasterizedTriangle *>(drawer.get())->setColor(filled_color[0], filled_color[1], filled_color[2]);
                    }
                    break;
            }
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
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

}

