//
// Created by Miguel Chan on 2019-04-03.
//

#ifndef TRANSFORMATION_TEXTURE_H
#define TRANSFORMATION_TEXTURE_H

#include <glad/glad.h>

class Texture {
private:
    GLuint textureID;
    int width, height, channels;
public:
    explicit Texture(const char* imgPath);
    ~Texture();
    void bind();

};


#endif //TRANSFORMATION_TEXTURE_H
