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

glm::mat4 StaticCube::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(0, 0.0, 0.0));

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

glm::mat4 TranslationCube::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view(1.0);
    float x = 0, y = 0;
    if (horizontal) {
        x = sin((float)glfwGetTime()) * movingLength;
    }
    if (vertical) {
        y = cos(float(glfwGetTime())) * movingLength;
    }
    view = glm::translate(view, glm::vec3(x, y, 0));

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

void TranslationCube::setMovingLength(float n) {
    movingLength = n;
}

TranslationCube::TranslationCube(float movingLength): movingLength(movingLength) {}

glm::mat4 RotationCube::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    auto weight = static_cast<float>(fmod(float(glfwGetTime()), 3.6));
    model = glm::rotate(model, glm::radians(weight * 100.0f), axis);
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(0, 0.0, 0));

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

RotationCube::RotationCube(glm::vec3 axis): axis(axis) {

}

glm::mat4 ScalingCube::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    //[0.2, 2]
    auto factor = static_cast<float>((sin(glfwGetTime()) + 1) * 0.9 + 0.2);
    model = glm::scale(model, glm::vec3(factor, factor, factor));
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(0, 0.0, 0.0));

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

glm::mat4 EllipseTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    auto weight = static_cast<float>(fmod(float(glfwGetTime()), 3.6));
    model = glm::rotate(model, glm::radians(weight * 100.0f), glm::vec3(0, 0, 1));

    glm::mat4 view(1.0);

    if (dynamicSpeed) {
        auto deltaTime = static_cast<float>(glfwGetTime() - time);
        time = static_cast<float>(glfwGetTime());
        float acceleration = (a > b ? (-x + a) / (a) : (-y + b) / (b)) / 1.6 + 0.75;
        posValue += acceleration * deltaTime;
    } else {
        time = posValue = glfwGetTime();
    }

    x = cos(posValue) * a;
    y = sin(posValue) * b;
    view = glm::translate(view, glm::vec3(x, y, 0.0));

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

EllipseTransform::EllipseTransform(float a, float b, bool dynamicSpeed): a(a), b(b), dynamicSpeed(dynamicSpeed) {
    time = static_cast<float>(glfwGetTime());
    posValue = time;
    x = cos(posValue) * a;
    y = sin(posValue) * b;
}

CombinedTransform::CombinedTransform(glm::vec3 rotateAxis, glm::vec3 scale, glm::vec3 translate):
rotateAxis(rotateAxis), scale(scale), translate(translate) {

}

glm::mat4 CombinedTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    auto weight = static_cast<float>(fmod(float(glfwGetTime()), 3.6));
    model = glm::rotate(model, glm::radians(weight * 100.0f), rotateAxis);

    model = glm::scale(model, scale);
    glm::mat4 view(1.0);
    view = glm::translate(view, translate);

    //set camera
    view *= glm::lookAt( glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}
