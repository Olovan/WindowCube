#pragma once

#include <vector>
#include <string>

class Model {
  public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<unsigned int> indices;
    std::vector<float> textureCoords;

    bool loadFromFile(std::string filename, bool normalize);
    bool loadFromFile(std::string filename);
    void setupBuffers();
    void render();

  private:
    unsigned int VAO; // Vertex Array Object
    unsigned int VBO; // Vertex Buffer Object
    unsigned int EBO; // Element Buffer Object
};
