#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include "painter.h"
#include "display.h"

using namespace std;

namespace painter {

    static GLuint idProgram;

    void initialize() {
        GLuint idVertexArray;
        glGenVertexArrays(1, &idVertexArray);
        glBindVertexArray(idVertexArray);

        GLuint idVertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint idFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        string codeVertexShader;
        ifstream streamVertexShader("../res/default_vert.vert", ios::in);
        if(streamVertexShader.is_open()) {
            stringstream sstr;
            sstr << streamVertexShader.rdbuf();
            codeVertexShader = sstr.str();
            streamVertexShader.close();
        }else throw runtime_error("error initializing vertex shader");

        string codeFragmentShader;
        ifstream streamFragmentShader("../res/default_frag.frag", ios::in);
        if(streamFragmentShader.is_open()) {
            stringstream sstr;
            sstr << streamFragmentShader.rdbuf();
            codeFragmentShader = sstr.str();
            streamFragmentShader.close();
        }else throw runtime_error("error initializing fragment shader");

        char const* pointerVertexCode = codeVertexShader.c_str();
        glShaderSource(idVertexShader, 1, &pointerVertexCode, nullptr);
        glCompileShader(idVertexShader);

        char const* pointerFragmentCode = codeFragmentShader.c_str();
        glShaderSource(idFragmentShader, 1, &pointerFragmentCode, nullptr);
        glCompileShader(idFragmentShader);

        idProgram = glCreateProgram();
        glAttachShader(idProgram, idVertexShader);
        glAttachShader(idProgram, idFragmentShader);
        glLinkProgram(idProgram);

        glDetachShader(idProgram, idVertexShader);
        glDetachShader(idProgram, idFragmentShader);

        glDeleteShader(idVertexShader);
        glDeleteShader(idFragmentShader);
    }

    void draw(Quad& quad, int texture, const Color& color) {
        glUseProgram(idProgram);

        quad.setBufferToDisplay(display::getSize());

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad.getId());
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
    }

}
