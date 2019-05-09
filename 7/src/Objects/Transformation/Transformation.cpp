//
// Created by Miguel Chan on 2019-04-03.
//

#include "Transformation.h"
#include "../../Utils/Utils.h"

#include <glm/gtc/type_ptr.hpp>

using namespace std;

TransformMatrix toTransformMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    return TransformMatrix(model, view, projection);
}

TransformMatrix OrthoTransform::getTransformMatrix() {
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

TransformMatrix PerspectiveTransform::getTransformMatrix() {
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

PerspectiveTransform::PerspectiveTransform(float fovy_degree, float aspect, float zNear, float zFar) :
        fovy_degree(fovy_degree), aspect(aspect), zNear(zNear), zFar(zFar) {

}


TransformMatrix EllipseTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    glm::mat4 model(1.0);
    auto weight = static_cast<float>(fmod(float(glfwGetTime()), 3.6));

    if (dynamicSpeed) {
        auto deltaTime = static_cast<float>(glfwGetTime() - time);
        time = static_cast<float>(glfwGetTime());
        float acceleration = (a > b ? (-x + a) / (a) : (-y + b) / (b)) / 1.6 + 0.75;
        if (!pause) {
            posValue += acceleration * deltaTime;
        }
    } else {
        if (!pause) {
            posValue += glfwGetTime() - time;
        }
        time = glfwGetTime();
    }
        x = cos(posValue) * a;
    y = sin(posValue) * b;
    model = glm::translate(model, glm::vec3(x, y, 0.0));
    model = glm::rotate(model, glm::radians(weight * 100.0f), glm::vec3(0, 0, 1));



    glm::mat4 view(1.0);
    //set camera
    view *= glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

EllipseTransform::EllipseTransform(float a, float b, bool dynamicSpeed) : a(a), b(b), dynamicSpeed(dynamicSpeed) {
    time = static_cast<float>(glfwGetTime());
    posValue = time;
    x = cos(posValue) * a;
    y = sin(posValue) * b;
    pause = false;
}

glm::vec2 EllipseTransform::getPosition() {
    return glm::vec2(x, y);
}

CombinedTransform::CombinedTransform(glm::vec3 rotateAxis, glm::vec3 scale, glm::vec3 translate) :
        rotateAxis(rotateAxis), scale(scale), translate(translate) {
    rotationSpeed = 0.5;
    lastTime = glfwGetTime();
    pause = false;
    posValue = 0;
}

TransformMatrix CombinedTransform::getTransformMatrix() {
    GLuint width, height;
    tie(width, height) = Utils::getScreenSize();
    auto model = getModel();

    glm::mat4 view(1.0);

    //set camera
    view *= glm::lookAt(glm::vec3(0, 0, 10),
                        glm::vec3(0, 0, 0),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

    return toTransformMatrix(model, view, projection);
}

float CombinedTransform::getRotationSpeed() const {
    return rotationSpeed;
}

void CombinedTransform::setRotationSpeed(float rotationSpeed) {
    CombinedTransform::rotationSpeed = rotationSpeed;
}

const glm::vec3 &CombinedTransform::getRotateAxis() const {
    return rotateAxis;
}

void CombinedTransform::setRotateAxis(const glm::vec3 &rotateAxis) {
    CombinedTransform::rotateAxis = rotateAxis;
}

const glm::vec3 &CombinedTransform::getScale() const {
    return scale;
}

void CombinedTransform::setScale(const glm::vec3 &scale) {
    CombinedTransform::scale = scale;
}

const glm::vec3 &CombinedTransform::getTranslate() const {
    return translate;
}

void CombinedTransform::setTranslate(const glm::vec3 &translate) {
    CombinedTransform::translate = translate;
}

float CombinedTransform::getPosValue() const {
    return posValue;
}

void CombinedTransform::setPosValue(float posValue) {
    CombinedTransform::posValue = posValue;
}

float CombinedTransform::getLastTime() const {
    return lastTime;
}

void CombinedTransform::setLastTime(float lastTime) {
    CombinedTransform::lastTime = lastTime;
}

glm::mat4 CombinedTransform::getModel() {
    glm::mat4 model(1.0);
    if (!pause) {
        posValue += glfwGetTime() - lastTime;
    }
    lastTime = glfwGetTime();
    auto weight = static_cast<float>(fmod(posValue * rotationSpeed, 3.6));

    model = glm::scale(model, scale);
    model = glm::translate(model, translate);
    model = glm::rotate(model, glm::radians(weight * 100.0f), rotateAxis);
    return model;
}

TransformMatrix::TransformMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    this->model = model;
    this->view = view;
    this->projection = projection;
}
