
#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"

using namespace std;

Shader::Shader(const char *pathVertex, const char *pathFragment, const function<void(const GLuint&)>& funcCustomParams) :
        funcCustomParams(funcCustomParams) {
    GLuint idVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint idFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    string codeVertexShader;
    ifstream streamVertexShader(pathVertex, ios::in);
    if(streamVertexShader.is_open()) {
        stringstream sstr;
        sstr << streamVertexShader.rdbuf();
        codeVertexShader = sstr.str();
        streamVertexShader.close();
    }else throw runtime_error("error initializing vertex shader");

    string codeFragmentShader;
    ifstream streamFragmentShader(pathFragment, ios::in);
    if(streamFragmentShader.is_open()) {
        stringstream sstr;
        sstr << streamFragmentShader.rdbuf();
        codeFragmentShader = sstr.str();
        streamFragmentShader.close();
    }else throw runtime_error("error initializing fragment shader");

    GLint compileResult = GL_FALSE;
    int compileLogLength;

    char const* pointerVertexCode = codeVertexShader.c_str();
    glShaderSource(idVertexShader, 1, &pointerVertexCode, nullptr);
    glCompileShader(idVertexShader);

    glGetShaderiv(idVertexShader, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(idVertexShader, GL_INFO_LOG_LENGTH, &compileLogLength);
    if(compileLogLength > 0) {
        vector<char> errorMessage(compileLogLength + 1);
        glGetShaderInfoLog(idVertexShader, compileLogLength, nullptr, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }

    char const* pointerFragmentCode = codeFragmentShader.c_str();
    glShaderSource(idFragmentShader, 1, &pointerFragmentCode, nullptr);
    glCompileShader(idFragmentShader);

    glGetShaderiv(idFragmentShader, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(idFragmentShader, GL_INFO_LOG_LENGTH, &compileLogLength);
    if(compileLogLength > 0) {
        vector<char> errorMessage(compileLogLength + 1);
        glGetShaderInfoLog(idFragmentShader, compileLogLength, nullptr, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }

    idProgram = glCreateProgram();
    glAttachShader(idProgram, idVertexShader);
    glAttachShader(idProgram, idFragmentShader);
    glLinkProgram(idProgram);

    glDetachShader(idProgram, idVertexShader);
    glDetachShader(idProgram, idFragmentShader);

    glDeleteShader(idVertexShader);
    glDeleteShader(idFragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(idProgram);
}