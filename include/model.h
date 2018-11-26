#pragma once

#include <vector>
#include <string>
#include "vertex.h"
#include <glm/mat4x4.hpp>

class Model {
  public:
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> textures;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 modelMatrix;
    unsigned int program; //Shader program


    bool loadFromFile(std::string filename, bool normalize);
    bool loadFromFile(std::string filename);
    void setupBuffers();
    void render();

  private:
    unsigned int VAO; // Vertex Array Object
    unsigned int VBO; // Vertex Buffer Object
    unsigned int EBO; // Element Buffer Object
};
