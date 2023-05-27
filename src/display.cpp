#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "display.h"

namespace display {

    static GLFWwindow* window;
    static pair<GLuint, GLuint> windowSize;
    static pair<GLfloat, GLfloat> locationCursor;

    void initialize() {
        windowSize = {1280, 720};

        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(windowSize.first, windowSize.second, "", nullptr, nullptr);
        glfwMakeContextCurrent(window);

        glewInit();
    }

    void preUpdate() {
        glfwPollEvents();

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        locationCursor.first = static_cast<GLfloat>(x);
        locationCursor.second = static_cast<GLfloat>(y);

        glViewport(0, 0, windowSize.first, windowSize.second);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void postUpdate() {
        glfwSwapBuffers(window);
    }

    void close() {
        glfwTerminate();
    }

    bool isExiting() {
        return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    const pair<GLuint, GLuint>& getSize() {
        return windowSize;
    }

    const pair<GLfloat, GLfloat>& getCursor() {
        return locationCursor;
    }

}
