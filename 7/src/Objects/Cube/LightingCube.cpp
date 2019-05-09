//
// Created by Miguel Chan on 2019-04-23.
//

#include <glm/ext.hpp>
#include "LightingCube.h"

using namespace std;


auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

void LightingCube::drawWithMat(const TransformMatrix& transform) {
    shader.use();

    shader.setMat4("model", transform.model);
    shader.setMat4("projection", transform.projection);
    shader.setMat4("view", transform.view);

    lighting.use(&shader);
    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

LightingCube::LightingCube(glm::vec3 colors, float length, float width, float height) :
        lighting(glm::vec3(1, 1, 1), colors, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
        shader(Lighting::BlinnPhong()) {

    float x = 0.5 * length, y = 0.5 * height, z = 0.5 * width;

    float vertices[] = {
            -x, -y, -z, 0.0f, 0.0f, -1.0f,
            x, -y, -z, 0.0f, 0.0f, -1.0f,
            x, y, -z, 0.0f, 0.0f, -1.0f,
            x, y, -z, 0.0f, 0.0f, -1.0f,
            -x, y, -z, 0.0f, 0.0f, -1.0f,
            -x, -y, -z, 0.0f, 0.0f, -1.0f,

            -x, -y, z, 0.0f, 0.0f, 1.0f,
            x, -y, z, 0.0f, 0.0f, 1.0f,
            x, y, z, 0.0f, 0.0f, 1.0f,
            x, y, z, 0.0f, 0.0f, 1.0f,
            -x, y, z, 0.0f, 0.0f, 1.0f,
            -x, -y, z, 0.0f, 0.0f, 1.0f,

            -x, y, z, -1.0f, 0.0f, 0.0f,
            -x, y, -z, -1.0f, 0.0f, 0.0f,
            -x, -y, -z, -1.0f, 0.0f, 0.0f,
            -x, -y, -z, -1.0f, 0.0f, 0.0f,
            -x, -y, z, -1.0f, 0.0f, 0.0f,
            -x, y, z, -1.0f, 0.0f, 0.0f,

            x, y, z, 1.0f, 0.0f, 0.0f,
            x, y, -z, 1.0f, 0.0f, 0.0f,
            x, -y, -z, 1.0f, 0.0f, 0.0f,
            x, -y, -z, 1.0f, 0.0f, 0.0f,
            x, -y, z, 1.0f, 0.0f, 0.0f,
            x, y, z, 1.0f, 0.0f, 0.0f,

            -x, -y, -z, 0.0f, -1.0f, 0.0f,
            x, -y, -z, 0.0f, -1.0f, 0.0f,
            x, -y, z, 0.0f, -1.0f, 0.0f,
            x, -y, z, 0.0f, -1.0f, 0.0f,
            -x, -y, z, 0.0f, -1.0f, 0.0f,
            -x, -y, -z, 0.0f, -1.0f, 0.0f,

            -x, y, -z, 0.0f, 1.0f, 0.0f,
            x, y, -z, 0.0f, 1.0f, 0.0f,
            x, y, z, 0.0f, 1.0f, 0.0f,
            x, y, z, 0.0f, 1.0f, 0.0f,
            -x, y, z, 0.0f, 1.0f, 0.0f,
            -x, y, -z, 0.0f, 1.0f, 0.0f,
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);

}


LightingCube::LightingCube(glm::vec3 colors, std::shared_ptr<Utils::VertexBufferObject> vbo)
        : lighting(glm::vec3(1, 1, 1), colors, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
          shader(Lighting::BlinnPhong()) {
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(vbo, vertexAttribSetter);

}


const unique_ptr<Utils::VertexArrayBuffer> &LightingCube::getVertexBuffer() const {
    return vertexBuffer;
}

void LightingCube::setShader(Shader &shader) {
    LightingCube::shader = shader;
}

void LightingCube::putElements() {
    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}



