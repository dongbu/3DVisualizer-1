#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logger.h"
#include "cube.h"
#include "tinygl.h"
#include "datahandler.h"
#include "datatransfer.h"
#include "datasetmanager.h"
#include "dataset.h"

extern "C"
{
#include "perlin.h"
}

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

GLFWwindow* g_window;
Shader* g_simple;

glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;

GLuint tex_id;

void cb_drawpoint(size_t num_points)
{
  glDrawArrays(GL_POINTS, 0, num_points);
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

static void cb_keyboard(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  if(action == GLFW_PRESS) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    case GLFW_KEY_1:
      DatasetManager::getInstance()->SetActiveDataset("neghip");
      break;
    case GLFW_KEY_2:
      DatasetManager::getInstance()->SetActiveDataset("bonsai");
      break;
    case GLFW_KEY_3:
      DatasetManager::getInstance()->SetActiveDataset("nucleon");
      break;
    case GLFW_KEY_4:
      DatasetManager::getInstance()->SetActiveDataset("silicium");
      break;
    }
  }
}

GLuint initGLFW()
{
  if(!glfwInit())
    return 1;

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g_window = glfwCreateWindow(WINDOW_W, WINDOW_H, "First window", NULL, NULL);

  if(!g_window) {
    glfwTerminate();
    return 2;
  }

  glfwMakeContextCurrent(g_window);
  glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetKeyCallback(g_window, cb_keyboard);
  return 0;
}

GLuint initGLEW()
{
  glewExperimental = true;
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    std::cerr << glewGetErrorString(err) << std::endl;
    return 1;
  }

  return 0;
}

void initResources()
{
  float screen_sz[2] = {WINDOW_W, WINDOW_H};
  glm::vec4 cam_pos = glm::vec4(0, 0, 4, 1);
  g_viewMatrix = glm::lookAt(glm::vec3(cam_pos), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  g_projMatrix = glm::perspective(0.7f, static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H), 0.1f, 10.f);

  glm::mat4 modelMatrix = glm::mat4(1.f);
  glm::mat4 MV = g_viewMatrix * modelMatrix;
  glm::vec4 ray_origin = glm::transpose(MV) * cam_pos;

  g_simple = new Shader(RESOURCE_PATH + "/shaders/SinglePass.vs", RESOURCE_PATH + "/shaders/SinglePass.fs", RESOURCE_PATH + "/shaders/SinglePass.gs");
  g_simple->bind();
  g_simple->bindFragDataLoc("fColor", 0);
  g_simple->setUniformMatrix("viewMat", g_viewMatrix);
  g_simple->setUniformMatrix("projMat", g_projMatrix);
  g_simple->setUniformMatrix("modelMat", modelMatrix);
  g_simple->setUniformfv("u_vScreenSize", screen_sz, 2);
  g_simple->setUniform1i("u_sDensityMap", 0);
  g_simple->setUniform4fv("u_vRayOrigin", ray_origin);
  g_simple->setUniform1f("u_fFocalLength", 1.0f / tan(0.7f / 2.f));

  DatasetManager::getInstance()->Init("C:/Users/schardong/Pictures/datasets/");
  glActiveTexture(GL_TEXTURE0);
  DatasetManager::getInstance()->SetActiveDataset("neghip");
  //  DatasetManager::getInstance()->SetActiveDataset("foot");
  //  DatasetManager::getInstance()->SetActiveDataset("bonsai");
  //  DatasetManager::getInstance()->SetActiveDataset("nucleon");
}

int main()
{
  Logger::getInstance()->setLogStream(&std::cout);

  GLuint errCode = initGLFW();
  if(errCode != 0)
    return -1;

  errCode = initGLEW();
  initResources();
  
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  GLfloat vertex[] = {
    0, 0, 0
  };

  Mesh* cube = new Mesh;
  BufferObject* vbuff = new BufferObject(GL_ARRAY_BUFFER, sizeof(vertex), GL_STATIC_DRAW);
  vbuff->sendData(&vertex[0]);
  cube->attachBuffer(vbuff);

  cube->bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  cube->setNumPoints(1);
  cube->setDrawCb(cb_drawpoint);
  
  TinyGL::getInstance()->addResource(MESH, "proxy_cube", cube);
  TinyGL::getInstance()->addResource(SHADER, "simple", g_simple);
  TinyGL::getInstance()->getShader("simple")->bind();
  
  while(!glfwWindowShouldClose(g_window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    TinyGL::getInstance()->getMesh("proxy_cube")->draw();
    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  DatasetManager::getInstance()->FreeResources();
  TinyGL::getInstance()->freeResources();

  glfwTerminate();
  return 0;
}
