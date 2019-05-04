//
// Created by Miguel Chan on 2019-03-22.
//

#ifndef DRAWLINE_BRESENHAMCIRCLE_H
#define DRAWLINE_BRESENHAMCIRCLE_H

#include "Bresenham.h"
#include <vector>
#include <array>

class BresenhamCircle : public Bresenham {
private:
    GLfloat r, g, b;
    Point<GLfloat> center;
    GLint radius;
    bool updatedParameters;
    bool updateParameters();
    std::vector<float> pointsData;

public:
    BresenhamCircle(GLFWwindow* window, GLfloat centerX, GLfloat centerY, GLint radius);
    void draw() override;
    void setCenter(GLfloat x, GLfloat y);
    void setRadius(GLint r);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
};


#endif //DRAWLINE_BRESENHAMCIRCLE_H
