
#include <map>

#include "stb/stb_image.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "display.h"
#include "texture.h"

static bool eventMousePress, eventMouseRelease;
static std::map<int, bool> keyStatesLast;

void callbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) eventMousePress = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) eventMouseRelease = true;
}

namespace display {

    static GLFWwindow* window;
    static Coordw windowSize;
    static Coordf locationCursor;

    void initialize() {
        windowSize = {1920, 1080};

        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(windowSize.first, windowSize.second, "Iota Nor - by Calvin Weaver", glfwGetPrimaryMonitor(), nullptr);
        glfwMakeContextCurrent(window);

        GLFWimage images[1];
        images[0].pixels = stbi_load("res/texture/ui/icon.png", &images[0].width, &images[0].height, nullptr, 4);
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);

        glfwSetMouseButtonCallback(window, callbackMouseButton);

        glewInit();
    }

    void preUpdate() {
        eventMousePress = false;
        eventMouseRelease = false;

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

        for (const auto &keyState : keyStatesLast) {
            keyStatesLast[keyState.first] = glfwGetKey(window, keyState.first) == GLFW_PRESS;
        }
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

    bool hasEventMousePress() {
        return eventMousePress;
    }

    bool hasEventMouseRelease() {
        return eventMouseRelease;
    }

    bool hasEventKeyPress(int key) {
        if(!keyStatesLast.count(key)) {
            keyStatesLast[key] = glfwGetKey(window, key) == GLFW_PRESS;
            return false;
        } else return glfwGetKey(window, key) == GLFW_PRESS && keyStatesLast.count(key) && !keyStatesLast[key];
    }

    bool hasEventKeyRelease(int key) {
        if(!keyStatesLast.count(key)) {
            keyStatesLast[key] = glfwGetKey(window, key) == GLFW_PRESS;
            return false;
        } else return glfwGetKey(window, key) == GLFW_RELEASE && keyStatesLast.count(key) && keyStatesLast[key];
    }

}
