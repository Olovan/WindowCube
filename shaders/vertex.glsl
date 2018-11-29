#version 330 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNorm;
layout (location=2) in vec2 inTex;

out vec3 pos;
out vec3 norm;
out vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
  gl_Position = projection * view * model * vec4(inPos.x, inPos.y, inPos.z, 1.0);
  // Pass on the position
  pos = vec3(model * vec4(inPos, 1));
  // Modify normal and pass it on
  norm = mat3(transpose(inverse(model))) * inNorm; //Special normal transformation because of non-uniform scaling
  tex = inTex;
}
