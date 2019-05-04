//
// Created by Miguel Chan on 2019-04-03.
//

#ifndef TRANSFORMATION_CUBE_H
#define TRANSFORMATION_CUBE_H

#include <glad/glad.h>
#include <memory>
#include "../Texture/Texture.h"
#include "../Utils/Utils.h"
#include "../Shader/shader.h"
#include "BaseCube.h"
#include "Transformation.h"

class TextureCube: public BaseCube {
private:
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
    std::unique_ptr<Utils::ElementBuffer> elementBuffer;
    Texture texture;
    //Same shader used by all Cubes
    static std::unique_ptr<Shader> shader;
    float length;
    float width;
    float height;

public:
    explicit TextureCube(const char* texture_path, float length = 1, float width = 1, float height = 1);
    void draw(Transform *transformer) override;
    ~TextureCube();
};


#endif //TRANSFORMATION_CUBE_H
