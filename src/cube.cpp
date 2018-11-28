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
extern GLuint program;

void initCube() {
  front.loadFromFile("quad.obj");
  back.loadFromFile("quad.obj");
  left.loadFromFile("quad.obj");
  right.loadFromFile("quad.obj");
  top.loadFromFile("quad.obj");
  bottom.loadFromFile("quad.obj");

  front.program = program;
  back.program = program;
  top.program = program;
  bottom.program = program;
  left.program = program;
  right.program = program;

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
  float color[] = {0.2, 0.2, 0.2};
  float defaultColor[] = {0.8, 0.8, 0.8};
  int loc = glGetUniformLocation(program, "objectColor");
  glUniform3fv(loc, 1, color);

  //Front
  enableStencil(1);
  front.render(base);
  disableStencil();

  // Back
  enableStencil(2);
  back.render(base);
  disableStencil();

  // Left
  enableStencil(3);
  left.render(base);
  disableStencil();

  // Right
  enableStencil(4);
  right.render(base);
  disableStencil();

  glUniform3fv(loc, 1, defaultColor);


  top.render(base);
  bottom.render(base);
}

void enableStencil(int num) {
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, num, 0xFF);
  glStencilMask(0xFF);
  //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void disableStencil() {
  glDisable(GL_STENCIL_TEST);
  //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
