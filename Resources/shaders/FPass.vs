#version 330

layout (location = 0) in vec3 in_vPosition;

out vec3 vColor;

uniform mat4 u_mModel;
uniform mat4 u_mView;
uniform mat4 u_mProj;

void main(void)
{
  mat4 MVP = u_mProj * u_mView * u_mModel;
  vColor = in_vPosition + 0.5;
  gl_Position = MVP * vec4(in_vPosition, 1);
}