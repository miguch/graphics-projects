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
#include "Transformation.h"

class Cube {
private:
    std::unique_ptr<Utils::VertexBuffer> vertexBuffer;
    std::unique_ptr<Utils::ElementBuffer> elementBuffer;
    Texture texture;
    //Same shader used by all Cubes
    static std::unique_ptr<Shader> shader;
    float length;
    float width;
    float height;

public:
    explicit Cube(const char* texture_path, float length = 1, float width = 1, float height = 1);
    void draw(Transform *transformer);
    ~Cube();
};


#endif //TRANSFORMATION_CUBE_H
