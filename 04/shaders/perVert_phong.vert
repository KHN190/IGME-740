#version 430

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 viewPos;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;

float lambert(vec3 lightPos, vec3 vertex_position, vec3 vertex_normal) {
  vec3 lightDir = normalize(lightPos - vertex_position);
  vec3 reflectDir = reflect(-lightDir, vertex_normal);
  float lambert = max(dot(vertex_normal, lightDir), 0.0);
  return lambert;
}

float spec(vec3 viewPos, vec3 lightPos, vec3 vertex_position, vec3 vertex_normal) {
  vec3 viewDir = normalize(viewPos - vertex_position);
  vec3 lightDir = normalize(lightPos - vertex_position);
  vec3 reflectDir = reflect(-lightDir, vertex_normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20.0);
  return spec;
}

void main() {
  // hard coded attrs
  float ambient_strength = 1.5;
  vec3 l_ambient0 = vec3(0.0, 0.15, 1.0);
  vec3 l_diffuse0 = vec3(1.0, 1.0, 0.0);
  vec3 l_specular = vec3(0.0, 0.0, 1.0);
  vec3 l_ambient1 = vec3(0.0, 0.0, 0.15);
  vec3 l_diffuse1 = vec3(1.0, 0.0, 1.0);
  vec3 objColor = vec3(0.5, 0.2, 0.4);

  // calculate light attrs
  // use per vertex normal
  float spec0 = spec(viewPos, lightPos0, vertex_position, vertex_normal);
  float spec1 = spec(viewPos, lightPos1, vertex_position, vertex_normal);
  float lambert0 = lambert(lightPos0, vertex_position, vertex_normal);
  float lambert1 = lambert(lightPos1, vertex_position, vertex_normal);

  // Phong shading
  vec3 ambient  = (l_ambient0 * 0.5 + l_ambient1 * 0.5) * ambient_strength;
  vec3 specular = spec0 * l_specular * 0.5 + spec1 * l_specular * 0.5;
  vec3 diffuse  = lambert0 * l_diffuse0 * 0.5 + lambert1 * l_diffuse1 * 0.5;
  color = (ambient + specular + diffuse) * objColor;

  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}
