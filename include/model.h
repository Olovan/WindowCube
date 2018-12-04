#pragma once

#include <vector>
#include <string>
#include "vertex.h"
#include "texture.h"
#include <glm/mat4x4.hpp>

class Model {
  public:
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> textures;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    unsigned int program; //Shader program
    Texture diffuseTexture;
    Texture specularTexture;
    Texture normalTexture;


    bool loadFromFile(std::string filename, bool normalize);
    bool loadFromFile(std::string filename);
    void setupBuffers();
    void render();
    void render(glm::mat4);

  private:
    unsigned int VAO; // Vertex Array Object
    unsigned int VBO; // Vertex Buffer Object
    unsigned int EBO; // Element Buffer Object
};
