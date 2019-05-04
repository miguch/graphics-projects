//
// Created by Miguel Chan on 2019-04-03.
//

#include "Cube.h"
#include "../Utils/Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <ctime>

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

const char *vert_shader_path = "shaders/texture_vert.glsl";
const char *frag_shader_path = "shaders/texture_frag.glsl";

std::unique_ptr<Shader> Cube::shader = nullptr;

Cube::Cube(const char *texture_path, float length, float width, float height) :
        texture(texture_path), length(length), width(width), height(height) {

    if (!shader) {
        shader = make_unique<Shader>(vert_shader_path, frag_shader_path);
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
    vertexBuffer = make_unique<Utils::VertexBuffer>(sizeof(vertices), vertices, vertexAttribSetter);
//    elementBuffer = make_unique<Utils::ElementBuffer>(vertexBuffer->VAO, sizeof(indices), indices);

}


//Get the transform matrix from transformer and pass to vertex shader to perform transform
void Cube::draw(Transform *transformer) {
    shader->use();
    texture.bind();

    int transformLoc = glGetUniformLocation(shader->getProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformer->getTransformMatrix()));

    vertexBuffer->bind();
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);


}

Cube::~Cube() = default;
