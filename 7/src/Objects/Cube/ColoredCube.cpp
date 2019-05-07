//
// Created by Miguel Chan on 2019-04-10.
//

#include "ColoredCube.h"

ColoredCube::~ColoredCube() = default;

using namespace std;

void ColoredCube::draw(Transform *transformer) {
    shader->use();

    auto transform = transformer->getTransformMatrix();
    shader->setMat4("model", transform.model);
    shader->setMat4("projection", transform.projection);
    shader->setMat4("view", transform.view);

    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

std::unique_ptr<Shader> ColoredCube::shader = nullptr;

ColoredCube::ColoredCube(std::array<float, 18> colors, float length, float width, float height) :
        length(length), width(width), height(height) {
    if (!shader) {
        shader = make_unique<Shader>(ShaderPaths::simpleColor);
    }
    float x = 0.5 * length, y = 0.5 * height, z = 0.5 * width;

    float vertices[] = {
            -x, -y, -z, colors[0], colors[1], colors[2],
            x, -y, -z, colors[0], colors[1], colors[2],
            x, y, -z, colors[0], colors[1], colors[2],
            x, y, -z, colors[0], colors[1], colors[2],
            -x, y, -z, colors[0], colors[1], colors[2],
            -x, -y, -z, colors[0], colors[1], colors[2],

            -x, -y, z, colors[3], colors[4], colors[5],
            x, -y, z, colors[3], colors[4], colors[5],
            x, y, z, colors[3], colors[4], colors[5],
            x, y, z, colors[3], colors[4], colors[5],
            -x, y, z, colors[3], colors[4], colors[5],
            -x, -y, z, colors[3], colors[4], colors[5],

            -x, y, z, colors[6], colors[7], colors[8],
            -x, y, -z, colors[6], colors[7], colors[8],
            -x, -y, -z, colors[6], colors[7], colors[8],
            -x, -y, -z, colors[6], colors[7], colors[8],
            -x, -y, z, colors[6], colors[7], colors[8],
            -x, y, z, colors[6], colors[7], colors[8],

            x, y, z, colors[9], colors[10], colors[11],
            x, y, -z, colors[9], colors[10], colors[11],
            x, -y, -z, colors[9], colors[10], colors[11],
            x, -y, -z, colors[9], colors[10], colors[11],
            x, -y, z, colors[9], colors[10], colors[11],
            x, y, z, colors[9], colors[10], colors[11],

            -x, -y, -z, colors[12], colors[13], colors[14],
            x, -y, -z, colors[12], colors[13], colors[14],
            x, -y, z, colors[12], colors[13], colors[14],
            x, -y, z, colors[12], colors[13], colors[14],
            -x, -y, z, colors[12], colors[13], colors[14],
            -x, -y, -z, colors[12], colors[13], colors[14],

            -x, y, -z, colors[15], colors[16], colors[17],
            x, y, -z, colors[15], colors[16], colors[17],
            x, y, z, colors[15], colors[16], colors[17],
            x, y, z, colors[15], colors[16], colors[17],
            -x, y, z, colors[15], colors[16], colors[17],
            -x, y, -z, colors[15], colors[16], colors[17],
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);
}
