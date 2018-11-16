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

    bool loadFromFile(std::string filename);
};
