//
// Created by Miguel Chan on 2019-05-09.
//

#ifndef SHADOW_SHADOWCUBE_H
#define SHADOW_SHADOWCUBE_H

#include "../BaseObject.h"
#include <unordered_map>
#include "../../Shader/shader.h"
#include "../../Utils/Utils.h"
#include "../../Lighting/Lighting.h"
#include "../../Texture/Texture.h"

class ShadowCube : public BaseObject {
private:
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
    float length;
    float width;
    float height;
    Shader &shader;
    glm::mat4 lightSpaceMatrix;
    GLuint depthMap = 0;
    Texture texture;
public:
    void setShadow(GLuint depthMap, const glm::mat4& lightSpaceMatrix);

    const std::unique_ptr<Utils::VertexArrayBuffer> &getVertexBuffer() const;

    Lighting lighting;

    void setShader(Shader &shader);

    void drawWithMat(const TransformMatrix& matrix) override;

    void putElements() override;

    explicit ShadowCube(glm::vec3 colors, float length = 1, float width = 1, float height = 1);

    ShadowCube(glm::vec3 colors, std::shared_ptr<Utils::VertexBufferObject> vbo);
};


#endif //SHADOW_SHADOWCUBE_H
