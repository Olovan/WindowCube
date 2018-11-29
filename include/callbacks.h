#pragma once
#include <GLFW/glfw3.h>

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods); 
void keyHoldEvents(); 
void windowSizeCallback(GLFWwindow* win, int width, int height);
