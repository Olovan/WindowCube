#version 330 core
layout (location=0) in vec3 aPos;

out vec3 color;
uniform mat4 matrix;

void main() 
{
  gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  color = vec3(abs(aPos.z), abs(aPos.y), abs(aPos.x));
}
