//
// Created by Miguel Chan on 2019-03-22.
//

#include "BresenhamCircle.h"

using namespace std;

vector<BresenhamCircle::Point<GLint>> getCirclePoints(GLint cx, GLint cy, GLint radius) {
    GLint d = 1 - radius;

    vector<BresenhamCircle::Point<GLint>> result;
    //Draw 8 parts of the circle
    auto putPoints = [&](GLint dx, GLint dy) {
        result.push_back({cx + dx, cy + dy});
        result.push_back({cx + dx, cy - dy});
        result.push_back({cx - dx, cy + dy});
        result.push_back({cx - dx, cy - dy});
        result.push_back({cx + dy, cy + dx});
        result.push_back({cx + dy, cy - dx});
        result.push_back({cx - dy, cy + dx});
        result.push_back({cx - dy, cy - dx});
    };
    for (int x = 0, y = radius; x <= y; x++) {
        if (d >= 0) {
            y--;
            d += 2 * (x - y) + 5;
        } else {
            d += 2 * x + 3;
        }
        putPoints(x, y);
    }
    return result;
}


void BresenhamCircle::draw() {
    Bresenham::useShader();
    if (updateWindowSize() || updateParameters()) {
        //To pixel coordinate
        int cx = center.x * windowWidth / 2, cy = center.y * windowHeight / 2;
        auto points = getCirclePoints(cx, cy, radius);
        pointsData.clear();
        for (const auto &point : points) {
            pointsData.push_back(static_cast<float>(point.x) / windowWidth * 2);
            pointsData.push_back(static_cast<float>(point.y) / windowHeight * 2);
            pointsData.push_back(0);
            pointsData.push_back(r);
            pointsData.push_back(g);
            pointsData.push_back(b);
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pointsData.size() * sizeof(GLfloat), pointsData.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, pointsData.size() / 6);
}

BresenhamCircle::BresenhamCircle(GLFWwindow *window, GLfloat centerX, GLfloat centerY, GLint radius) : Bresenham(
        window) {
    setCenter(centerX, centerY);
    setRadius(radius);
    setColor(1, 1, 1);
}

void BresenhamCircle::setRadius(GLint r) {
    updatedParameters = true;
    radius = r;
}

void BresenhamCircle::setCenter(GLfloat x, GLfloat y) {
    updatedParameters = true;
    center = {x, y};
}

bool BresenhamCircle::updateParameters() {
    if (updatedParameters) {
        updatedParameters = false;
        return true;
    }
    return false;
}

void BresenhamCircle::setColor(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
    updatedParameters = true;
}
