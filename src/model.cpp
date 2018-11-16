#include "model.h" 
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

vector<string> split_coords(string coords); 

bool Model::loadFromFile(string filename) {
  ifstream file(filename);

  // Bail out if file cannot be opened or doesn't exist
  if(!file.good()) return false;

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
        indices.push_back(x[i * 2]);
      }
    }
    else {
    }
  }

  //Fix Indices because OBJ file format is not 0 indexed
  for(int i = 0; i < indices.size(); i++) {
    if(indices[i] > 0) {
      indices[i] -= 1;
    } else {
      indices[i] = indices.size() - indices[i];
    }
  }
  return true;
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
