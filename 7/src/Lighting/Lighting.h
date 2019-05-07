//
// Created by Miguel Chan on 2019-05-07.
//

#ifndef SHADOW_LIGHTING_H
#define SHADOW_LIGHTING_H


#include <glm/vec3.hpp>
#include "../Shader/shader.h"

class Lighting {
public:
    Lighting(const glm::vec3 &lightColor,
             const glm::vec3 &objectColor,
             const glm::vec3 &lightPos,
             const glm::vec3 &viewPos);

    void use(const Shader *shader);

    static Shader &Phong();

    static Shader &BlinnPhong();

    static Shader &Gouraud();

    // copy lighting colors, position and view position;
    void syncLighting(const Lighting& rhs);
    glm::vec3 lightColor, objectColor;
    glm::vec3 lightPos, viewPos;
    float ambientStrength;
    float specularStrength;
    float diffuseStrength;
    int shininess;
};


#endif //SHADOW_LIGHTING_H
