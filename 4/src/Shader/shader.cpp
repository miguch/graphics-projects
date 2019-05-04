//
// Created by Miguel Chan on 2019-03-06.
//

#include "shader.h"

#include "../Utils/Utils.h"

using namespace Utils;
using namespace std;

Shader::Shader(const char *vertexPath, const char *fragPath) {
    ifstream vFile(vertexPath), fFile(fragPath);

    if (vFile.fail()) {
        throw ("cannot open vertex shader file");
    }
    if (fFile.fail()) {
        throw GLToolsException("cannot open fragment shader file");
    }

    stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    string vCode(vStream.str()), fCode(fStream.str());
    vFile.close();
    fFile.close();

    GLuint vertex = createShader(vertexPath, vCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragment = createShader(fragPath, fCode.c_str(), GL_FRAGMENT_SHADER);

    programId = glCreateProgram();
    glAttachShader(programId, vertex);
    glAttachShader(programId, fragment);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(programId, 512, nullptr, log);
        stringstream errStream;
        errStream << "link error: " << log;
        string x = errStream.str();
        throw GLToolsException(x.c_str());
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


GLuint Shader::createShader(const char * name, const char *code, GLenum shaderType) {
    GLint success;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        stringstream errStream;
        errStream << name << " compile error: " << log;
        string x = errStream.str();
        throw GLToolsException(x.c_str());
    }
    return shader;
}

void Shader::use() {
    glUseProgram(programId);
}

GLuint Shader::getProgram() {
    return programId;
}

