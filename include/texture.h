#pragma once

#include <string>

class Texture {
  public:
    unsigned int id;
    bool assigned = false;
    void loadFromFile(std::string filename);
};
