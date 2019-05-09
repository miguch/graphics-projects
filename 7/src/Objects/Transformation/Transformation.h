//
// Created by Miguel Chan on 2019-04-03.
//

#ifndef TRANSFORMATION_TRANSFORMATION_H
#define TRANSFORMATION_TRANSFORMATION_H

#include <glm/glm.hpp>
#include "../../Camera/Camera.h"

struct TransformMatrix {
    glm::mat4 model, view, projection;

    TransformMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

class Transform {
public:
    virtual TransformMatrix getTransformMatrix() = 0;

    virtual glm::mat4 getModel() = 0;

    virtual ~Transform() = default;
};


class CombinedTransform : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

    CombinedTransform(glm::vec3 rotateAxis, glm::vec3 scale, glm::vec3 translate);

    glm::mat4 getModel() override;

private:
    glm::vec3 rotateAxis, scale, translate;
    float rotationSpeed;
    float posValue, lastTime;
public:
    const glm::vec3 &getRotateAxis() const;

    void setRotateAxis(const glm::vec3 &rotateAxis);

    const glm::vec3 &getScale() const;

    void setScale(const glm::vec3 &scale);

    const glm::vec3 &getTranslate() const;

    void setTranslate(const glm::vec3 &translate);

    float getPosValue() const;

    void setPosValue(float posValue);

    float getLastTime() const;

    void setLastTime(float lastTime);

    void setRotationSpeed(float rotationSpeed);

    bool pause;

    float getRotationSpeed() const;

};

class EllipseTransform : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

    EllipseTransform(float a, float b, bool dynamicSpeed = false);

    bool dynamicSpeed, pause;

    glm::vec2 getPosition();

private:
    float a, b;
    float posValue, time, x, y;
};


class OrthoTransform : public Transform {
private:

public:
    TransformMatrix getTransformMatrix() override;

    OrthoTransform(float left, float right, float bottom, float top, float zNear, float zFar);

    float left, right, bottom, top, zNear, zFar;
};

class PerspectiveTransform : public Transform {
private:

public:
    TransformMatrix getTransformMatrix() override;

    PerspectiveTransform(float fovy_degree, float aspect, float zNear, float zFar);

    float fovy_degree, aspect, zNear, zFar;
};

#endif //TRANSFORMATION_TRANSFORMATION_H
