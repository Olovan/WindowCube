#include<callbacks.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern float dist;
extern float sideMovement;
extern float upMovement;
extern glm::mat4 view;
extern GLFWwindow* window;

void keyHoldEvents() {
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
}

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
      case GLFW_KEY_D:
        sideMovement += 0.2;
        break;
      case GLFW_KEY_A:
        sideMovement -= 0.2;
        break;
      case GLFW_KEY_E:
        upMovement += 0.2;
        break;
      case GLFW_KEY_Q:
        upMovement -= 0.2;
        break;
    }
  }
}
