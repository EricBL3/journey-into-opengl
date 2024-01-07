#pragma once

#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int colors();

void processCameraInput(GLFWwindow* window);
