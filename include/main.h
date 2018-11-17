#pragma once
#include <settings.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]);
void init(Settings &settings, GLFWwindow* &window); 
void crash(string message); 
void render();
std::string readFile(std::string filename);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
