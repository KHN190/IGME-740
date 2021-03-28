#version 430

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;

void main() {
  // hard coded attrs
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 objColor = vec3(0.5, 0.1, 0.2);

  // calculate light attrs
  vec3 lightDir = normalize(lightPos - vertex_position);
  float lambert = max(dot(vertex_normal, lightDir), 0.0);

  vec3 diffuse  = lambert * lightColor;
  color = diffuse * objColor;

  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}
