//
// Created by Miguel Chan on 2019-04-10.
//

#ifndef CAMERA_COLOREDCUBE_H
#define CAMERA_COLOREDCUBE_H

#include "../Utils/Utils.h"
#include "../Shader/shader.h"
#include "Transformation.h"
#include "BaseCube.h"
#include <memory>
#include <array>

class ColoredCube: public BaseCube {
private:
    std::unique_ptr<Utils::VertexArrayBuffer> vertexBuffer;
    static std::unique_ptr<Shader> shader;
    float length;
    float width;
    float height;
public:
    explicit ColoredCube(std::array<float, 18> colors, float length = 1, float width = 1, float height = 1);
    void draw(Transform *transformer) override;
    ~ColoredCube();
};


#endif //CAMERA_COLOREDCUBE_H
