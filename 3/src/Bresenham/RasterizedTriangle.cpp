//
// Created by Miguel Chan on 2019-03-22.
//

#include "RasterizedTriangle.h"
#include <iostream>
#include <algorithm>

using namespace std;

void RasterizedTriangle::draw() {
    useShader();

    if (updateWindowSize() || updateParameters()) {
        vector<Bresenham::Point<GLint>> filledPoints;
        int linesSeq[3][3] = {{0, 1, 2},
                              {0, 2, 1},
                              {1, 2, 0}};
        // OpenGL coordinate to display coordinate
        array<Bresenham::Point<GLint>, 3> dispPoints = {{
                                                                {GLint(vertices[0].x * windowWidth / 2),
                                                                        GLint(vertices[0].y * windowHeight / 2)},
                                                                {GLint(vertices[1].x * windowWidth / 2),
                                                                        GLint(vertices[1].y * windowHeight / 2)},
                                                                {GLint(vertices[2].x * windowWidth / 2),
                                                                        GLint(vertices[2].y * windowHeight / 2)},
                                                        }};
        vector<Line> lines;
        lines.reserve(3);
        if (vertices[2].y > 0.72) {
            printf("");
        }

        for (auto seq : linesSeq) {
            auto line = Line(dispPoints[seq[0]], dispPoints[seq[1]], dispPoints[seq[2]]);
            lines.push_back(line);
        }
        BoundingBox box(dispPoints[0], dispPoints[1], dispPoints[2]);
        pointsData.clear();
        for (int x = box.minX; x <= box.maxX; x++) {
            for (int y = box.minY; y <= box.maxY; y++) {
                bool isInside = true;
                for (const auto& line : lines) {
                    if (!line.isInside({x, y})) {
                        isInside = false;
                        break;
                    }
                }
                if (isInside) {
                    filledPoints.push_back({x, y});
                }
            }
        }
        pointsData.clear();
        for (const auto &point : filledPoints) {
            // To opengl coordinate
            pointsData.push_back(static_cast<float>(point.x) * 2 / static_cast<float>(windowWidth));
            pointsData.push_back(static_cast<float>(point.y) * 2 / static_cast<float>(windowHeight));
            // z-axis
            pointsData.push_back(0);
            // color
            pointsData.push_back(r);
            pointsData.push_back(g);
            pointsData.push_back(b);
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pointsData.size() * sizeof(GLfloat),
                 pointsData.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(pointsData.size() / 6));
}

RasterizedTriangle::RasterizedTriangle(GLFWwindow *window,
                                       const std::array<GLfloat, 6> &vertices)
        : BresenhamLine(window, vertices) {}

bool RasterizedTriangle::Line::isInside(Bresenham::Point<GLint> point) const{
    return A * point.x + B * point.y + C <= 0;
}

// Point a and b are points that the line will go through, c is used to check if
// the third point of the triangle is on the right halfspace of the line, if not,
// multiply all parameters by -1.
RasterizedTriangle::Line::Line(Bresenham::Point<GLint> a,
                               Bresenham::Point<GLint> b,
                               Bresenham::Point<GLint> c) {
    A = a.y - b.y;
    B = b.x - a.x;
    C = -(a.x * A + a.y * B);
    if (!isInside(c)) {
        A = -A;
        B = -B;
        C = -C;
    }
}

RasterizedTriangle::BoundingBox::BoundingBox(Bresenham::Point<GLint> a, Bresenham::Point<GLint> b,
                                             Bresenham::Point<GLint> c) {
    minX = min({a.x, b.x, c.x});
    minY = min({a.y, b.y, c.y});
    maxX = max({a.x, b.x, c.x});
    maxY = max({a.y, b.y, c.y});
}
