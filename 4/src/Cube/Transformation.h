//
// Created by Miguel Chan on 2019-04-03.
//

#ifndef TRANSFORMATION_TRANSFORMATION_H
#define TRANSFORMATION_TRANSFORMATION_H

#include <glm/glm.hpp>

class Transform {
public:
    virtual glm::mat4 getTransformMatrix() = 0;
    virtual ~Transform() = default;
};

class StaticCube : public Transform {
public:
    glm::mat4 getTransformMatrix() override;

};

class TranslationCube : public Transform {
private:
    float movingLength = 0.5;
public:
    bool horizontal = true, vertical = false;
    void setMovingLength(float n);
    glm::mat4 getTransformMatrix() override;
    TranslationCube(float movingLength = 0.5);
};

class RotationCube: public Transform {
public:
    glm::mat4 getTransformMatrix() override;
    RotationCube(glm::vec3 axis = glm::vec3(1.0f, 0.0f, 1.0f));

private:
    glm::vec3 axis;
};

class ScalingCube : public Transform {
public:
    glm::mat4 getTransformMatrix() override;

};

class CombinedTransform: public Transform {
public:
    glm::mat4 getTransformMatrix() override;
    CombinedTransform(glm::vec3 rotateAxis, glm::vec3 scale, glm::vec3 translate);

private:
    glm::vec3 rotateAxis, scale, translate;
};

class EllipseTransform: public Transform {
public:
    glm::mat4 getTransformMatrix() override;
    EllipseTransform(float a, float b, bool dynamicSpeed=false);
    bool dynamicSpeed;

private:
    float a, b;
    float posValue, time, x, y;


};
#endif //TRANSFORMATION_TRANSFORMATION_H
