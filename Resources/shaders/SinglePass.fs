#version 330 core

layout (location = 0) out vec4 fColor;

uniform sampler3D u_sDensityMap;
//uniform sampler1D u_sTFunction;
uniform float u_fFocalLength;
uniform vec2 u_vScreenSize;
uniform vec4 u_vRayOrigin;

uniform mat4 modelMat;
uniform mat4 viewMat;

const float g_maxDist = sqrt(2.0);
const int g_numSamples = 128;
const float g_stepSize = g_maxDist / float(g_numSamples);
const float g_intensityFactor = 5;

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

bool Intersect(Ray ray, AABB aabb, out float t0, out float t1)
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
  mat4 MV = viewMat * modelMat;
  vec3 ray_dir;
  ray_dir.xy = 2.0 * gl_FragCoord.xy / u_vScreenSize - 1.0;
  ray_dir.z = -u_fFocalLength;
  ray_dir = (MV * vec4(ray_dir, 0)).xyz;

  Ray eye = Ray(u_vRayOrigin.xyz, normalize(ray_dir));
  AABB bbox = AABB(vec3(-1.0), vec3(1.0));

  float t_near, t_far;
  Intersect(eye, bbox, t_near, t_far);
  if(t_near < 0.0) t_near = 0.0;

  vec3 ray_start = eye.origin + eye.dir * t_near;
  vec3 ray_end = eye.origin + eye.dir * t_far;
  ray_start = 0.5 * (ray_start + 1.0);
  ray_end = 0.5 * (ray_end + 1.0);

  vec3 pos = ray_start;
  vec3 step = normalize(ray_end - ray_start) * g_stepSize;
  float dist = distance(ray_end, ray_start);
  float alpha = 1.0;
  vec3 color = vec3(0.f);

  for(int i = 0; i < g_numSamples && dist > 0.0; i++, pos += step, dist -= g_stepSize) {
    float intensity = texture(u_sDensityMap, pos).r * g_intensityFactor;
    if(intensity <= 0.0)
      continue;
    
    alpha *= 1.0 - intensity * g_stepSize;
    if(alpha <= 0.01)
      break;

    color += alpha * intensity * g_stepSize;
  }
  
  fColor = vec4(color, 1-alpha);
}