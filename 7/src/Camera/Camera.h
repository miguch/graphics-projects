//
// Created by Miguel Chan on 2019-04-10.
//

#ifndef CAMERA_CAMERA_H
#define CAMERA_CAMERA_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

class Camera {
public:

    void moveForward(GLfloat distance);

    void moveBack(GLfloat distance);

    void moveRight(GLfloat distance);

    void moveLeft(GLfloat distance);

    void eulerRotate(GLfloat pitch, GLfloat yaw);

    glm::mat4 getViewMatrix();

    void setCameraPos(const glm::vec3 &cameraPos);

    const glm::vec3 &getCameraPos() const;

    glm::vec3 getCameraFront() const;

    glm::vec3 getTarget() const;

    void setTarget(const glm::vec3& target);

    glm::mat4 getProjectionMatrix();

    explicit Camera(GLFWwindow *window);

    void processInput();

    bool usingMouse();

    void setUsingMouse(bool b);

    glm::mat4 getLookAt();

    float speed;
    float mouseSensetivity;
private:
    GLfloat pfov, pratio, pnear, pfar;
    glm::vec3 cameraPos;
    glm::vec3 target;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    GLFWwindow *window;
    bool useMouse;
    float lastTime;

    //(pitch, yaw, roll)
    glm::vec3 current_euler;
    double lastCursor_x, lastCursor_y;
};


#endif //CAMERA_CAMERA_H
