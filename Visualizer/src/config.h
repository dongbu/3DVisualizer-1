#ifndef VISUALIZER_H
#define VISUALIZER_H

static int WINDOW_W = 700;
static int WINDOW_H = 700;

#define M_PI 3.14159265358979323846

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

static const std::string FPASS_KEY = "first_pass";
static const std::string SPASS_KEY = "second_pass";
static const std::string FBO_KEY = "back_faces";

#ifdef WIN32
  static const std::string RESOURCE_PATH = "../Resources";
#else
  static const char* RESOURCE_PATH = "../../Resources";
#endif // WIN32

extern "C"
{
#include "perlin.h"
}

static GLuint CreatePyroclasticVolume(int n, float r)
{
  GLuint handle;
  glGenTextures(1, &handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, handle);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  unsigned char *data = new unsigned char[n*n*n];
  unsigned char *ptr = data;

  float frequency = 3.0f / n;
  float center = n / 2.0f + 0.5f;

  for(int x=0; x < n; ++x) {
    for (int y=0; y < n; ++y) {
      for (int z=0; z < n; ++z) {
	float dx = center-x;
	float dy = center-y;
	float dz = center-z;
       
	float off = fabsf((float) PerlinNoise3D(
						x*frequency,
						y*frequency,
						z*frequency,
						5,
						6, 3));

	float d = sqrtf(dx*dx+dy*dy+dz*dz)/(n);
	bool isFilled = (d-off) < r;
	*ptr++ = isFilled ? 255 : 0;
            }
    }
  }

  glTexImage3D(GL_TEXTURE_3D, 0,
	       GL_R8,
	       n, n, n, 0,
	       GL_RED,
	       GL_UNSIGNED_BYTE,
	       data);

    delete[] data;
    return handle;
}

glm::vec3 get_arcball_vector(int x, int y) {
  glm::vec3 P = glm::vec3(1.0*x/WINDOW_W*2 - 1.0,
			  1.0*y/WINDOW_H*2 - 1.0,
			  0);
  P.y = -P.y;
  float OP_squared = P.x * P.x + P.y * P.y;
  if (OP_squared <= 1*1)
    P.z = sqrt(1*1 - OP_squared);  // Pythagore
  else
    P = glm::normalize(P);  // nearest point
  return P;
}

#endif // VISUALIZER_H