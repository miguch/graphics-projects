//
// Created by Miguel Chan on 2019-05-09.
//

#include "ShadowCube.h"
#include "../../Shadow/ShadowMapping.h"

using namespace std;

auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

void ShadowCube::drawWithMat(const TransformMatrix &transform) {
    shader.use();

    shader.setMat4("model", transform.model);
    shader.setMat4("projection", transform.projection);
    shader.setMat4("view", transform.view);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setBool("useTexture", false);

    auto location = glGetUniformLocation(shader.getProgram(), "shadowMap");
    glUniform1i(location, 0);
    location = glGetUniformLocation(shader.getProgram(), "objectTex");
    glUniform1i(location, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    lighting.use(&shader);
    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

ShadowCube::ShadowCube(glm::vec3 colors, float length, float width, float height) :
        lighting(glm::vec3(1, 1, 1), colors, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
        shader(ShadowMapping::getObjectShader()),
        texture("resources/GreyOak.jpg") {

    float x = 0.5 * length, y = 0.5 * height, z = 0.5 * width;

    float vertices[] = {
            -x, -y, -z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            x, -y, -z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            x, y, -z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            x, y, -z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -x, y, -z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -x, -y, -z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            -x, -y, z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            x, -y, z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            x, y, z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            x, y, z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -x, y, z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -x, -y, z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -x, y, z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -x, y, -z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -x, -y, -z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -x, -y, -z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -x, -y, z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -x, y, z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            x, y, z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            x, y, -z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            x, -y, -z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            x, -y, -z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            x, -y, z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

            -x, -y, -z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            x, -y, -z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            x, -y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            x, -y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -x, -y, z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -x, -y, -z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -x, y, -z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            x, y, -z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -x, y, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -x, y, -z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);

}


ShadowCube::ShadowCube(glm::vec3 colors, std::shared_ptr<Utils::VertexBufferObject> vbo)
        : lighting(glm::vec3(1, 1, 1), colors, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
          shader(ShadowMapping::getObjectShader()),
          texture("resources/GreyOak.jpg") {
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(vbo, vertexAttribSetter);

}


const std::unique_ptr<Utils::VertexArrayBuffer> &ShadowCube::getVertexBuffer() const {
    return vertexBuffer;
}

void ShadowCube::setShader(Shader &shader) {
    ShadowCube::shader = shader;
}

void ShadowCube::putElements() {
    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ShadowCube::setShadow(GLuint depthMap, const glm::mat4 &lightSpaceMatrix) {
    ShadowCube::depthMap = depthMap;
    this->lightSpaceMatrix = lightSpaceMatrix;
}
