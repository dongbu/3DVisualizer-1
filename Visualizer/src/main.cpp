#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "cube.h"
#include "tinygl.h"
#include "datahandler.h"
#include "datatransfer.h"
#include "datasetmanager.h"
#include "dataset.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

GLFWwindow* g_window;
Shader* g_simple;

glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;

void draw_cube(size_t num_points)
{
  glDrawArrays(GL_TRIANGLE_STRIP, 0, num_points);
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
  g_viewMatrix = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
  g_projMatrix = glm::perspective(static_cast<float>(M_PI / 4.f), static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H), 0.1f, 10.f);

  g_simple = new Shader(RESOURCE_PATH + "/shaders/simple.vs", RESOURCE_PATH + "/shaders/simple.fs");
  g_simple->bind();
  g_simple->bindFragDataLoc("fColor", 0);
  g_simple->setUniformMatrix("viewMatrix", g_viewMatrix);
  g_simple->setUniformMatrix("projMatrix", g_projMatrix);
  g_simple->setUniformMatrix("modelMatrix", glm::mat4(1));
  g_simple->setUniform4fv("inColor", glm::vec4(1.0, 0.0, 1.0, 1.0));

  DatasetManager::getInstance()->Init("C:/Users/Guilherme/Pictures/datasets/");
  DatasetManager::getInstance()->SetActiveDataset("stent");
  DatasetManager::getInstance()->FreeResources();
}

int main()
{
  
  GLuint errCode = initGLFW();
  if(errCode != 0)
    return -1;

  errCode = initGLEW();
  initResources();
  
  glClearColor(0.8f, 0.8f, 0.8f, 1.f);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  Cube* q = new Cube();
  q->setDrawCb(draw_cube);
  q->setMaterialColor(glm::vec4(1, 0, 1, 1));

  TinyGL::getInstance()->addResource(MESH, "test_quad", q);
  TinyGL::getInstance()->addResource(SHADER, "simple", g_simple);
  TinyGL::getInstance()->getShader("simple")->bind();
  
  while(!glfwWindowShouldClose(g_window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TinyGL::getInstance()->getMesh("test_quad")->draw();
    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
