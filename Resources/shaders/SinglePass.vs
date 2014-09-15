#version 330

layout (location = 0) in vec3 in_vPosition;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec4 vPosition;

void main()
{
  mat4 MVP = projMat * viewMat * modelMat;
  vPosition = vec4(in_vPosition, 1);
  gl_Position = MVP * vec4(in_vPosition, 1);
}