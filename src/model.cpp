#include <GL/glew.h>
#include "model.h" 

#include "tinyobj.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tinyobj.h"

using namespace std;

vector<string> split_coords(string coords); 
void normalize(vector<float> &v);

bool Model::loadFromFile(string filename) {
  return Model::loadFromFile(filename, false);
}

bool Model::loadFromFile(string filename, bool normalize_verts) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;
  if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
    std::cout << "Obj load failed for file " << filename << std::endl;
    return false;
  }

  positions = attrib.vertices;
  normals = attrib.normals;
  textures = attrib.texcoords;

  if(normalize_verts) {
    normalize(positions);
    normalize(normals);
  }

  // Create vertices and indices
  int dups = 0;
  std::unordered_map<Vertex, uint32_t> uniqueVerts = {};
  for(const auto& shape : shapes) {
    for(const auto& index : shape.mesh.indices) {
      Vertex v = {};
      v.Position.x = positions[index.vertex_index * 3 + 0];
      v.Position.y = positions[index.vertex_index * 3 + 1];
      v.Position.z = positions[index.vertex_index * 3 + 2];
      v.Normal.x = normals[index.normal_index * 3 + 0];
      v.Normal.y = normals[index.normal_index * 3 + 1];
      v.Normal.z = normals[index.normal_index * 3 + 2];
      if(textures.size() > 0) {
        v.TextureCoordinate.x = textures[index.texcoord_index * 2 + 0];
        v.TextureCoordinate.y = textures[index.texcoord_index * 2 + 1];
      }
      if(uniqueVerts.count(v) == 0) {
        uniqueVerts[v] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(v);
      } 
      else {
        dups++;
      }
      indices.push_back(uniqueVerts[v]);
    }
  }

  // Set up tangent and bitangent coordinates for normal mapping
  for (unsigned int i = 0 ; i < indices.size() ; i += 3) {
      Vertex& v0 = vertices[indices[i]];
      Vertex& v1 = vertices[indices[i+1]];
      Vertex& v2 = vertices[indices[i+2]];

      glm::vec3 edge1 = v1.Position - v0.Position;
      glm::vec3 edge2 = v2.Position - v0.Position;

      float DeltaU1 = v1.TextureCoordinate.x - v0.TextureCoordinate.x;
      float DeltaV1 = v1.TextureCoordinate.y - v0.TextureCoordinate.y;
      float DeltaU2 = v2.TextureCoordinate.x - v0.TextureCoordinate.x;
      float DeltaV2 = v2.TextureCoordinate.y - v0.TextureCoordinate.y;

      float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

      glm::vec3 Tangent, Bitangent;

      Tangent.x = f * (DeltaV2 * edge1.x - DeltaV1 * edge2.x);
      Tangent.y = f * (DeltaV2 * edge1.y - DeltaV1 * edge2.y);
      Tangent.z = f * (DeltaV2 * edge1.z - DeltaV1 * edge2.z);

      Bitangent.x = f * (-DeltaU2 * edge1.x - DeltaU1 * edge2.x);
      Bitangent.y = f * (-DeltaU2 * edge1.y - DeltaU1 * edge2.y);
      Bitangent.z = f * (-DeltaU2 * edge1.z - DeltaU1 * edge2.z);

      v0.Tangent += Tangent;
      v1.Tangent += Tangent;
      v2.Tangent += Tangent;
      v0.BiTangent += Bitangent;
      v1.BiTangent += Bitangent;
      v2.BiTangent += Bitangent;
  }

  for (unsigned int i = 0 ; i < vertices.size() ; i++) {
    vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
    vertices[i].BiTangent = glm::normalize(vertices[i].BiTangent);
  } 


  setupBuffers();
  return true;
}

void Model::setupBuffers() {
  // VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  // EBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * indices.size(), indices.data(), GL_STATIC_DRAW);

  //Setup attribute bindings
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); 
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinate)); 
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)); 
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent)); 

  glBindVertexArray(0); // Stop tracking state with VAO
  // Now that we are no longer tracking state with VAO we can unbind these buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::render() {
  render(glm::mat4(1));
}

void Model::render(glm::mat4 base) {
  glBindVertexArray(VAO);
  // Set model matrix
  int loc = glGetUniformLocation(program, "model");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(base * modelMatrix));
  // Set up textures
  if(diffuseTexture.assigned) {
    int useTex = glGetUniformLocation(program, "useDiffuseTexture");
    glUniform1i(useTex, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture.id);
  }
  if(specularTexture.assigned) {
    int useTex = glGetUniformLocation(program, "useSpecularTexture");
    glUniform1i(useTex, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture.id);
  }
  if(normalTexture.assigned) {
    int useTex = glGetUniformLocation(program, "useNormalTexture");
    glUniform1i(useTex, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTexture.id);
  }

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

  // Turn off textures if they were turned on
  if(diffuseTexture.assigned) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    int useTex = glGetUniformLocation(program, "useDiffuseTexture");
    glUniform1i(useTex, 0);
  }
  if(specularTexture.assigned) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    int useTex = glGetUniformLocation(program, "useSpecularTexture");
    glUniform1i(useTex, 0);
  }
  if(normalTexture.assigned) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    int useTex = glGetUniformLocation(program, "useNormalTexture");
    glUniform1i(useTex, 0);
  }
  glBindVertexArray(0);
}

// Helper Methods
void normalize(vector<float> &coords) {
  float max = 0;
  for(int i = 0; i < coords.size(); i++) {
    if(fabs(coords[i]) > max)
      max = fabs(coords[i]);
  }
  for(int i = 0; i < coords.size(); i++) {
    coords[i] = coords[i] / max;
  }
}

vector<string> split_coords(string coords) {
  vector<string> results;
  char* token;
  token = strtok((char*)coords.c_str(), " ");
  while(token != nullptr) {
    results.push_back(token);
    token = strtok(NULL, " ");
  }
  return results;
}

Vertex buildFromIndices(int pos, int norm, int tex) {
  Vertex vert;
  return vert;
}

