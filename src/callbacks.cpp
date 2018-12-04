#include "model.h"
#include "callbacks.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

const double MOUSE_SENSITIVITY = 1;

extern glm::mat4 view, mouseRotation, proj;
extern GLFWwindow* window;
extern Model customModel3;
extern bool xrayVision;

int window_width, window_height;
double prev_mouseX, prev_mouseY;

void continuousInputHandler() {
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    view = glm::rotate(view, -0.01f, glm::vec3(0, 1, 0));
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    view = glm::rotate(view, 0.01f, glm::vec3(0, 1, 0));
  }
  if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    view = glm::translate(view, glm::vec3(0, -0.01, 0));
  }
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    view = glm::translate(view, glm::vec3(0, 0.01, 0));
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    view = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0.01)) * view;
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    view = glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.01)) * view;
  }

  double x, y;
  glfwGetCursorPos(window, &x, &y);
  double movement_x, movement_y;
  movement_x = x - prev_mouseX;
  movement_y = y - prev_mouseY;
  prev_mouseX = x;
  prev_mouseY = y;

  mouseRotation = glm::rotate(glm::mat4(1), (float)(x * MOUSE_SENSITIVITY/1000), glm::vec3(0, 1, 0));
  mouseRotation = glm::rotate(glm::mat4(1), (float)(y * MOUSE_SENSITIVITY/1000), glm::vec3(1, 0, 0)) * mouseRotation;
}

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
  if(action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_N:
        customModel3.normalTexture.assigned = !customModel3.normalTexture.assigned;
        break;
      case GLFW_KEY_X:
        xrayVision = !xrayVision;
        break;
    }
  }
}

void windowSizeCallback(GLFWwindow* win, int width, int height) {
  window_width = width;
  window_height = height;
  proj = glm::perspective(45.0f, (float)width / height, 0.1f, 100.0f);
  glViewport(0, 0, width, height);
}
