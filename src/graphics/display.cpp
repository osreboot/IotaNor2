
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "display.h"

namespace display {

    static GLFWwindow* window;
    static Coordw windowSize;
    static Coordf locationCursor;

    void initialize() {
        windowSize = {1920, 1080};

        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(windowSize.first, windowSize.second, "", glfwGetPrimaryMonitor(), nullptr);
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

    const Coordw& getSize() {
        return windowSize;
    }

    const Coordf& getCursor() {
        return locationCursor;
    }

}
