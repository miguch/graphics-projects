//
// Created by Miguel Chan on 2019-04-23.
//

#ifndef CAMERA_BASECUBE_H
#define CAMERA_BASECUBE_H

#include "Transformation/Transformation.h"
#include "../Shader/shader.h"

class BaseObject {
public:
    virtual void draw(Transform *transformer) {
        drawWithMat(transformer->getTransformMatrix());
    }

    virtual void drawWithMat(const TransformMatrix& matrix) = 0;

    // draw elements without setting shader and matrix
    virtual void putElements() = 0;

    virtual ~BaseObject() = default;
};


#endif //CAMERA_BASECUBE_H
