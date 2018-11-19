#include<callbacks.h>

extern float dist;

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
  if(action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_W:
        dist -= 0.2;
        break;
      case GLFW_KEY_S:
        dist += 0.2;
        break;
    }
  }
}
