#include "main.h"
#include "model.h"
#include "settings.h"
#include "callbacks.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;
Model model;

int main(int argc, char* argv[]) {
  Settings cfg;

  GLFWwindow* window;
  init(cfg, window);
  model.loadFromFile("rabbit.obj");
  printf("Verts:   %ld\n", model.vertices.size());
  printf("Normals: %ld\n", model.normals.size());
  printf("Indices: %ld\n", model.indices.size());
  printf("Colors:  %ld\n\n", model.colors.size());
  printf("VertLocation:  %p\n", &model.colors[0]);
  printf("Poly0 normals:  %d, %d, %d\n", model.indices[0], model.indices[1], model.indices[2]);
  printf("Poly0:          %f, %f, %f\n", model.vertices[model.indices[0]], model.vertices[model.indices[1]], model.vertices[model.indices[2]]);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  window = glfwCreateWindow(cfg.windowWidth, cfg.windowHeight, cfg.windowTitle.c_str(), NULL, NULL);
  if(!window) {
    crash("Window is null!");
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, keyCallback);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  //glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient);
  //glMatrixMode(GL_PROJECTION);
  //glOrtho(-10, 10, -10, 10, -10, 10);
  //glLightfv(GL_LIGHT0, GL_DIFFUSE, (float[]){0.5, 0.5, 0.5, 0.4});
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat ambient[] = {.2f, .2f, .2f, .2f};
  GLfloat specularMat[] = {0.3f, 0.2f, 0.2f, 0.4f};
  GLfloat diffuseMat[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat position[] = {0.0f, 5.0f, 0.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
  //Camera Stuff
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.6, 0.01, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, -5, 0, 0, 0, 0.0, 1.0, 0.0);
}

void crash(string message) {
  cout << message << endl;
  exit(1);
}

void render() {
  glMatrixMode(GL_MODELVIEW);
  glRotatef(1, 0, 1, 0);


  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &model.vertices.front());
  glColorPointer(3, GL_FLOAT, 0, &model.colors.front());
  glNormalPointer(GL_FLOAT, 0, &model.normals.front());
  glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, &model.indices.front());
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  /*
  glBegin(GL_TRIANGLES);
  glColor3f(1, 0, 0);
  glVertex3f(-1, -1, 0);
  glColor3f(0, 1, 0);
  glVertex3f(1, -1, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 1, 0);
  glEnd();
  */
}
