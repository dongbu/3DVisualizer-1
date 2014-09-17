#version 330

layout (location = 0) in vec3 in_vPosition;

uniform mat4 u_mModel;
uniform mat4 u_mView;
uniform mat4 u_mProj;

out vec4 vPosition;

void main()
{
  mat4 MVP = u_mProj * u_mView * u_mModel;
  vPosition = vec4(in_vPosition, 1);
  gl_Position = MVP * vec4(in_vPosition, 1);
}