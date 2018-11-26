#include "vertex.h"

  bool Vertex::operator==(const Vertex& other) const {
    if(
        this->Position.x == other.Position.x &&
        this->Position.y == other.Position.y &&
        this->Position.z == other.Position.z &&
        this->Normal.x == other.Normal.x &&
        this->Normal.y == other.Normal.y &&
        this->Normal.z == other.Normal.z &&
        this->TextureCoordinate.x == other.TextureCoordinate.x &&
        this->TextureCoordinate.y == other.TextureCoordinate.y
      ) {
      return true;
    }
    else {
      return false;
    }
  }
