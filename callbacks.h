#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "globals.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    scrSize = { width, height };
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // TODO
        // emit signal, information, to all objects.
        // objects subscribed to such signal will react accordingly.

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 pos = { x, y };
        if (led1.contains(pos)) {
            expr.btn1.switch_();
            led1.setValue1(expr.btn1.value);
        }
        else if (led2.contains(pos)) {
            expr.btn2.switch_();
            led2.setValue1(expr.btn2.value);
        }
        else if (expr.btn1.contains(pos))
        {
            expr.btn1.switch_();
            led1.setValue1(expr.btn1.value);
        }
        else if (expr.btn2.contains(pos))
        {
            expr.btn2.switch_();
            led2.setValue1(expr.btn2.value);
        }

        expr.btnRes.setValue(expr.btn1.value && expr.btn2.value);
        led3.setValue1(expr.btnRes.value);
    }
}

