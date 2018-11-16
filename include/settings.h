#pragma once

#include <string>

using std::string;
class Settings {
  public:
    int windowWidth = 640;
    int windowHeight = 480;
    string windowTitle = "TITLE";

    void loadFromFile(string filename);

  private:
    void parse(string option, string value);
};
