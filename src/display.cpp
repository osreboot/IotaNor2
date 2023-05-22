#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "display.h"

namespace display {

    static GLFWwindow* window;
    static pair<int, int> windowSize;

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

    const pair<int, int>& getSize() {
        return windowSize;
    }

}
