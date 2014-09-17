#version 330

layout (location = 0) in vec3 in_vPosition;

out vec3 vEntryPoint;
out vec4 vExitPoint;

uniform mat4 u_mProj;
uniform mat4 u_mView;
uniform mat4 u_mModel;

void main(void)
{
  mat4 MVP = u_mProj * u_mView * u_mModel;
  gl_Position = MVP * vec4(in_vPosition, 1.f);
  vEntryPoint = in_vPosition + 0.5;
  vExitPoint = gl_Position;
}
