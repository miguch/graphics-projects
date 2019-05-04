//
// Created by Miguel Chan on 2019-03-22.
//

#include "BresenhamLine.h"

using namespace std;

bool BresenhamLine::updateParameters() {
    if (updatedParameters) {
        updatedParameters = false;
        return true;
    }
    return false;
}

void BresenhamLine::draw() {
    Bresenham::useShader();
    // Recalculate line only when window size or line parameters have changed
    if (updateWindowSize() || updateParameters()) {
        vector<Point < GLint>>
        linesPoints;
        int linesSeq[3][2] = {{0, 1},
                              {0, 2},
                              {1, 2}};
        for (auto seq : linesSeq) {
            //OpenGL coordinate to display coordinate
            Point <GLint> point1 = {GLint(vertices[seq[0]].x * windowWidth / 2),
                                    GLint(vertices[seq[0]].y * windowHeight / 2)};
            Point <GLint> point2 = {GLint(vertices[seq[1]].x * windowWidth / 2),
                                    GLint(vertices[seq[1]].y * windowHeight / 2)};
            auto line = getBresenhamLine(point1.x, point1.y, point2.x, point2.y);
            linesPoints.insert(end(linesPoints), begin(line), end(line));
        }
        pointsData.clear();
        for (const auto &point: linesPoints) {
            //To opengl coordinate
            pointsData.push_back(static_cast<float>(point.x) / windowWidth * 2);
            pointsData.push_back(static_cast<float>(point.y) / windowHeight * 2);
            // z-axis
            pointsData.push_back(0);
            //color
            pointsData.push_back(r);
            pointsData.push_back(g);
            pointsData.push_back(b);
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pointsData.size() * sizeof(GLfloat), pointsData.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(pointsData.size() / 6));
}

BresenhamLine::BresenhamLine(GLFWwindow *window, const std::array<GLfloat, 6> &vertices) : Bresenham(window) {
    setVertices(vertices);
    setColor(1, 1, 1);
}

void BresenhamLine::setVertices(const std::array<GLfloat, 6> &vertices) {
    for (int i = 0; i < 3; i++) {
        Point <GLfloat> vertex = {vertices[i * 2], vertices[i * 2 + 1]};
        this->vertices[i] = vertex;
    }
    updatedParameters = true;
}


void BresenhamLine::setColor(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
    updatedParameters = true;
}

vector<Bresenham::Point<GLint>> BresenhamLine::getBresenhamLine(GLint x1, GLint y1, GLint x2, GLint y2) {
    vector<Bresenham::Point<GLint>> result;
    // draw line from left to right and slope's absolute value must be smaller than 1
    bool isSteep = abs(y2 - y1) > abs(x2 - x1);
    // if line is steep, rotate the coordinate to draw
    auto putPoint = [&](GLint x, GLint y) {
        if (isSteep) {
            result.push_back({y, x});
        } else {
            result.push_back({x, y});
        }
    };
    if (isSteep) {
        swap(x1, y1);
        swap(x2, y2);
    }
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    GLint deltaX = x2 - x1;
    GLint deltaY = abs(y2 - y1);
    GLint p = 2 * deltaY - deltaX;
    GLint yStep = (y1 < y2) ? 1 : -1;
    GLint pStep1 = 2 * deltaY;
    GLint pStep2 = 2 * (deltaY - deltaX);

    putPoint(x1, y1);

    for (int x = x1 + 1, y = y1; x <= x2; x++) {
        if (p <= 0) {
            p += pStep1;
        } else {
            y += yStep;
            p += pStep2;
        }
        putPoint(x, y);
    }
    return result;
}