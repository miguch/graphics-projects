//
// Created by Miguel Chan on 2019-04-03.
//

#include "Texture.h"
#include "../stb_image.h"
#include "../Utils/Utils.h"
#include <string>
#include <functional>

Texture::Texture(const char *imgPath) {

    if (auto imgData = stbi_load(imgPath, &width, &height, &channels, 0)) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imgData);
    } else {
        std::string errMessage = "Fail to load Texture data from ";
        errMessage += imgPath;
        throw Utils::GLToolsException(errMessage.c_str());
    }

}



Texture::~Texture() {
    glDeleteTextures(1, &textureID);

}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, textureID);

}
