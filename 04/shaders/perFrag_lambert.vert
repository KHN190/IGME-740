#version 430

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;
out vec3 normal;
out vec3 lightDir;
out vec3 viewDir;

void main() {

  lightDir = normalize(lightPos - vertex_position);
  viewDir  = normalize(viewPos - vertex_position);

  normal = vertex_normal;
  color = vec3(1.0, 1.0, 1.0);

  gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}
