#version 330 core

in vec3 pos;
in vec3 norm;
in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useTexture = false;
uniform sampler2D texture0;

uniform vec4 eyePos = vec4(0, 0, 0, 1);
uniform vec3 objectColor = vec3(0.7, 0.2, 0.0);

uniform vec4 lightPos = vec4(-10, 15, 10, 1);
uniform vec4 ambientColor = vec4(1, 1, 1, 1);
uniform vec4 diffuseColor = vec4(1, 1, 1, 1);
uniform vec4 specularColor = vec4(1, 1, 1, 1);
uniform float ambientStrength = 0.2;
uniform float specularStrength = 0.4;
uniform float hardness = 32;

out vec4 FragColor;

void main(){
  // Normalize normals just in case
  vec3 normNorm = normalize(norm);

  // Ambient Calculation
  vec4 ambient = ambientStrength * ambientColor;

  // Diffuse Calculation
  vec3 relativeLightPos = vec3(lightPos);
  vec3 lightDir = normalize(relativeLightPos - pos);
  vec4 diffuse = max(dot(lightDir, normNorm), 0.0) * diffuseColor;

  // Specular Calculation
  vec3 viewDir = normalize(eyePos.xyz - pos);
  vec3 reflectDir = reflect(-lightDir, normNorm);
  float specFalloff = pow(max(dot(viewDir, reflectDir), 0.0), hardness);
  vec4 specular = specularStrength * specFalloff * specularColor;
  

  if(useTexture) {
    FragColor = specular + (ambient + diffuse) * texture(texture0, vec2(tex.x, 1 - tex.y));
  }
  else {
    FragColor = specular + (ambient + diffuse) * vec4(objectColor, 1);
  }
}
