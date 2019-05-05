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

std::unordered_map<LightingCube::Mode, std::unique_ptr<Shader>> LightingCube::shaders = {

};

void LightingCube::draw(Transform *transformer) {
    Shader *shader = shaders[current].get();
    shader->use();

    auto transform = transformer->getTransformMatrix();
    shader->setMat4("model", transform.model);
    shader->setMat4("projection", transform.projection);
    shader->setMat4("view", transform.view);
    shader->setVec3("objectColor", objectColor);
    shader->setVec3("lightColor", lightColor);
    shader->setFloat("ambientStrength", ambientStrength);
    shader->setVec3("lightPos", lightPos);
    shader->setVec3("viewPos", viewPos);
    shader->setFloat("specularStrength", specularStrength);
    shader->setFloat("diffuseStrength", diffuseStrength);
    shader->setInt("shininess", shininess);

    vertexBuffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

const char *phong_vert_shader_path = "shaders/phong_light_vert.glsl";
const char *phong_frag_shader_path = "shaders/phong_light_frag.glsl";
const char *gouroud_vert_shader_path = "shaders/gouraud_light_vert.glsl";
const char *gouroud_frag_shader_path = "shaders/gouraud_light_frag.glsl";
const char *blinn_phong_vert_shader_path = "shaders/blinn_phong_light_vert.glsl";
const char *blinn_phong_frag_shader_path = "shaders/blinn_phong_light_frag.glsl";

LightingCube::LightingCube(glm::vec3 colors, float length, float width, float height) {
    lightColor = glm::vec3(1, 1, 1);
    objectColor = colors;
    ambientStrength = 0.2;
    diffuseStrength = 1;
    specularStrength = 1;
    viewPos = glm::vec3(0, 0, 0);
    shininess = 32;
    current = PHONG;

    float x = 0.5 * length, y = 0.5 * height, z = 0.5 * width;

    if (!shaders.count(PHONG)) {
        shaders[PHONG] = make_unique<Shader>(phong_vert_shader_path, phong_frag_shader_path);
    }
    if (!shaders.count(GOURAUD)) {
        shaders[GOURAUD] = make_unique<Shader>(gouroud_vert_shader_path, gouroud_frag_shader_path);
    }
    if (!shaders.count(BLINN_PHONG)) {
        shaders[BLINN_PHONG] = make_unique<Shader>(blinn_phong_vert_shader_path, blinn_phong_frag_shader_path);
    }

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


const glm::vec3 &LightingCube::getObjectColor() const {
    return objectColor;
}

void LightingCube::setObjectColor(const glm::vec3 &objectColor) {
    LightingCube::objectColor = objectColor;
}

LightingCube::LightingCube(glm::vec3 colors, std::shared_ptr<Utils::VertexBufferObject> vbo) {
    vertexBuffer = make_unique<Utils::VertexArrayBuffer>(vbo, vertexAttribSetter);
    lightColor = glm::vec3(1, 1, 1);
    objectColor = colors;
    ambientStrength = 0.2;
    diffuseStrength = 1;
    specularStrength = 1;
    viewPos = glm::vec3(0, 0, 0);
    shininess = 32;
    current = PHONG;


    if (!shaders.count(PHONG)) {
        shaders[PHONG] = make_unique<Shader>(phong_vert_shader_path, phong_frag_shader_path);
    }
    if (!shaders.count(GOURAUD)) {
        shaders[GOURAUD] = make_unique<Shader>(gouroud_vert_shader_path, gouroud_frag_shader_path);
    }
    if (!shaders.count(BLINN_PHONG)) {
        shaders[BLINN_PHONG] = make_unique<Shader>(blinn_phong_vert_shader_path, blinn_phong_frag_shader_path);
    }
}

const unique_ptr<Utils::VertexArrayBuffer> &LightingCube::getVertexBuffer() const {
    return vertexBuffer;
}

const glm::vec3 &LightingCube::getLightColor() const {
    return lightColor;
}

void LightingCube::setLightColor(const glm::vec3 &lightColor) {
    LightingCube::lightColor = lightColor;
}

const glm::vec3 &LightingCube::getLightPos() const {
    return lightPos;
}

void LightingCube::setLightPos(const glm::vec3 &lightPos) {
    LightingCube::lightPos = lightPos;
}

const glm::vec3 &LightingCube::getViewPos() const {
    return viewPos;
}

void LightingCube::setViewPos(const glm::vec3 &viewPos) {
    LightingCube::viewPos = viewPos;
}

LightingCube::Mode LightingCube::getCurrent() const {
    return current;
}

void LightingCube::setCurrent(LightingCube::Mode current) {
    LightingCube::current = current;
}


