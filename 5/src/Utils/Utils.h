//
// Created by Miguel Chan on 2019-04-02.
//

#ifndef TRANSFORMATION_DEPTHTEST_H
#define TRANSFORMATION_DEPTHTEST_H

#include <exception>
#include <glad/glad.h>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <string>

namespace Utils {
    void disableDepthTest();
    void enableDepthTest();

    std::pair<GLuint, GLuint> getScreenSize();
    void setScreenSize(GLuint w, GLuint h);

    void printMat4(const glm::mat4& mat);
    void printVec3(const glm::vec3& vec);

    struct VertexBuffer{
        VertexBuffer(int size, float *data,
                     const std::function<void()> &vertexAttribCallback);
        VertexBuffer(const VertexBuffer&) = delete;
        GLuint VAO, VBO;
        void bind();
        ~VertexBuffer();
    };

    struct ElementBuffer{
        GLuint EBO;
        ElementBuffer(GLuint VAO, int size, unsigned int *data);
        ElementBuffer(const ElementBuffer&) = delete;
        ~ElementBuffer();
    };

    class GLToolsException : public std::exception {
    private:
        //Use std::string to copy msg data in order to
        // prevent memory safety issue due to the destruction of original message data
        std::string msg;
    public:
        explicit GLToolsException(const char* msg);
        const char * what() const noexcept override;
    };
};


#endif //TRANSFORMATION_DEPTHTEST_H
