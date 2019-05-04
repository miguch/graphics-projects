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
#include <glm/glm.hpp>

#include "Cube/Cube.h"
#include "Cube/ColoredCube.h"
#include "Utils/Utils.h"
#include "Cube/Transformation.h"

using namespace std;

enum MODE {
    ORTHOGRAPHIC, PERSPECTIVE, VIEW_CHANGING, CAMERA
};

const char *glsl_version = "#version 150";

void MainLoop::loop(GLFWwindow *window) {

    int w, h;
    tie(w, h) = Utils::getScreenSize();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    array<float, 18> colors = {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
            1, 1, 0,
            1, 0, 1,
            0, 1, 1
    };

    ColoredCube cube(colors, 4, 4, 4);

    unordered_map<MODE, unique_ptr<Transform>> trans;
    trans[MODE::ORTHOGRAPHIC] = make_unique<OrthoTransform>(-5, 5, -5, 5, -5, 5.0f);
    trans[MODE::PERSPECTIVE] = make_unique<PerspectiveTransform>(90, float(w) / h, 0.1, 100);
    trans[MODE::VIEW_CHANGING] = make_unique<ViewChangeTransform>();
    trans[MODE::CAMERA] = make_unique<MovingCamTransform>(Camera(window));

    MODE current = MODE::ORTHOGRAPHIC;

    while (!glfwWindowShouldClose(window)) {
        auto start = clock();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        cube.draw(trans[current].get());

        {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Mode")) {
                    if (current != MODE::ORTHOGRAPHIC && ImGui::MenuItem("Ortho")) {
                        current = MODE::ORTHOGRAPHIC;
                    }
                    if (current != MODE::PERSPECTIVE && ImGui::MenuItem("Perspective")) {
                        current = MODE::PERSPECTIVE;
                    }
                    if (current != MODE::VIEW_CHANGING && ImGui::MenuItem("View Changing")) {
                        current = MODE::VIEW_CHANGING;
                    }
                    if (current != MODE::CAMERA && ImGui::MenuItem("Move")) {
                        current = MODE::CAMERA;
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }

        {
            ImGui::Begin("Options");
            static bool depthTest = true;
            ImGui::Checkbox("Depth Test", &depthTest);
            if (depthTest) {
                Utils::enableDepthTest();
            } else {
                Utils::disableDepthTest();
            }

            if (current == MODE::ORTHOGRAPHIC) {
                GLint w = 20, h = 20, depth = 5;
                w /= 2;
                h /= 2;
                auto ortho = dynamic_cast<OrthoTransform*>(trans[ORTHOGRAPHIC].get());
                ImGui::SliderFloat("Left", &ortho->left, -w, w);
                ImGui::SliderFloat("Right", &ortho->right, -w, w);
                ImGui::SliderFloat("Bottom", &ortho->bottom, -h, h);
                ImGui::SliderFloat("Top", &ortho->top, -h, h);
                ImGui::SliderFloat("Near", &ortho->zNear, -depth, depth);
                ImGui::SliderFloat("Far", &ortho->zFar, -depth, depth);
            } else if (current == MODE::PERSPECTIVE) {
                auto perspective = dynamic_cast<PerspectiveTransform*>(trans[PERSPECTIVE].get());
                int depth = 100;
                ImGui::SliderFloat("Fovy Degree", &perspective->fovy_degree, 0, 180);
                ImGui::SliderFloat("aspect", &perspective->aspect, 0, 10);
                ImGui::SliderFloat("Near", &perspective->zNear, 0.1, depth);
                ImGui::SliderFloat("Far", &perspective->zFar, 0.1, depth);
            } else if (current == MODE::VIEW_CHANGING) {
                auto views = dynamic_cast<ViewChangeTransform*>(trans[VIEW_CHANGING].get());
                const char * labels[3] = {"x", "y", "z"};
                ImGui::ListBox("axis", &views->axis, labels, 3);
            } else if (current == MODE::CAMERA) {
                auto cam = dynamic_cast<MovingCamTransform*>(trans[CAMERA].get());
                ImGui::SliderFloat("speed", &cam->cam.speed, 0, 20);
                ImGui::SliderFloat("mouse sensetivity", &cam->cam.mouseSensetivity, 0, 1);
                if (!cam->cam.usingMouse() && ImGui::Button("Mouse Direction")) {
                    cam->cam.setUsingMouse(true);
                } else if (cam->cam.usingMouse()) {
                    ImGui::Text("Press Esc to get cursor");
                }
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

