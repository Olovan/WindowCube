#include <Windows.h> // Need windows.h before GL.h
#include "texture.h"
#include "stb_image.h"
#include <GL/gl.h>

void Texture::loadFromFile(std::string filename) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  int x, y, bpp;
  unsigned char *data = stbi_load(filename.c_str(), &x, &y, &bpp, 4);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);
  assigned = true;
}
