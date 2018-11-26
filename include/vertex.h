#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


struct Vertex
{
  // Position Vector
  glm::vec3 Position;

  // Normal Vector
  glm::vec3 Normal;

  // Texture Coordinate Vector
  glm::vec2 TextureCoordinate;

  bool operator==(const Vertex& other) const;
};

namespace std {
  template<> struct hash<Vertex> {
    size_t operator()(Vertex const& vertex) const {
      return ((hash<glm::vec3>()(vertex.Position) ^
            (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
        (hash<glm::vec2>()(vertex.TextureCoordinate) << 1);
    }
  };
};
