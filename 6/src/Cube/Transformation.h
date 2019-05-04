//
// Created by Miguel Chan on 2019-04-03.
//

#ifndef TRANSFORMATION_TRANSFORMATION_H
#define TRANSFORMATION_TRANSFORMATION_H

#include <glm/glm.hpp>
#include "../Camera/Camera.h"

struct TransformMatrix {
    glm::mat4 model, view, projection;

    TransformMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

class Transform {
public:
    virtual TransformMatrix getTransformMatrix() = 0;

    virtual ~Transform() = default;
};


class StaticCube : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

};

class TranslationCube : public Transform {
private:
    float movingLength = 0.5;
public:
    bool horizontal = true, vertical = false;

    void setMovingLength(float n);

    TransformMatrix getTransformMatrix() override;

    TranslationCube(float movingLength = 0.5);
};

class RotationCube : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

    RotationCube(glm::vec3 axis = glm::vec3(1.0f, 0.0f, 1.0f));

private:
    glm::vec3 axis;
};

class ScalingCube : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

};

class CombinedTransform : public Transform {
public:
    TransformMatrix getTransformMatrix() override;

    CombinedTransform(glm::vec3 rotateAxis, glm::vec3 scale, glm::vec3 translate);

private:
    glm::vec3 rotateAxis, scale, translate;
    float rotationSpeed;
    float posValue, lastTime;
public:
    void setRotationSpeed(float rotationSpeed);
    bool pause;
public:
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

class ViewChangeTransform : public Transform {
private:

public:
    TransformMatrix getTransformMatrix() override;

    ViewChangeTransform();

    int axis;
};

class MovingCamTransform : public Transform {
private:

public:
    TransformMatrix getTransformMatrix() override;

    explicit MovingCamTransform(const Camera &cam);

    Camera cam;
};

#endif //TRANSFORMATION_TRANSFORMATION_H
