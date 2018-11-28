#version 330 core

in vec3 pos;
in vec3 norm;
in vec2 tex;

//uniform sampler2D tex;

vec3 objectColor = vec3(0.7, 0.2, 0.0);

float ambientStrength = 0.2;
vec3 lightPos = vec3(-10, 15, 0);
vec3 lightColor = vec3(1, 1, 1);

out vec4 FragColor;

void main(){
  // Ambient Calculation
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse Calculation
  vec3 lightDir = normalize(lightPos - pos);
  vec3 diffuse = max(dot(lightDir, norm), 0.0) * lightColor;

  FragColor = vec4((ambient + diffuse) * objectColor, 1);
}
