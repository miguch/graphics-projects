//
// Created by Miguel Chan on 2019-05-08.
//

#ifndef SHADOW_SHADOWMAPPING_H
#define SHADOW_SHADOWMAPPING_H


#include <glm/gtc/matrix_transform.hpp>
#include "../Utils/Utils.h"
#include "../Shader/shader.h"

class ShadowMapping {
private:
    Utils::FrameBuffer FBO;
    GLuint depthMap;
public:
    GLuint getDepthMap() const;

    Utils::PerspectiveParam perspective;
    Utils::OrthoParam ortho;

    ShadowMapping(unsigned int width, unsigned int height);

    int width, height;

    // Render func should configure models and draw all needed objects
    void render(std::function<void()> renderFunc,
                const glm::mat4 & lightSpaceMat);

    glm::mat4 getLightSpaceMat(const glm::vec3& source, const glm::vec3& center);
    void setModel(const glm::mat4& model);
    static Shader &getDepthMapShader();

    static Shader &getObjectShader();

    void bindDepthMap();

    enum ShadowProjection { Ortho, Perspective } mode;

    ~ShadowMapping();
};


#endif //SHADOW_SHADOWMAPPING_H
