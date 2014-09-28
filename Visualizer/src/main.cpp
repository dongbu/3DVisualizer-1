#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logger.h"
#include "tinygl.h"
#include "arcball.h"
#include "cube.h"
#include "perlin.h"
#include "topanalyzer.h"
#include "alphamanager.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

GLFWwindow* g_window;
static Arcball* g_mouse;

glm::vec3 g_eye;
glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;
static GLuint g_numSamples = 356;

static void initMesh();
static void initFBO();
static void initShaders();
static void cb_drawpoint(size_t num_points);
static void cb_drawcube_idx(size_t num_points);
static void cb_drawcube(size_t num_points);
static void cb_keyboard(GLFWwindow*, int, int, int, int);
static void cb_mousebutton(GLFWwindow*, int, int, int);
static void cb_mousemotion(GLFWwindow*, double, double);

GLuint initGLFW()
{
  if(!glfwInit())
    return 1;

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  g_window = glfwCreateWindow(WINDOW_W, WINDOW_H, "First window", NULL, NULL);

  if(!g_window) {
    glfwTerminate();
    return 2;
  }

  glfwMakeContextCurrent(g_window);
  glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetKeyCallback(g_window, cb_keyboard);
  glfwSetMouseButtonCallback(g_window, cb_mousebutton);
  glfwSetCursorPosCallback(g_window, cb_mousemotion);
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
  using namespace knl;
  g_eye = glm::vec3(0, 0, 3.5);
  g_viewMatrix = glm::lookAt(g_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  g_projMatrix = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 1.f, 5.f);

  DatasetManager::getInstance()->Init("C:/Users/Guilherme/Pictures/datasets/");

  Dataset* pyro1 = Dataset::CreatePyroclasticVolume(128, 0.05f);
  Dataset* pyro2 = Dataset::CreatePyroclasticVolume(64, 0.4f);
  Dataset* pyro3 = Dataset::CreatePyroclasticVolume(64, 0.14f);

  DatasetManager::getInstance()->Add("pyro1", pyro1);
  DatasetManager::getInstance()->Add("pyro2", pyro2);
  DatasetManager::getInstance()->Add("pyro3", pyro3);

  DatasetManager::getInstance()->SetActive("fuel", GL_TEXTURE1);

  TFManager::getInstance()->Init("C:/Users/Guilherme/Pictures/datasets/transfer-functions/");
  TFManager::getInstance()->SetActive("tff1", GL_TEXTURE3);

  TopAnalyzer::getInstance()->AnalyzeCurrDataset();
  AlphaManager::getInstance()->SetActive("alpha_map1", GL_TEXTURE2);

  //TFManager::getInstance()->SetActive("tf1", GL_TEXTURE1);

  delete pyro1;
  delete pyro2;
  delete pyro3;
}

int main()
{
  Logger::getInstance()->setLogStream(&std::cout);

  GLuint errCode = initGLFW();
  if(errCode != 0)
    return -1;

  errCode = initGLEW();
  initResources();
  initMesh();
  
  initFBO();
  initShaders();
  
  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1.0f);

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
  g_mouse = new Arcball(WINDOW_W, WINDOW_H, 1.5f, true, true);
  
  while(!glfwWindowShouldClose(g_window)) {

    TinyGL* gl_ptr = TinyGL::getInstance();
    Mesh* m = gl_ptr->getMesh("proxy_cube");
    Shader* fpass = gl_ptr->getShader(FPASS_KEY);
    Shader* spass = gl_ptr->getShader(SPASS_KEY);
    FramebufferObject* fbo = gl_ptr->getFBO(FBO_KEY);

    fpass->bind();
    glm::mat4 rot = g_viewMatrix * g_mouse->createViewRotationMatrix();
    fpass->setUniformMatrix("u_mView", rot);

    //First pass
    fbo->bind(GL_FRAMEBUFFER);
    glViewport(0, 0, WINDOW_W, WINDOW_H);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    m->draw();

    FramebufferObject::unbind();
    glViewport(0, 0, WINDOW_W, WINDOW_H);

    //Second pass
    spass->bind();
    spass->setUniformMatrix("u_mView", rot);

    DatasetManager::getInstance()->GetCurrent()->bind(GL_TEXTURE1);
    AlphaManager::getInstance()->GetCurrent()->bind(GL_TEXTURE2);
    TFManager::getInstance()->GetCurrent()->bind(GL_TEXTURE3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->getAttachmentId(GL_COLOR_ATTACHMENT0));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    m->draw();

    Shader::unbind();

    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  DatasetManager::getInstance()->FreeResources();
  TinyGL::getInstance()->freeResources();
  delete g_mouse;

  glfwTerminate();
  return 0;
}

static void initMesh()
{
  Cube* cube = new Cube();
  cube->m_modelMatrix = glm::mat4(1);
  cube->setDrawCb(cb_drawcube_idx);

  TinyGL::getInstance()->addResource(MESH, "proxy_cube", cube);
}

static void initFBO()
{
  FramebufferObject* fbo = new FramebufferObject;
  fbo->bind(GL_FRAMEBUFFER);

  GLuint ids[2];

  glGenTextures(1, &ids[0]);

  glBindTexture(GL_TEXTURE_2D, ids[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_W, WINDOW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  fbo->attachTexBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ids[0], 0);

  glGenRenderbuffers(1, &ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, ids[1]);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WINDOW_W, WINDOW_H);
  fbo->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  fbo->checkStatus();

  GLenum draw_buff[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buff);

  FramebufferObject::unbind();
  TinyGL::getInstance()->addResource(FRAMEBUFFER, FBO_KEY, fbo);
}

static void initShaders()
{
  float screen_sz[2] = {(float)WINDOW_W, (float)WINDOW_H};
  glm::mat4 modelMatrix = TinyGL::getInstance()->getMesh("proxy_cube")->m_modelMatrix;

  Shader* fpass = new Shader(RESOURCE_PATH + "/shaders/FPass.vs", RESOURCE_PATH + "/shaders/FPass.fs");
  fpass->bind();
  fpass->bindFragDataLoc("fColor", 0);
  fpass->setUniformMatrix("u_mView", g_viewMatrix);
  fpass->setUniformMatrix("u_mProj", g_projMatrix);
  fpass->setUniformMatrix("u_mModel", modelMatrix);

  /*Shader* spass = new Shader(RESOURCE_PATH + "/shaders/SPass.vs", RESOURCE_PATH + "/shaders/SPass.fs");
  spass->bind();
  spass->bindFragDataLoc("fColor", 0);
  spass->setUniformMatrix("u_mView", g_viewMatrix);
  spass->setUniformMatrix("u_mProj", g_projMatrix);
  spass->setUniformMatrix("u_mModel", modelMatrix);
  spass->setUniformfv("u_vScreenSize", screen_sz, 2);
  spass->setUniform1i("u_sDensityMap", 0);
  spass->setUniform1i("u_sTransferFunction", 1);
  spass->setUniform1i("u_sBackFaces", 2);
  spass->setUniform1f("u_fNumSamples", (float)g_numSamples);*/

  Shader* spass = new Shader(RESOURCE_PATH + "/shaders/SPass.vs", RESOURCE_PATH + "/shaders/SPassMultiOp.fs");
  spass->bind();
  spass->bindFragDataLoc("fColor", 0);
  spass->setUniformMatrix("u_mView", g_viewMatrix);
  spass->setUniformMatrix("u_mProj", g_projMatrix);
  spass->setUniformMatrix("u_mModel", modelMatrix);
  spass->setUniformfv("u_vScreenSize", screen_sz, 2);
  spass->setUniform1f("u_fNumSamples", (float)g_numSamples);
  spass->setUniform1i("u_sBackFaces", 0);
  spass->setUniform1i("u_sDensityMap", 1);
  spass->setUniform1i("u_sAlphaMap", 2);
  spass->setUniform1i("u_sColorTFunction", 3);

  Shader::unbind();

  TinyGL::getInstance()->addResource(SHADER, FPASS_KEY, fpass);
  TinyGL::getInstance()->addResource(SHADER, SPASS_KEY, spass);
}

static void cb_keyboard(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  glActiveTexture(GL_TEXTURE0);
  if(action == GLFW_PRESS) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    case GLFW_KEY_SPACE:
      TopAnalyzer::getInstance()->AnalyzeCurrDataset();
      AlphaManager::getInstance()->SetActive("alpha_map1", GL_TEXTURE2);
      break;
    case GLFW_KEY_1:
      TFManager::getInstance()->SetActive("tff1", GL_TEXTURE3);
      break;
    case GLFW_KEY_2:
      TFManager::getInstance()->SetActive("tff2", GL_TEXTURE3);
      break;
    case GLFW_KEY_F1:
      DatasetManager::getInstance()->SetActive("neghip", GL_TEXTURE1);
      break;
    case GLFW_KEY_F2:
      DatasetManager::getInstance()->SetActive("bonsai", GL_TEXTURE1);
      break;
    case GLFW_KEY_F3:
      DatasetManager::getInstance()->SetActive("nucleon", GL_TEXTURE1);
      break;
    case GLFW_KEY_F4:
      DatasetManager::getInstance()->SetActive("silicium", GL_TEXTURE1);
      break;
    case GLFW_KEY_F5:
      DatasetManager::getInstance()->SetActive("pyro1", GL_TEXTURE1);
      break;
    case GLFW_KEY_F6:
      DatasetManager::getInstance()->SetActive("pyro2", GL_TEXTURE1);
      break;
    case GLFW_KEY_F7:
      DatasetManager::getInstance()->SetActive("pyro3", GL_TEXTURE1);
      break;
    case GLFW_KEY_F8:
      DatasetManager::getInstance()->SetActive("foot", GL_TEXTURE1);
      break;
    case GLFW_KEY_F9:
      DatasetManager::getInstance()->SetActive("virgo", GL_TEXTURE1);
      break;
    }
  }
}

static void cb_mousebutton(GLFWwindow* win, int button, int action, int mods)
{
  g_mouse->mouseButtonCallback(win, button, action, mods);
}

static void cb_mousemotion(GLFWwindow* win, double xpos, double ypos)
{
  g_mouse->cursorCallback(win, xpos, ypos);
}

static void cb_drawcube_idx(size_t num_points)
{
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}