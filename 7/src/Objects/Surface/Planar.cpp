//
// Created by Miguel Chan on 2019-05-07.
//

#include "Planar.h"


using namespace std;


auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

Planar::Planar(const std::array<glm::vec3, 4> &points,
               const glm::vec3 &normal,
               const glm::vec3 &color,
               const glm::vec3 &lightPos,
               const glm::vec3 &viewPos) :
        lighting(glm::vec3(1, 1, 1), color, lightPos, viewPos),
        shader(Lighting::BlinnPhong()){
    initPoints(points, normal);
}

void Planar::drawWithMat(const TransformMatrix& transform) {
    shader.use();

    shader.setMat4("model", transform.model);
    shader.setMat4("projection", transform.projection);
    shader.setMat4("view", transform.view);

    lighting.use(&shader);

    vertexBuffer->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Planar::setShader(Shader &shader) {
    Planar::shader = shader;
}

void Planar::putElements() {
    vertexBuffer->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Planar::initPoints(const std::array<glm::vec3, 4> &points, const glm::vec3 &normal) {
    float vertices[] = {
            points[0].x, points[0].y, points[0].z, normal.x, normal.y, normal.z,
            points[1].x, points[1].y, points[1].z, normal.x, normal.y, normal.z,
            points[2].x, points[2].y, points[2].z, normal.x, normal.y, normal.z,
            points[3].x, points[3].y, points[3].z, normal.x, normal.y, normal.z,
    };
    unsigned int indices[] = {
            0, 1, 3, 1, 3, 2
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);
    elementBuffer = make_unique<Utils::ElementBuffer>(vertexBuffer->getVAO(), sizeof(indices), indices);

}

