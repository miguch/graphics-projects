//
// Created by Miguel Chan on 2019-05-08.
//

#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(unsigned int width, unsigned int height) : width(width), height(height) {
    perspective = {glm::radians(45.0f), float(width) / float(height), 0.1, 100.0f};
    ortho = {-10, 10, -10, 10, -20, 20};

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    FBO.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    FBO.unbind();

    mode = Ortho;
}

void ShadowMapping::render(std::function<void()> renderFunc,
                           const glm::mat4& lightSpaceMat) {
    GLint prevViewPort[4];
    glGetIntegerv(GL_VIEWPORT, prevViewPort);

    getDepthMapShader().use();
    getDepthMapShader().setMat4("lightSpaceMatrix", lightSpaceMat);

    glCullFace(GL_FRONT);
    // Render depth map
    glViewport(0, 0, width, height);
    FBO.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    renderFunc();
    FBO.unbind();
    glCullFace(GL_BACK);

    // Reset viewport, render normal scene after exiting the function
    glViewport(prevViewPort[0], prevViewPort[1], prevViewPort[2], prevViewPort[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Shader &ShadowMapping::getDepthMapShader() {
    static Shader inst(ShaderPaths::simpleDepth);
    return inst;
}

void ShadowMapping::setModel(const glm::mat4 &model) {
    getDepthMapShader().setMat4("model", model);
}

ShadowMapping::~ShadowMapping() {
    glDeleteTextures(1, &depthMap);
}

void ShadowMapping::bindDepthMap() {
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

Shader &ShadowMapping::getObjectShader() {
    static Shader inst(ShaderPaths::shadowMap);
    return inst;
}

glm::mat4 ShadowMapping::getLightSpaceMat(const glm::vec3& source, const glm::vec3& center) {
    auto lightProjection = mode == Ortho ? ortho.mat() : perspective.mat();
    auto lightView = glm::lookAt(source, center, glm::vec3(0, 1.0, 0));
    auto lightSpaceMat = lightProjection * lightView;
    return lightSpaceMat;
}

GLuint ShadowMapping::getDepthMap() const {
    return depthMap;
}
