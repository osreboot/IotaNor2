#pragma once

#include <functional>

#include <GL/glew.h>

class Shader {

private:
    GLuint idProgram;
    std::function<void(const GLuint&)> funcCustomParams;

public:
    Shader(Shader const&) = delete;
    Shader& operator=(Shader const&) = delete;

    Shader(const char* pathVertex, const char* pathFragment, const std::function<void(const GLuint&)>& funcCustomParams);
    ~Shader();

    void applyCustomParameters() const { funcCustomParams(idProgram); }

    const GLuint& getProgramId() const { return idProgram; }

};
