//
// Created by Miguel Chan on 2019-03-22.
//

#include "MainLoop.h"

#include <array>
#include <ctime>
#include <thread>
#include <glm/glm.hpp>

#include "Objects/Cube/TextureCube.h"
#include "Objects/Cube/LightingCube.h"
#include "Objects/Surface/Planar.h"

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

    glm::vec3 cameraPos(-7, 3, 6.5);

    LightingCube source(glm::vec3(1, 1, 1), 0.5, 0.5, 0.5);
    source.lighting.ambientStrength = 1;
    CombinedTransform sourceMovement(glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(-0.8, 0, 0));
    sourceMovement.setRotationSpeed(0.2);

    LightingCube object(glm::vec3(1.0f, 0.5f, 0.31f), 1, 1, 1);
    CombinedTransform objectTrans(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1));
    objectTrans.pause = true;
    object.lighting.ambientStrength = 0.3;
    object.lighting.diffuseStrength = 0.9;
    object.lighting.shininess = 64;
    object.lighting.specularStrength = 1;

    LightingCube rotatingObject(glm::vec3(0.32, 0.5, 0.45), 0.6, 0.6, 0.6);
    CombinedTransform rotatingTrans(glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(-2, -0.7, -2));
    rotatingTrans.setRotationSpeed(0.3);
    rotatingObject.lighting.ambientStrength = 0.3;
    rotatingObject.lighting.diffuseStrength = 0.9;
    rotatingObject.lighting.shininess = 64;
    rotatingObject.lighting.specularStrength = 1;

    array<glm::vec3, 4> points = {
            glm::vec3(-3, -1, -3),
            glm::vec3(3, -1, -3),
            glm::vec3(3, -1, 3),
            glm::vec3(-3, -1, 3)
    };
    CombinedTransform surfaceTrans(glm::vec3(0, 0, 1), glm::vec3(2, 2, 2), glm::vec3(0, 0, 0));
    Planar surface(points, glm::vec3(0, 1, 0), glm::vec3(0.59, 0.74, 0.2),
                   sourceMovement.getTranslate(), surfaceTrans.getCameraPos());
    surfaceTrans.pause = true;

    surface.lighting.ambientStrength = 0.25;
    surface.lighting.diffuseStrength = 0.3;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        object.draw(&objectTrans);
        source.draw(&sourceMovement);
        rotatingObject.draw(&rotatingTrans);
        surface.lighting.lightPos = sourceMovement.getTranslate();
        surface.draw(&surfaceTrans);
        object.lighting.syncLighting(surface.lighting);
        rotatingObject.lighting.syncLighting(surface.lighting);

        {
            if (ImGui::BeginMainMenuBar()) {

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

            ImGui::SliderFloat("AmbientStrength", &surface.lighting.ambientStrength, 0, 1);
            ImGui::SliderFloat("DiffuseStrength", &surface.lighting.diffuseStrength, 0, 1);
            ImGui::SliderFloat("SpecularStrength", &surface.lighting.specularStrength, 0, 1);
            ImGui::SliderInt("Shininess", &surface.lighting.shininess, 2, 512);

            ImGui::SliderFloat("Source X", (float *) &sourceMovement.getTranslate().x, -5, 5);
            ImGui::SliderFloat("Source Y", (float *) &sourceMovement.getTranslate().y, -0.2, 5);
            ImGui::SliderFloat("Source Z", (float *) &sourceMovement.getTranslate().z, -5, 5);

            ImGui::SliderFloat("Camera X", &cameraPos.x, -20, 20);
            ImGui::SliderFloat("Camera Y", &cameraPos.y, 0.1, 20);
            ImGui::SliderFloat("Camera Z", &cameraPos.z, 0.1, 15);
            sourceMovement.setCameraPos(cameraPos);
            surfaceTrans.setCameraPos(cameraPos);
            objectTrans.setCameraPos(cameraPos);
            rotatingTrans.setCameraPos(cameraPos);
            surface.lighting.viewPos = cameraPos;

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

