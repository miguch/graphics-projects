//
// Created by Miguel Chan on 2019-03-22.
//

#ifndef DRAWLINE_BRESENHAMLINE_H
#define DRAWLINE_BRESENHAMLINE_H

#include "Bresenham.h"
#include <vector>
#include <array>

//Used to draw the outline of a given triangle
class BresenhamLine : public Bresenham {
protected:
    GLfloat r, g, b;
    std::array<Point<GLfloat>, 3> vertices;
    std::vector<float> pointsData;
    bool updatedParameters;
    bool updateParameters();
    std::vector<Bresenham::Point<GLint>> getBresenhamLine(GLint x1, GLint y1, GLint x2, GLint y2);

public:
    BresenhamLine(GLFWwindow* window, const std::array<GLfloat, 6>& vertices);
    void draw() override;
    void setVertices(const std::array<GLfloat, 6>& vertices);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
};


#endif //DRAWLINE_BRESENHAMLINE_H
