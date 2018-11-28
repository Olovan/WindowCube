#include <GL/glew.h> // Call glew first because it's picky like that
#include "main.h"
#include "model.h"
#include "settings.h"
#include "callbacks.h"
#include "shader.h"
#include "cube.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using glm::vec3;
using glm::mat4;

GLFWwindow* window;
Model customModel;
Model customModel2;
Model customModel3;
Model customModel4;
Model customModel5;
Model street;
GLuint vShader, fShader, program;
glm::mat4 view, proj; 
float dist = 3.0f;
float sideMovement = 0.0f;
float upMovement = 0.0f;


void setMatrix(glm::mat4 mat, std::string name); 

int main(int argc, char* argv[]) {
  Settings cfg;

  init(cfg, window);

  customModel.loadFromFile("rabbit.obj", true);
  customModel2.loadFromFile("knight.obj", true);
  customModel3.loadFromFile("sphere.obj", true);
  customModel4.loadFromFile("lamp.obj", true);
  customModel5.loadFromFile("teapot.obj", true);
  street.loadFromFile("StreetEnv.obj", true);
  initCube();
  customModel.program = program;
  customModel2.program = program;
  customModel3.program = program;
  customModel4.program = program;
  customModel5.program = program;
  street.program = program;

  std::cout << "Model Vertices: " << customModel.vertices.size() << std::endl;

  customModel.modelMatrix = glm::translate(customModel.modelMatrix, glm::vec3(0, -0.4, 0));
  customModel.modelMatrix = glm::scale(customModel.modelMatrix, glm::vec3(0.5, 0.5, 0.5));
  customModel2.modelMatrix = glm::translate(customModel2.modelMatrix, glm::vec3(0, -0.5, 0));
  customModel3.modelMatrix = glm::scale(customModel3.modelMatrix, glm::vec3(0.3, 0.3, 0.3));
  customModel4.modelMatrix = glm::translate(customModel4.modelMatrix, glm::vec3(0, -0.3, 0));
  customModel5.modelMatrix = glm::translate(customModel5.modelMatrix, glm::vec3(0, 0.3, 0.5));
  customModel5.modelMatrix = glm::scale(customModel5.modelMatrix, glm::vec3(0.1, 0.1, 0.1));
  street.modelMatrix = glm::translate(street.modelMatrix, glm::vec3(0, -0.65, 0));
  street.modelMatrix = glm::scale(street.modelMatrix, glm::vec3(10, 10, 10));
  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.3, 0.3, 0.3, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Rotate objects
    //customModel2.modelMatrix = glm::rotate(customModel2.modelMatrix, .01f, glm::vec3(0, 1, 0));
    customModel.modelMatrix = glm::rotate(customModel.modelMatrix, .01f, glm::vec3(0, 1, 0));
    customModel2.modelMatrix = glm::rotate(customModel2.modelMatrix, -.005f, glm::vec3(0, 1, 0));

    keyHoldEvents();
    //view = glm::lookAt(vec3(sideMovement, upMovement, dist), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void init(Settings &cfg, GLFWwindow* &window) {
  if(!glfwInit()) {
    crash("GLFW init failed!");
  }
  cfg.loadFromFile("settings.config");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  window = glfwCreateWindow(cfg.windowWidth, cfg.windowHeight, cfg.windowTitle.c_str(), NULL, NULL);
  if(!window) {
    crash("Window is null!");
  }
  glfwMakeContextCurrent(window);
  glewExperimental = true;
  GLenum err = glewInit();
  if(GLEW_OK != err) {
    crash("GLEW failed to init!");
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, keyCallback);

  //Shaders
  program = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
  glUseProgram(program);

  std::cout << "OPENGL VERSION: " << (char*)glGetString(GL_VERSION) << std::endl;

  // Enable stencil and depth test
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  proj = glm::perspective(45.0f, 1.5f, 0.1f, 100.0f);
  view = glm::lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
  setMatrix(proj, "projection");
}

void crash(string message) {
  cout << message << endl;
  exit(1);
}

void setMatrix(glm::mat4 mat, std::string name) {
  int loc = glGetUniformLocation(program, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

glm::mat4 cubeModel;
void render() {
  setMatrix(view, "view");

  //cubeModel = glm::rotate(cubeModel, 0.017f, glm::vec3(0, 1, 0));
  renderCube(cubeModel);

  // Render customModel with depth mask and stencil mask
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);

  glStencilFunc(GL_EQUAL, 1, 0xFF);
  customModel.render();

  glStencilFunc(GL_EQUAL, 2, 0xFF);
  customModel2.render();

  glStencilFunc(GL_EQUAL, 3, 0xFF);
  customModel3.render();

  glStencilFunc(GL_EQUAL, 4, 0xFF);
  customModel4.render();
  street.render();

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  customModel5.render();

  glDisable(GL_STENCIL_TEST);
  glStencilMask(0xFF);
}

string readFile(string filename) {
  ifstream f(filename);
  string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
  return s;
}

