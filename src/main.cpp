#include <GL/glew.h> // Call glew first because it's picky like that
#include "main.h"
#include "model.h"
#include "settings.h"
#include "callbacks.h"
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
Model quad;
Model customModel;
GLuint vShader, fShader, program;
glm::mat4 view, proj; //3 global matrices
float dist = 3.0f;
float sideMovement = 0.0f;
float upMovement = 0.0f;


void setMatrix(glm::mat4 mat, std::string name); 

int main(int argc, char* argv[]) {
  Settings cfg;

  GLFWwindow* window;
  init(cfg, window);

  quad.loadFromFile(cfg.modelName);
  customModel.loadFromFile("knight.obj", true);
  quad.program = program;
  customModel.program = program;

  std::cout << "Quad Vertices: " << quad.vertices.size() << std::endl;
  std::cout << "Model Vertices: " << customModel.vertices.size() << std::endl;

  quad.modelMatrix = glm::scale(quad.modelMatrix, glm::vec3(3, 3, 3));
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Rotate objects
    //quad.modelMatrix = glm::rotate(quad.modelMatrix, .01f, glm::vec3(0, 1, 0));
    customModel.modelMatrix = glm::rotate(customModel.modelMatrix, .01f, glm::vec3(0, 1, 0));

    view = glm::lookAt(vec3(sideMovement, upMovement, dist), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
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

  // Enable stencil and depth test
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  proj = glm::perspective(45.0f, 1.5f, 0.1f, 10.0f);
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

void render() {
  // Render quad without depth mask
  setMatrix(view, "view");
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
  quad.render();
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  // Render customModel with depth mask and stencil mask
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glDepthMask(GL_TRUE);
  customModel.render();
  glDisable(GL_STENCIL_TEST);
  glStencilMask(0xFF);
}

string readFile(string filename) {
  ifstream f(filename);
  string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
  return s;
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
