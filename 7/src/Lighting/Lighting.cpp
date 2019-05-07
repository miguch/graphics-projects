//
// Created by Miguel Chan on 2019-05-07.
//

#include "Lighting.h"

Lighting::Lighting(const glm::vec3 &lightColor,
                   const glm::vec3 &objectColor,
                   const glm::vec3 &lightPos,
                   const glm::vec3 &viewPos) :
        lightColor(lightColor),
        objectColor(objectColor),
        lightPos(lightPos),
        viewPos(viewPos) {
    ambientStrength = 0.1;
    diffuseStrength = 0.8;
    specularStrength = 0.9;
    shininess = 32;

}

void Lighting::use(const Shader *shader) {
    shader->use();
    shader->setVec3("objectColor", objectColor);
    shader->setVec3("lightColor", lightColor);
    shader->setFloat("ambientStrength", ambientStrength);
    shader->setVec3("lightPos", lightPos);
    shader->setVec3("viewPos", viewPos);
    shader->setFloat("specularStrength", specularStrength);
    shader->setFloat("diffuseStrength", diffuseStrength);
    shader->setInt("shininess", shininess);
}

Shader &Lighting::Phong() {
    static Shader inst(ShaderPaths::phong);
    return inst;
}

Shader &Lighting::BlinnPhong() {
    static Shader inst(ShaderPaths::blinnPhong);
    return inst;
}

Shader &Lighting::Gouraud() {
    static Shader inst(ShaderPaths::gouraud);
    return inst;
}

void Lighting::syncLighting(const Lighting &rhs) {
    lightColor = rhs.lightColor;
    lightPos = rhs.lightPos;
    viewPos = rhs.viewPos;
}
