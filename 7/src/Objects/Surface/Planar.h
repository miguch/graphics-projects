//
// Created by Miguel Chan on 2019-05-07.
//

#ifndef SHADOW_PLANAR_H
#define SHADOW_PLANAR_H

#include "../BaseObject.h"
#include <glm/glm.hpp>
#include "../../Shader/shader.h"
#include "../../Utils/Utils.h"
#include "../../Lighting/Lighting.h"
#include <array>

class Planar : public BaseObject {
public:
    // The Upper-left, upper-right, lower-right and lower-left points.
    explicit Planar(const std::array<glm::vec3, 4> &points,
                    const glm::vec3 &normal,
                    const glm::vec3 &color,
                    const glm::vec3 &lightPos,
                    const glm::vec3 &viewPos);

    void draw(Transform *transformer) override;

    Lighting lighting;

private:
    Shader &shader;
public:
    void setShader(Shader &shader);

private:
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
    std::unique_ptr<Utils::ElementBuffer> elementBuffer;
};


#endif //SHADOW_PLANAR_H
