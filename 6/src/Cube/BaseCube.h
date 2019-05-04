//
// Created by Miguel Chan on 2019-04-23.
//

#ifndef CAMERA_BASECUBE_H
#define CAMERA_BASECUBE_H

#include "Transformation.h"

class BaseCube {
public:
    virtual void draw(Transform *transformer) = 0;
    virtual ~BaseCube() = default;
};


#endif //CAMERA_BASECUBE_H
