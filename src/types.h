#pragma once

#include <utility>

#include "GL/glew.h"

class Render;
class Game;

// Common coordinate types
typedef std::pair<GLsizei, GLsizei> Coordw;
typedef std::pair<GLfloat, GLfloat> Coordf;
typedef std::pair<GLint, GLint> Coordi;
