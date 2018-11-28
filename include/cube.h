#pragma once

#include <glm/mat4x4.hpp>

void initCube();
void renderCube(glm::mat4);
void enableStencil(int num); 
void disableStencil();
