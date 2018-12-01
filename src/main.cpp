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
Model ground;
Model customModel;
Model customModel2;
Model customModel3;
Model customModel4;
Model customModel5;
Model street;
Model handmadeModel;
GLuint vShader, fShader, program;
glm::mat4 view, proj; 
float dist = 3.0f;
float sideMovement = 0.0f;
float upMovement = 0.0f;
float degToRad = 3.14159265f / 180;


void setMatrix(glm::mat4 mat, std::string name); 

int main(int argc, char* argv[]) {
  Settings cfg;

  init(cfg, window);

  setupModels();

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.3, 0.3, 0.3, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Rotate objects
    customModel.modelMatrix = glm::rotate(customModel.modelMatrix, .01f, glm::vec3(0, 1, 0));
    //customModel2.modelMatrix = glm::rotate(glm::mat4(), -.005f, glm::vec3(0, 1, 0)) * customModel2.modelMatrix;
    customModel3.modelMatrix = glm::rotate(customModel3.modelMatrix, -.005f, glm::vec3(0, 1, 0));

    keyHoldEvents();
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
  glfwSetWindowSizeCallback(window, windowSizeCallback);
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
  int lightPos = glGetUniformLocation(program, "lightPos");
  int diffuseColor = glGetUniformLocation(program, "diffuseColor");
  int specularColor = glGetUniformLocation(program, "specularColor");
  int eyePos = glGetUniformLocation(program, "eyePos");
  int hardness = glGetUniformLocation(program, "hardness");
  int specPower = glGetUniformLocation(program, "specularStrength");
  int objColor = glGetUniformLocation(program, "objectColor");
  int ambientStr = glGetUniformLocation(program, "ambientStrength");
  glm::vec3 eye = getEyePosFromView(view);
  glUniform4f(eyePos, eye.x, eye.y, eye.z, 1.0f);

  resetLightToDefaults();
  glUniform4f(lightPos, 0, 100, -10, 0);
  renderCube(cubeModel);
  handmadeModel.render();
  glUniform4f(lightPos, 0, 5, -10, 0);
  glUniform3f(objColor, 0.5, 0.2, 0.0);
  glUniform1f(hardness, 50);
  glUniform1f(specPower, 0.7);
  ground.render();
  glUniform3f(objColor, 0.4, 0.4, 0.4);

  // Render customModel with depth mask and stencil mask
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);

  // Front (Rabbit)
  glUniform1f(specPower, 1);
  glUniform1f(hardness, 80);
  glUniform4f(lightPos, 0, 0, 10, 1);
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  customModel.render();
  resetLightToDefaults();

  // Back (AlienCity)
  glUniform4f(lightPos, 0, 0, -10, 1);
  glUniform4f(specularColor, 0.2, 1, 0.4, 1);
  glUniform4f(diffuseColor, 0.4, 1, 0.8, 1);
  glUniform1f(hardness, 70);
  glUniform1f(specPower, 0.9);
  glStencilFunc(GL_EQUAL, 2, 0xFF);
  customModel2.render();

  // Left (Earth)
  glUniform4f(specularColor, 0.8, 0.8, 0.3, 1);
  glUniform4f(diffuseColor, 1, 1, 1.0, 1.0);
  glUniform1f(hardness, 3);
  glUniform1f(specPower, 0.7);
  glUniform4f(lightPos, -10, 0, 0, 1);
  glStencilFunc(GL_EQUAL, 3, 0xFF);
  customModel3.render();

  // Right (Lamp Post)
  glUniform4f(specularColor, 0.0, 0.3, 1, 1);
  glUniform4f(diffuseColor, 0.7, 0.8, 1, 1);
  glUniform1f(hardness, 32);
  glUniform1f(specPower, 0.4);
  glUniform4f(lightPos, 10, 0, 0, 1);
  glStencilFunc(GL_EQUAL, 4, 0xFF);
  customModel4.render();
  street.render();
  resetLightToDefaults();

  // Teapot
  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  // Draw a red outline
  glUniform4f(lightPos, 0, 0, 10, 1);

  // Set up red sillouette
  glUniform1f(ambientStr, 1.0f);
  glUniform1f(specPower, 0);
  glUniform3f(objColor, 1, 0, 0);
  customModel5.render();
  // Reset to defaults
  glUniform1f(ambientStr, 0.4);
  glUniform1f(specPower, 0.4);
  glUniform3f(objColor, 0.4, 0.4, 0.4);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  customModel5.render();

  glDisable(GL_STENCIL_TEST);
  glStencilMask(0xFF);

}

string readFile(string filename) {
  ifstream f(filename);
  string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
  return s;
}

glm::vec3 getEyePosFromView(glm::mat4 &view) {
  glm::mat4 inv = glm::inverse(view);
  return glm::vec3(inv[3][0], inv[3][1], inv[3][2]);
}

void setupModels() {
  // Load Models
  customModel.loadFromFile("rabbit.obj", true);
  customModel2.loadFromFile("AlienCity.obj", true);
  customModel3.loadFromFile("earth.obj", true);
  customModel4.loadFromFile("lamp.obj", true);
  customModel5.loadFromFile("teapot.obj", true);
  street.loadFromFile("StreetEnv.obj", true);
  ground.loadFromFile("quad.obj", true);
  handmadeModel.loadFromFile("CustomObject.obj");
  initCube();

  // Load Textures
  customModel2.texture.loadFromFile("AlienCity.jpg");
  customModel3.texture.loadFromFile("earth.jpg");

  customModel.program = program;
  customModel2.program = program;
  customModel3.program = program;
  customModel4.program = program;
  customModel5.program = program;
  street.program = program;
  ground.program = program;
  handmadeModel.program = program;

  customModel.modelMatrix = glm::translate(customModel.modelMatrix, glm::vec3(0, -0.4, 0));
  customModel.modelMatrix = glm::scale(customModel.modelMatrix, glm::vec3(0.5, 0.5, 0.5));
  customModel2.modelMatrix = glm::translate(customModel2.modelMatrix, glm::vec3(0, -1.75, -3.2));
  customModel2.modelMatrix = glm::rotate(customModel2.modelMatrix, 180.f * degToRad, glm::vec3(0, 1, 0));
  customModel2.modelMatrix = glm::scale(customModel2.modelMatrix, glm::vec3(8, 8, 8));
  customModel3.modelMatrix = glm::scale(customModel3.modelMatrix, glm::vec3(0.3, 0.3, 0.3));
  customModel4.modelMatrix = glm::translate(customModel4.modelMatrix, glm::vec3(0, -0.3, 0));
  customModel5.modelMatrix = glm::translate(customModel5.modelMatrix, glm::vec3(0, 0.3, 0.3));
  customModel5.modelMatrix = glm::scale(customModel5.modelMatrix, glm::vec3(0.1, 0.1, 0.1));
  street.modelMatrix = glm::translate(street.modelMatrix, glm::vec3(0, -0.65, 0));
  street.modelMatrix = glm::scale(street.modelMatrix, glm::vec3(10, 10, 10));
  ground.modelMatrix = glm::translate(ground.modelMatrix, glm::vec3(0, -0.5, 0));
  ground.modelMatrix = glm::rotate(ground.modelMatrix, -90.0f * degToRad, glm::vec3(1, 0, 0));
  ground.modelMatrix = glm::scale(ground.modelMatrix, glm::vec3(1.125, 1.125, 1.125));
  handmadeModel.modelMatrix = glm::translate(handmadeModel.modelMatrix, glm::vec3(0, 2, 0));
}

void resetLightToDefaults() {
  int lightPos = glGetUniformLocation(program, "lightPos");
  int diffuseColor = glGetUniformLocation(program, "diffuseColor");
  int specularColor = glGetUniformLocation(program, "specularColor");
  int eyePos = glGetUniformLocation(program, "eyePos");
  int hardness = glGetUniformLocation(program, "hardness");
  int specPower = glGetUniformLocation(program, "specularStrength");
  int objColor = glGetUniformLocation(program, "objectColor");
  int ambientStr = glGetUniformLocation(program, "ambientStrength");

  glUniform4f(specularColor, 1, 1, 1, 1);
  glUniform4f(diffuseColor, 1, 1, 1, 1);
  glUniform1f(ambientStr, 0.4);
  glUniform1f(specPower, 0.4);
  glUniform3f(objColor, 0.4, 0.4, 0.4);
  glUniform1f(hardness, 32);
}
