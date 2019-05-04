//
// Created by Miguel Chan on 2019-04-03.
//

#include "Transformation.h"
#include "../Utils/Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

glm::mat4 toTransformMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    return projection * view * model;
}

glm::mat4 OrthoTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(-1.5, 0.5, -1.5));
    glm::mat4 projection(1.0);
    projection = glm::ortho(left, right, bottom, top, zNear, zFar);

    return toTransformMatrix(model, view, projection);
}

OrthoTransform::OrthoTransform(float left, float right, float bottom, float top, float zNear, float zFar) :
        left(left), right(right), bottom(bottom), top(top), zNear(zNear), zFar(zFar) {

}

glm::mat4 PerspectiveTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.7f, 1.0f, 0.0f));
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(-1.5, 0.5, -1.5));
    //set camera
    view *= glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(fovy_degree), aspect, zNear, zFar);


    return toTransformMatrix(model, view, projection);
}

PerspectiveTransform::PerspectiveTransform(float fovy_degree, float aspect, float zNear, float zFar):
fovy_degree(fovy_degree), aspect(aspect), zNear(zNear), zFar(zFar) {

}

ViewChangeTransform::ViewChangeTransform() {
    axis = 1;
}

glm::mat4 ViewChangeTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view(1.0);

    float radius = 10;
    float cam_1 = sin(glfwGetTime()) * radius;
    float cam_2 = cos(glfwGetTime()) * radius;
    glm::vec3 camera_pos, up;
    switch (axis) {
        case 0:
            camera_pos = glm::vec3(0, cam_1, cam_2);
            up = glm::vec3(0, cam_2 > 0 ? 1: -1, 0);
            break;
        case 1:
            camera_pos = glm::vec3(cam_1, 0, cam_2);
            up = glm::vec3(0, 1, 0);
            break;
        case 2:
            camera_pos = glm::vec3(cam_1, cam_2, 0);
            up = glm::vec3(cam_2 > 0 ? 1: -1, 0, 0);
            break;
        default:
            throw Utils::GLToolsException("Unexpected Camera Axis");
    }

    //set camera
    view *= glm::lookAt(camera_pos,
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        up);
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);


    return toTransformMatrix(model, view, projection);
}

MovingCamTransform::MovingCamTransform(const Camera &cam): cam(cam) {

}

glm::mat4 MovingCamTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    cam.processInput();
    //set camera
    auto view = cam.getViewMatrix(), projection = cam.getProjectionMatrix();

    return toTransformMatrix(model, view, projection);
}
