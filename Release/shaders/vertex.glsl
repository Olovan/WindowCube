#version 330 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNorm;
layout (location=2) in vec2 inTex;
layout (location=3) in vec3 tangent;
layout (location=4) in vec3 biTangent;

out vec3 pos;
out vec3 norm;
out vec2 tex;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useNormalTexture = false;

void main() 
{
  gl_Position = projection * view * model * vec4(inPos.x, inPos.y, inPos.z, 1.0);
  // Pass on the position
  pos = vec3(model * vec4(inPos, 1));
  tex = inTex;

  // Modify normal and pass it on
  norm = normalize(mat3(transpose(inverse(model))) * inNorm); //Special normal transformation because of non-uniform scaling

  vec3 T = normalize(vec3(model * vec4(tangent, 0)));
  vec3 B = normalize(vec3(model * vec4(biTangent, 0)));
  vec3 N = normalize(vec3(model * vec4(inNorm, 0)));
  TBN = mat3(T, B, N);
}
