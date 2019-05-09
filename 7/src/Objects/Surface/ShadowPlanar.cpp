//
// Created by Miguel Chan on 2019-05-08.
//

#include "ShadowPlanar.h"
#include "../../Shadow/ShadowMapping.h"

using namespace std;

ShadowPlanar::ShadowPlanar(const std::array<glm::vec3, 4> &points,
                           const glm::vec3 &normal,
                           const glm::vec3 &color,
                           const glm::vec3 &lightPos,
                           const glm::vec3 &viewPos) :
        lighting(glm::vec3(1, 1, 1), color, lightPos, viewPos),
        shader(ShadowMapping::getObjectShader()) {
    initPoints(points, normal);
}

auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

void ShadowPlanar::initPoints(const std::array<glm::vec3, 4> &points, const glm::vec3 &normal) {
    float length = fmax(abs(points[0].x - points[1].x),
                        fmax(abs(points[0].y - points[2].y), abs(points[0].z - points[3].z)));
    float vertices[] = {
            points[0].x, points[0].y, points[0].z, normal.x, normal.y, normal.z, 0, length,
            points[1].x, points[1].y, points[1].z, normal.x, normal.y, normal.z, length, length,
            points[2].x, points[2].y, points[2].z, normal.x, normal.y, normal.z, length, 0,
            points[3].x, points[3].y, points[3].z, normal.x, normal.y, normal.z, 0, 0
    };
    unsigned int indices[] = {
            0, 1, 3, 1, 3, 2
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);
    elementBuffer = make_unique<Utils::ElementBuffer>(vertexBuffer->getVAO(), sizeof(indices), indices);

}


void ShadowPlanar::putElements() {
    vertexBuffer->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ShadowPlanar::drawWithMat(const TransformMatrix &transform) {
    shader.use();

    auto location = glGetUniformLocation(shader.getProgram(), "shadowMap");
    glUniform1i(location, 0);
    location = glGetUniformLocation(shader.getProgram(), "objectTex");
    glUniform1i(location, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    if (texture) {
        glActiveTexture(GL_TEXTURE1);
        texture->bind();
        shader.setBool("useTexture", true);
    } else {
        shader.setBool("useTexture", false);
    }
    shader.setMat4("model", transform.model);
    shader.setMat4("projection", transform.projection);
    shader.setMat4("view", transform.view);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    lighting.use(&shader);

    vertexBuffer->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ShadowPlanar::setShadow(GLuint depthMap, const glm::mat4 &lightSpaceMatrix) {
    ShadowPlanar::depthMap = depthMap;
    this->lightSpaceMatrix = lightSpaceMatrix;
}

ShadowPlanar::ShadowPlanar(const std::array<glm::vec3, 4> &points,
                           const glm::vec3 &normal,
                           const char *texturePath,
                           const glm::vec3 &lightPos,
                           const glm::vec3 &viewPos) :
        lighting(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), lightPos, viewPos),
        shader(ShadowMapping::getObjectShader()) {
    initPoints(points, normal);
    texture = make_unique<Texture>(texturePath);
}
