#include "settings.h"
#include <fstream>
#include <string.h>
#include <iostream>

using namespace std;

void Settings::loadFromFile(string filename) {
  ifstream file;

  // Make sure file exists
  if(!file.good()) return;

  file.open(filename.c_str());

  for(string line; getline(file, line);) {
    string option = line.substr(0, line.find(" "));
    string value = line.substr(line.find(" ") + 1);
    parse(option, value);
  }
  file.close();
}

void Settings::parse(string option, string value) {
  if (option.compare("WINDOW_WIDTH") == 0) {
    windowWidth = stoi(value);
  }
  else if (option.compare("WINDOW_HEIGHT") == 0) {
    windowHeight = stoi(value);
  }
  else if (option.compare("WINDOW_TITLE") == 0) {
    windowTitle = value;
  }
}
