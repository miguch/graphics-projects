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

#include "Cube/TextureCube.h"
#include "Cube/ColoredCube.h"
#include "Cube/LightingCube.h"
#include "Utils/Utils.h"
#include "Cube/Transformation.h"

using namespace std;

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

    LightingCube cube(glm::vec3(1.0f, 0.5f, 0.31f), 0.5, 0.5, 0.5);
    LightingCube source(glm::vec3(1, 1, 1), cube.getVertexBuffer()->VBO);
    source.ambientStrength = 1;
    cube.setViewPos(glm::vec3(0.0f, 0.0f, 10.0f));
    CombinedTransform rotate(glm::vec3(1.0f, 0.0f, 0.5f), glm::vec3(3.5, 3.5, 3.5), glm::vec3(0, 0, 0));
    rotate.setRotationSpeed(0.2);
    EllipseTransform sourceMovement(5, 3);

    while (!glfwWindowShouldClose(window)) {
        auto start = clock();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        source.draw(&sourceMovement);
        cube.setLightPos(glm::vec3(sourceMovement.getPosition(), 0));
        cube.draw(&rotate);

        {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Mode")) {
                    if (cube.getCurrent() != cube.PHONG && ImGui::MenuItem("PHONG")) {
                        cube.setCurrent(cube.PHONG);
                    }
                    if (cube.getCurrent() != cube.GOURAUD && ImGui::MenuItem("GOUROUD")) {
                        cube.setCurrent(cube.GOURAUD);
                    }
                    if (cube.getCurrent() != cube.BLINN_PHONG && ImGui::MenuItem("BLINN PHONG")) {
                        cube.setCurrent(cube.BLINN_PHONG);
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

            ImGui::SliderFloat("AmbientStrength", &cube.ambientStrength, 0, 1);
            ImGui::SliderFloat("DiffuseStrength", &cube.diffuseStrength, 0, 1);
            ImGui::SliderFloat("SpecularStrength", &cube.specularStrength, 0, 1);
            ImGui::SliderInt("Shininess", &cube.shininess, 2, 512);

            ImGui::Checkbox("Cube Pause", &rotate.pause);
            ImGui::Checkbox("Light Source Pause", &sourceMovement.pause);

            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

