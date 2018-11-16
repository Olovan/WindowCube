#include<callbacks.h>

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}
