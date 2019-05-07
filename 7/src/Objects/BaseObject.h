//
// Created by Miguel Chan on 2019-04-23.
//

#ifndef CAMERA_BASECUBE_H
#define CAMERA_BASECUBE_H

#include "Transformation/Transformation.h"

class BaseObject {
public:
    virtual void draw(Transform *transformer) = 0;

    virtual ~BaseObject() = default;
};


#endif //CAMERA_BASECUBE_H
