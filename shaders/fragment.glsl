#version 330 core

in vec3 pos;
in vec3 norm;
in vec2 tex;
in mat3 TBN;

struct Light {
  vec4 lightPos;
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
  float ambientStrength;
  float specularStrength;
};

uniform float time = 0.0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform Light lights[2];

uniform bool useDiffuseTexture = false;
uniform bool useSpecularTexture = false;
uniform bool useNormalTexture = false;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec4 eyePos = vec4(0, 0, 0, 1);

uniform vec3 objectColor = vec3(0.7, 0.2, 0.0);
uniform float hardness = 32;


out vec4 FragColor;

void main(){
  vec3 normNorm;
  // Calculate normal from normalTexture if normal mapping is enabled
  if(useNormalTexture) {
    normNorm = texture(normalTexture, vec2(tex.x, 1-tex.y)).rgb;
    normNorm = normalize(normNorm * 2 - 1);
    normNorm = normalize(TBN * normNorm);
  }
  // If there is no bump mapping just use the regular normal
  else {
    normNorm = norm;
  }
  FragColor = vec4(0, 0, 0, 0);

  for(int i = 0; i < 2; i++) {
    // Ambient Calculation
    vec4 ambient = lights[i].ambientStrength * lights[i].ambientColor;

    // Diffuse Calculation
    vec3 relativeLightPos = vec3(lights[i].lightPos);
    vec3 lightDir = normalize(relativeLightPos - pos);
    vec4 diffuse = max(dot(lightDir, normNorm), 0.0) * lights[i].diffuseColor;

    // Specular Calculation
    vec3 viewDir = normalize(eyePos.xyz - pos);
    vec3 reflectDir = reflect(-lightDir, normNorm);
    float specFalloff = pow(max(dot(viewDir, reflectDir), 0.0), hardness);
    vec4 specular = lights[i].specularStrength * specFalloff * lights[i].specularColor;
    

    if(useDiffuseTexture) {
      FragColor += (ambient + diffuse) * texture(diffuseTexture, vec2(tex.x, 1 - tex.y));
    }
    else {
      FragColor += (ambient + diffuse) * vec4(objectColor, 1);
    }

    if(useSpecularTexture) {
      FragColor += specular * texture(specularTexture, vec2(tex.x, 1 - tex.y));
    }
    else {
      FragColor += specular;
    }
  }
}
