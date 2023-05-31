#pragma once

#include <utility>

#include <GL/glew.h>

namespace display {

    void initialize();
    void preUpdate();
    void postUpdate();
    void close();

    bool isExiting();

    const std::pair<GLuint, GLuint>& getSize();
    const std::pair<GLfloat, GLfloat>& getCursor();

}
