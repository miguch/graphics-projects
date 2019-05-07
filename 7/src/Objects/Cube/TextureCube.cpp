//
// Created by Miguel Chan on 2019-04-03.
//

#include "TextureCube.h"

using namespace std;

auto vertexAttribSetter = []() {
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    //Color
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
    //Texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};


std::unique_ptr<Shader> TextureCube::shader = nullptr;

TextureCube::TextureCube(const char *texture_path, float length, float width, float height) :
        texture(texture_path), length(length), width(width), height(height) {

    if (!shader) {
        shader = make_unique<Shader>(ShaderPaths::simpleTexture);
    }

    float x = 0.5 * length, y = 0.5 * height, z = 0.5 * width;

    float vertices[] = {
            -x, -y, -z, 0.0f, 0.0f,
            x, -y, -z, 1.0f, 0.0f,
            x, y, -z, 1.0f, 1.0f,
            x, y, -z, 1.0f, 1.0f,
            -x, y, -z, 0.0f, 1.0f,
            -x, -y, -z, 0.0f, 0.0f,

            -x, -y, z, 0.0f, 0.0f,
            x, -y, z, 1.0f, 0.0f,
            x, y, z, 1.0f, 1.0f,
            x, y, z, 1.0f, 1.0f,
            -x, y, z, 0.0f, 1.0f,
            -x, -y, z, 0.0f, 0.0f,

            -x, y, z, 1.0f, 0.0f,
            -x, y, -z, 1.0f, 1.0f,
            -x, -y, -z, 0.0f, 1.0f,
            -x, -y, -z, 0.0f, 1.0f,
            -x, -y, z, 0.0f, 0.0f,
            -x, y, z, 1.0f, 0.0f,

            x, y, z, 1.0f, 0.0f,
            x, y, -z, 1.0f, 1.0f,
            x, -y, -z, 0.0f, 1.0f,
            x, -y, -z, 0.0f, 1.0f,
            x, -y, z, 0.0f, 0.0f,
            x, y, z, 1.0f, 0.0f,

            -x, -y, -z, 0.0f, 1.0f,
            x, -y, -z, 1.0f, 1.0f,
            x, -y, z, 1.0f, 0.0f,
            x, -y, z, 1.0f, 0.0f,
            -x, -y, z, 0.0f, 0.0f,
            -x, -y, -z, 0.0f, 1.0f,

            -x, y, -z, 0.0f, 1.0f,
            x, y, -z, 1.0f, 1.0f,
            x, y, z, 1.0f, 0.0f,
            x, y, z, 1.0f, 0.0f,
            -x, y, z, 0.0f, 0.0f,
            -x, y, -z, 0.0f, 1.0f
    };
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(sizeof(vertices), vertices, vertexAttribSetter);
//    elementBuffer = make_unique<Utils::ElementBuffer>(vertexBuffer->VAO, sizeof(indices), indices);

}


//Get the transform matrix from transformer and pass to vertex shader to perform transform
void TextureCube::draw(Transform *transformer) {
    shader->use();
    texture.bind();
    auto transform = transformer->getTransformMatrix();
    shader->setMat4("model", transform.model);
    shader->setMat4("projection", transform.projection);
    shader->setMat4("view", transform.view);
    vertexBuffer->bind();
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);


}

TextureCube::~TextureCube() = default;
