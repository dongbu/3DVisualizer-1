#version 330 core

layout (location = 0) out vec4 fColor;

uniform sampler3D u_sDensityMap;
//uniform sampler1D u_sTFunction;
//uniform float u_fFocalLength;
//uniform vec2 u_vScreenSize;
//uniform vec3 u_vRayOrigin;

in vec4 vPosition;

struct Ray
{
  vec3 origin;
  vec3 dir;
};

struct AABB
{
  vec3 min;
  vec3 max;
};

bool Intersect(Ray ray, AABB aabb, out float t0, float t1)
{
  vec3 invR = 1.0 / ray.dir;
  vec3 tbot = invR * (aabb.min - ray.origin);
  vec3 ttop = invR * (aabb.max - ray.origin);
  vec3 tmin = min(ttop, tbot);
  vec3 tmax = max(ttop, tbot);
  vec2 t = max(tmin.xx, tmin.yz);
  t0 = max(t.x, t.y);
  t = min(tmax.xx, tmax.yz);
  t1 = min(t.x, t.y);
  return t0 <= t1;
}

void main()
{
  fColor = vec4(texture(u_sDensityMap, vPosition));
}