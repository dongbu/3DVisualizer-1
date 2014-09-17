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

static GLuint createCubeVAO()
{
  GLfloat cube_vertex[24] = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0
  };

  GLuint cube_index[36] = {
    1,5,7,
    7,3,1,
    0,2,6,
    6,4,0,
    0,1,3,
    3,2,0,
    7,5,4,
    4,6,7,
    2,3,7,
    7,6,2,
    1,0,4,
    4,5,1
  };

  GLuint vao, vbo, ibo;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, cube_vertex, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, cube_index, GL_STATIC_DRAW);

  glBindVertexArray(0);

  return vao;
}

void renderCubeVAO(GLuint cube_id)
{
  glBindVertexArray(cube_id);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
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