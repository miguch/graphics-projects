//
// Created by Miguel Chan on 2019-04-23.
//

#ifndef CAMERA_LIGHTINGCUBE_H
#define CAMERA_LIGHTINGCUBE_H


#include "../BaseObject.h"
#include <unordered_map>
#include "../../Shader/shader.h"
#include "../../Utils/Utils.h"
#include "../../Lighting/Lighting.h"


class LightingCube : public BaseObject {
private:
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
public:
    const std::unique_ptr<Utils::VertexArrayBuffer> &getVertexBuffer() const;

    Lighting lighting;
private:
    float length;
    float width;
    float height;
    Shader &shader;
public:
    void setShader(Shader &shader);

    void draw(Transform *transformer);

    explicit LightingCube(glm::vec3 colors, float length = 1, float width = 1, float height = 1);

    LightingCube(glm::vec3 colors, std::shared_ptr<Utils::VertexBufferObject> vbo);
};


#endif //CAMERA_LIGHTINGCUBE_H
