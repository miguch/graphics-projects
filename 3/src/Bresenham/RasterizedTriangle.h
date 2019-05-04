//
// Created by Miguel Chan on 2019-03-22.
//

#ifndef DRAWLINE_RASTERIZEDTRIANGLE_H
#define DRAWLINE_RASTERIZEDTRIANGLE_H

#include "Bresenham.h"
#include "BresenhamLine.h"
#include <array>
#include <vector>

class RasterizedTriangle : public BresenhamLine {
private:
    struct BoundingBox {
        GLint minX, minY, maxX, maxY;
        BoundingBox(Point<GLint> a, Point<GLint> b, Point<GLint> c);
    };
    struct Line {
        GLint A, B, C;
        bool isInside(Point<GLint> point) const;
        Line(Point<GLint> a, Point<GLint> b, Point<GLint> c);
    };

public:
    RasterizedTriangle(GLFWwindow* window, const std::array<GLfloat, 6>& vertices);
    void draw() override;
};



#endif //DRAWLINE_RASTERIZEDTRIANGLE_H
