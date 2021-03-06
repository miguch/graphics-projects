//
// Created by Miguel Chan on 2019-04-10.
//

#include "Camera.h"
#include "../Utils/Utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace std;

void Camera::moveForward(GLfloat const distance) {
    auto moveDist = distance * getCameraFront();
    cameraPos += moveDist;
    target += moveDist;
}

void Camera::moveBack(GLfloat const distance) {
    auto moveDist = distance * getCameraFront();
    cameraPos -= moveDist;
    target -= moveDist;
}

void Camera::moveRight(GLfloat const distance) {
    auto moveDist = distance * glm::normalize(glm::cross(getCameraFront(), cameraUp));
    cameraPos += moveDist;
    target += moveDist;
}

void Camera::moveLeft(GLfloat const distance) {
    auto moveDist = distance * glm::normalize(glm::cross(getCameraFront(), cameraUp));
    cameraPos -= moveDist;
    target -= moveDist;
}

void Camera::eulerRotate(GLfloat delta_pitch, GLfloat delta_yaw) {
    current_euler.x += delta_pitch;
    current_euler.y += delta_yaw;
    if (current_euler.x > 89.0f)
        current_euler.x = 89.0f;
    if (current_euler.x < -89.0f)
        current_euler.x = -89.0f;

    glm::vec3 front;

    front.x = cos(glm::radians(current_euler.x)) * cos(glm::radians(current_euler.y));
    front.y = sin(glm::radians(current_euler.x));
    front.z = cos(glm::radians(current_euler.x)) * sin(glm::radians(current_euler.y));

    auto frontDist = glm::length(target - cameraPos);
    this->target = cameraPos + glm::normalize(front) * frontDist;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(cameraPos, target, cameraUp);
}

void Camera::processInput() {
    float current = glfwGetTime();
    float delta = current - lastTime;
    lastTime = current;
    float distance = speed * delta;
    const std::unordered_map<int, std::function<void()>> actions = {
            {GLFW_KEY_W,      [&]() { moveForward(distance); }},
            {GLFW_KEY_S,      [&]() { moveBack(distance); }},
            {GLFW_KEY_A,      [&]() { moveLeft(distance); }},
            {GLFW_KEY_D,      [&]() { moveRight(distance); }},
            {GLFW_KEY_ESCAPE, [&]() { setUsingMouse(false); }}
    };
    for (auto &act : actions) {
        if (glfwGetKey(window, act.first) == GLFW_PRESS) {
            act.second();
        }
    }

    if (useMouse) {
        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
        double delta_x = cursor_x - lastCursor_x;
        double delta_y = lastCursor_y - cursor_y;
        lastCursor_x = cursor_x;
        lastCursor_y = cursor_y;
        delta_x *= mouseSensetivity;
        delta_y *= mouseSensetivity;
        if (delta_x || delta_y) {
            eulerRotate(delta_y, delta_x);
        }
    }
}


Camera::Camera(GLFWwindow *window) {
    float w, h;
    tie(w, h) = Utils::getScreenSize();
    pfov = 45.0f;
    pratio = w / h;
    pnear = 0.01;
    pfar = 100.0;
    cameraPos = glm::vec3(0, 0, 10);
    setTarget(glm::vec3(0, 0, 0));
    cameraUp = glm::vec3(0, 1, 0);
    cameraRight = glm::normalize(glm::cross(cameraUp, getCameraFront()));
    speed = 5;
    this->window = window;
    useMouse = false;
    lastTime = glfwGetTime();
    mouseSensetivity = 0.05;
    current_euler = glm::vec3(0, 270, 0);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(pfov), pratio, pnear, pfar);
}

void Camera::setUsingMouse(bool b) {
    useMouse = b;
    if (useMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &lastCursor_x, &lastCursor_y);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

bool Camera::usingMouse() {
    return useMouse;
}

const glm::vec3 &Camera::getCameraPos() const {
    return cameraPos;
}

glm::vec3 Camera::getTarget() const {
    return target;
}

void Camera::setCameraPos(const glm::vec3 &cameraPos) {
    Camera::cameraPos = cameraPos;
}

void Camera::setTarget(const glm::vec3& target) {
    auto dir = glm::normalize(target - cameraPos);
    current_euler.x = glm::degrees(atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z)));
    current_euler.y = glm::degrees(atan2(dir.z, dir.x));
    this->target = target;
}

glm::mat4 Camera::getLookAt() {
    return glm::lookAt(cameraPos, getTarget(), glm::vec3(0, 1, 0));
}

glm::vec3 Camera::getCameraFront() const {
    return glm::normalize(target - cameraPos);
}

