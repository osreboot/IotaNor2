#pragma once

#include <utility>

#include <GL/glew.h>

using namespace std;

namespace display {

    void initialize();
    void preUpdate();
    void postUpdate();
    void close();

    bool isExiting();

    const pair<GLuint, GLuint>& getSize();

}
