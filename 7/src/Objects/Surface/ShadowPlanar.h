//
// Created by Miguel Chan on 2019-05-08.
//

#ifndef SHADOW_SHADOWPLANAR_H
#define SHADOW_SHADOWPLANAR_H


#include <glm/vec3.hpp>
#include "../Transformation/Transformation.h"
#include "../BaseObject.h"
#include "../../Lighting/Lighting.h"
#include "../../Utils/Utils.h"
#include "Planar.h"
#include "../../Texture/Texture.h"

class ShadowPlanar : public BaseObject {
public:
    // The Upper-left, upper-right, lower-right and lower-left points.
    explicit ShadowPlanar(const std::array<glm::vec3, 4> &points,
                          const glm::vec3 &normal,
                          const glm::vec3 &color,
                          const glm::vec3 &lightPos,
                          const glm::vec3 &viewPos);

    explicit ShadowPlanar(const std::array<glm::vec3, 4> &points,
                          const glm::vec3 &normal,
                          const char* texturePath,
                          const glm::vec3 &lightPos,
                          const glm::vec3 &viewPos);
    Lighting lighting;

private:
    void initPoints(const std::array<glm::vec3, 4> &points, const glm::vec3 &normal);

    GLuint depthMap = 0;
    glm::mat4 lightSpaceMatrix;
    Shader &shader;
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
    std::unique_ptr<Utils::ElementBuffer> elementBuffer;

    std::unique_ptr<Texture> texture;

public:
    void putElements() override;

    void setShadow(GLuint depthMap, const glm::mat4& lightSpaceMatrix);

    void drawWithMat(const TransformMatrix &matrix) override;
};


#endif //SHADOW_SHADOWPLANAR_H
