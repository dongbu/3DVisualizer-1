#version 330

layout (location = 0) out vec4 fColor;

in vec3 vColor;

void main(void)
{
  fColor = vec4(vColor.rgb, 1.f);
}