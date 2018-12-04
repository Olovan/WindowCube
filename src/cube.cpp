#include <Windows.h> // Need windows.h before GL.h
#include <GL/glew.h>
#include "cube.h"
#include "model.h"
#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

float DEG_TO_RAD = 3.14159265 / 180;

Model front, back, left, right, top, bottom;
Model cube;
extern GLuint program;

void initCube() {
  front.loadFromFile("resources/quad.obj");
  back.loadFromFile("resources/quad.obj");
  left.loadFromFile("resources/quad.obj");
  right.loadFromFile("resources/quad.obj");
  top.loadFromFile("resources/quad.obj");
  bottom.loadFromFile("resources/quad.obj");
  cube.loadFromFile("resources/cube.obj");

  front.program = program;
  back.program = program;
  top.program = program;
  bottom.program = program;
  left.program = program;
  right.program = program;
  cube.program = program;

  front.modelMatrix = glm::translate(front.modelMatrix, glm::vec3(0, 0, 0.5));
  back.modelMatrix = glm::rotate(back.modelMatrix, 180.0f * DEG_TO_RAD, glm::vec3(0, 1, 0));
  back.modelMatrix = glm::translate(back.modelMatrix, glm::vec3(0, 0, 0.5));

  top.modelMatrix = glm::rotate(top.modelMatrix, 270.0f * DEG_TO_RAD, glm::vec3(1, 0, 0));
  top.modelMatrix = glm::translate(top.modelMatrix, glm::vec3(0, 0, 0.5));
  bottom.modelMatrix = glm::rotate(bottom.modelMatrix, 90.0f * DEG_TO_RAD, glm::vec3(1, 0, 0));
  bottom.modelMatrix = glm::translate(bottom.modelMatrix, glm::vec3(0, 0, 0.5));

  left.modelMatrix = glm::rotate(left.modelMatrix, -90.0f * DEG_TO_RAD, glm::vec3(0, 1, 0));
  left.modelMatrix = glm::translate(left.modelMatrix, glm::vec3(0, 0, 0.5));
  right.modelMatrix = glm::rotate(right.modelMatrix, 90.0f * DEG_TO_RAD, glm::vec3(0, 1, 0));
  right.modelMatrix = glm::translate(right.modelMatrix, glm::vec3(0, 0, 0.5));
}

void renderCube(glm::mat4 base) {
  // Set up different color for window sides
  int objColor = glGetUniformLocation(program, "objectColor");

  glUniform3f(objColor, 1, 1, 1);
  renderFrame(base);

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  glUniform3f(objColor, 0.8, 0.2, 0.2);
  //Front
  enableStencil(1);
  front.render(base);
  disableStencil();

  // Keep the background color for the large environments
  glUniform3f(objColor, 0.2, 0.5, 0.2);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  // Back
  enableStencil(2);
  back.render(base);
  disableStencil();

  glUniform3f(objColor, 0.8, 0.2, 0.8);
  // Right
  enableStencil(4);
  right.render(base);
  disableStencil();

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  glUniform3f(objColor, 0.2, 0.2, 0.8);
  // Left
  enableStencil(3);
  left.render(base);
  disableStencil();

  glUniform3f(objColor, 0.4, 0.4, 0.4);
  top.render(base);

  bottom.render(base);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void enableStencil(int num) {
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, num, 0xFF);
  glStencilMask(0xFF);
}

void renderFrame(glm::mat4 base) {
  cube.modelMatrix = glm::mat4(1);
  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.5f, 0.0f, 0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.5f, 0.0f, 0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.5f, 0.0f, -0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.5f, 0.0f, -0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
  cube.render(base);

  // Flat top
  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.5f, 0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(1.05, 0.05f, 0.05f));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.5f, -0.5f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(1.05, 0.05f, 0.05f));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.0f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 0.05f, 1.05));
  cube.render(base);

  cube.modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.5f, 0.5f, 0.0f));
  cube.modelMatrix = glm::scale(cube.modelMatrix, glm::vec3(0.05f, 0.05f, 1.05));
  cube.render(base);
}

void disableStencil() {
  glDisable(GL_STENCIL_TEST);
}
