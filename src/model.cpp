#include "model.h" 
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

vector<string> split_coords(string coords); 
void normalize(vector<float> &v);

bool Model::loadFromFile(string filename) {
  ifstream file(filename);

  // Bail out if file cannot be opened or doesn't exist
  if(!file.good()) return false;

  vector<int> tmpIndices;
  string line;
  while(getline(file, line)) {
    string token = line.substr(0, line.find(" "));
    string value = line.substr(line.find(" ") + 1);
    istringstream stream(value);

    if(token.compare("v") == 0) {
      float x[3];
      sscanf(value.c_str(), "%f %f %f", x, x+1, x+2);
      for(int i = 0; i < 3; i++) {
        vertices.push_back(x[i]);
      }
      // DEBUG push colors
      colors.push_back(.8);
      colors.push_back(.8);
      colors.push_back(.8);
    }
    else if(token.compare("vn") == 0) {
      float x[3];
      sscanf(value.c_str(), "%f %f %f", x, x+1, x+2);
      for(int i = 0; i < 3; i++) {
        normals.push_back(x[i]);
      }
    }
    else if(token.compare("vt") == 0) {
    }
    else if(token.compare("f") == 0) {
      unsigned int x[6];
      sscanf(value.c_str(), "%d//%d %d//%d %d//%d", x, x+1, x+2, x+3, x+4, x+5);
      for(int i = 0; i < 3; i++) {
        tmpIndices.push_back(x[i * 2]);
      }
    }
    else {
    }
  }

  //Fix Indices because OBJ file format is not 0 indexed
  for(int i = 0; i < tmpIndices.size(); i++) {
    if(tmpIndices[i] > 0) {
      tmpIndices[i] -= 1;
    } else {
      tmpIndices[i] = tmpIndices.size() - tmpIndices[i];
    }
  }

  for(int i = 0; i < tmpIndices.size(); i++) {
    indices.push_back((unsigned int)tmpIndices[i]);
  }

  // Normalize Vertices
  normalize(vertices);
  for(int i = 0; i < vertices.size(); i++) {
    if((i % 3) == 2)
      vertices[i] = 0;
  }

  return true;
}

void normalize(vector<float> &coords) {
  float max = 0;
  for(int i = 0; i < coords.size(); i++) {
    if(fabs(coords[i]) > max)
      max = fabs(coords[i]);
  }
  printf("MAX: %f\n", max);
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
