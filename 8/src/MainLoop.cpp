//
// Created by Miguel Chan on 2019-03-22.
//

#include "MainLoop.h"

#include <array>
#include <ctime>
#include <thread>
#include <glm/glm.hpp>

#include "Shader/shaderPaths.h"
#include "Shader/shader.h"
#include "Utils/Utils.h"
#include "Bezier.h"

using namespace std;

const char *glsl_version = "#version 150";

struct MouseClick {
    float x, y;
    bool updated = false;
    bool revert = false;
} MOUSE_CLICK;

void MainLoop::loop(GLFWwindow *window) {

    int w, h;
    tie(w, h) = Utils::getScreenSize();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Bezier curve;
    Utils::VertexArrayBuffer buf(0, nullptr, []() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window,int button, int action, int mods) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (!ImGui::IsMouseHoveringAnyWindow()) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                auto size = Utils::getScreenSize();
                MOUSE_CLICK.x = (x / size.first) * 2 - 1;
                MOUSE_CLICK.y = (y / size.second) * (-2) + 1;
                MOUSE_CLICK.updated = true;
            }
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            MOUSE_CLICK.revert = true;
        }
    });
    Shader shader(ShaderPaths::simpleShader);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (MOUSE_CLICK.updated) {
            curve.addPoint(glm::vec3(MOUSE_CLICK.x, MOUSE_CLICK.y, 0));
            cout <<  MOUSE_CLICK.x << " " << MOUSE_CLICK.y << endl;
            MOUSE_CLICK.updated = false;
        }
        if (MOUSE_CLICK.revert) {
            curve.removePoint();
            MOUSE_CLICK.revert = false;
        }

        // Draw control points
        shader.use();

        buf.bind();
        shader.setVec3("verColor", 1, 1, 1);
        glPointSize(20);
        glBufferData(GL_ARRAY_BUFFER, curve.points.size() * 3 * sizeof(float), curve.points.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, curve.points.size());

        // Draw outline
        shader.setVec3("verColor", 0.5, 0.5, 1);
        glDrawArrays(GL_LINE_STRIP, 0, curve.points.size());

        // Draw curve
        static int steps = 1000;

        auto curvePoints = curve.getCurve(steps);

        shader.setVec3("verColor", 1, 0.2, 0.2);
        glPointSize(5);
        glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(float), curvePoints.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size() / 3);


        if (curve.animating) {
            auto animatePoints = curve.getAnimationLines();
            shader.setVec3("verColor", 0.2, 1, 0.2);
            for (auto c : animatePoints) {
                Utils::printVec3(c);
            }
            cout << endl;
            glBufferData(GL_ARRAY_BUFFER, animatePoints.size() * 3 * sizeof(float), animatePoints.data(), GL_DYNAMIC_DRAW);
            glPointSize(6);
            glDrawArrays(GL_POINTS, 0, animatePoints.size());
            glDrawArrays(GL_LINES, 0, animatePoints.size());
        }

        {
            ImGui::Begin("window");

            ImGui::InputInt("Step", &steps);
            if (ImGui::Button("Animate Drawing")) {
                curve.animate();
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

    }
}

