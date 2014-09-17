#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logger.h"
#include "tinygl.h"
#include "datasetmanager.h"
#include "mouse.h"
#include "arcball.h"
#include "cube.h"

extern "C"
{
#include "perlin.h"
}

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

GLFWwindow* g_window;
static Arcball* g_mouse;

glm::vec3 g_eye;
glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;

static void initMesh();
static void initFBO();
static void initShaders();
static void cb_drawpoint(size_t num_points);
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
  g_eye = glm::vec3(0, 0, 4.5);
  g_viewMatrix = glm::lookAt(g_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  g_projMatrix = glm::ortho(-2.f, 2.f, -2.f, 2.f, 1.f, 20.f);//glm::perspective(0.7f, static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H), 1.f, 10.f);

  DatasetManager::getInstance()->Init("C:/Users/schardong/Pictures/datasets/");
  glActiveTexture(GL_TEXTURE0);
  DatasetManager::getInstance()->SetActiveDataset("neghip");
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
  //initFBO();
  initShaders();
  
  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
  g_mouse = new Arcball(WINDOW_W, WINDOW_H, 1.5f, true, true);
  
  while(!glfwWindowShouldClose(g_window)) {

    TinyGL* gl_ptr = TinyGL::getInstance();

    Mesh* m = gl_ptr->getMesh("proxy_cube");
    Shader* fpass = gl_ptr->getShader(FPASS_KEY);
    fpass->bind();

    glm::mat4 rot = g_viewMatrix * g_mouse->createViewRotationMatrix();
    fpass->setUniformMatrix("u_mView", rot);

    //glm::mat4 rot = m->m_modelMatrix * g_mouse->createModelRotationMatrix(g_viewMatrix);
    //fpass->setUniformMatrix("u_mModel", rot);

    glm::mat3 invMV = glm::transpose(glm::mat3(rot)*glm::mat3(m->m_modelMatrix));
    glm::vec3 ray_origin = invMV * g_eye;
    fpass->setUniform4fv("u_vRayOrigin", glm::vec4(ray_origin, 1));

    //if(g_mouse->curr_x != g_mouse->last_x || g_mouse->curr_y != g_mouse->last_y) {

    //  glm::vec3 va = glm::normalize(get_arcball_vector(g_mouse->last_x, g_mouse->last_y));
    //  glm::vec3 vb = glm::normalize(get_arcball_vector(g_mouse->curr_x, g_mouse->curr_y));
    //  float angle = std::acos(std::min(1.f, glm::dot(va, vb))) / 50;
    //  glm::vec3 axis_camera = glm::cross(va, vb);

    //  g_viewMatrix = glm::rotate(g_viewMatrix, glm::degrees(angle), axis_camera);
    //  glm::mat3 invMV = glm::transpose(glm::mat3(g_viewMatrix) * glm::mat3(m->m_modelMatrix));
    //  glm::vec3 axis_obj = invMV * axis_camera;

    //  //m->m_modelMatrix = glm::rotate(m->m_modelMatrix, glm::degrees(angle), axis_obj);
    //  
    //  glm::vec3 ray_origin = invMV * g_eye;

    //  fpass->bind();
    //  fpass->setUniformMatrix("u_mProj", g_projMatrix);
    //  fpass->setUniformMatrix("u_mView", g_viewMatrix);
    //  fpass->setUniformMatrix("u_mModel", m->m_modelMatrix);
    //  fpass->setUniform4fv("u_vRayOrigin", glm::vec4(ray_origin, 1));

    //  g_mouse->last_x = g_mouse->curr_x;
    //  g_mouse->last_y = g_mouse->curr_y;
    //}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m->draw();

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
  GLfloat vertex[] = {
    0, 0, 0
  };

  Mesh* cube = new Mesh;
  cube->m_modelMatrix = glm::mat4(1.f);

  BufferObject* vbuff = new BufferObject(GL_ARRAY_BUFFER, sizeof(vertex), GL_STATIC_DRAW);
  vbuff->sendData(&vertex[0]);
  cube->attachBuffer(vbuff);

  cube->bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  cube->setNumPoints(1);
  cube->setDrawCb(cb_drawpoint);

  /*Cube* cube = new Cube();
  cube->m_modelMatrix = glm::mat4(1);
  cube->setDrawCb(cb_drawcube);*/

  TinyGL::getInstance()->addResource(MESH, "proxy_cube", cube);
}

static void initFBO()
{
  FramebufferObject* fbo = new FramebufferObject;
  fbo->bind(GL_FRAMEBUFFER);

  GLuint back_faces_id;
  GLuint depth_buff_id;

  glGenTextures(1, &back_faces_id);

  glBindTexture(GL_TEXTURE_2D, back_faces_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_W, WINDOW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  fbo->attachTexBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, back_faces_id, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &depth_buff_id);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buff_id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WINDOW_W, WINDOW_H);
  fbo->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buff_id);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  fbo->checkStatus();

  GLenum draw_buff[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buff);

  FramebufferObject::unbind();
  TinyGL::getInstance()->addResource(FRAMEBUFFER, FBO_KEY, fbo);
}

static void initShaders()
{
  float screen_sz[2] = {WINDOW_W, WINDOW_H};
  glm::mat4 modelMatrix = TinyGL::getInstance()->getMesh("proxy_cube")->m_modelMatrix;
  glm::mat4 MV = g_viewMatrix * modelMatrix;
  glm::vec4 ray_origin = glm::transpose(MV) * glm::vec4(g_eye, 1);

  Shader* fpass = new Shader(RESOURCE_PATH + "/shaders/SinglePass.vs", RESOURCE_PATH + "/shaders/SinglePass.fs", RESOURCE_PATH + "/shaders/SinglePass.gs");
  fpass->bind();
  fpass->bindFragDataLoc("fColor", 0);
  fpass->setUniformMatrix("u_mView", g_viewMatrix);
  fpass->setUniformMatrix("u_mProj", g_projMatrix);
  fpass->setUniformMatrix("u_mModel", modelMatrix);
  fpass->setUniformfv("u_vScreenSize", screen_sz, 2);
  fpass->setUniform1i("u_sDensityMap", 0);
  fpass->setUniform4fv("u_vRayOrigin", ray_origin);
  fpass->setUniform1f("u_fFocalLength", 1.0f / tan(0.7f / 2.f));

  Shader::unbind();

  TinyGL::getInstance()->addResource(SHADER, FPASS_KEY, fpass);
}

static void cb_keyboard(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  bool cam_changed = false;
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
    case GLFW_KEY_LEFT:
      g_eye -= glm::vec3(1, 0, 0);
      cam_changed = true;
      break;
    case GLFW_KEY_RIGHT:
      g_eye += glm::vec3(1, 0, 0);
      cam_changed = true;
      break;
    }
  }

  if(cam_changed) {
    g_viewMatrix = glm::lookAt(g_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    Shader* s = TinyGL::getInstance()->getShader(FPASS_KEY);
    s->bind();
    s->setUniformMatrix("viewMat", g_viewMatrix);
    s = TinyGL::getInstance()->getShader(SPASS_KEY);
    s->bind();
    s->setUniformMatrix("viewMat", g_viewMatrix);
    Shader::unbind();
  }
}

static void cb_mousebutton(GLFWwindow* win, int button, int action, int mods)
{
  g_mouse->mouseButtonCallback(win, button, action, mods);
  /*if(action == GLFW_PRESS) {
    switch(button)  {
    case GLFW_MOUSE_BUTTON_LEFT:
      g_mouse->last_x = g_mouse->curr_x;
      g_mouse->last_y = g_mouse->curr_y;
      g_arcball = true;
      break;
    default:
      g_arcball = false;
      break;
    }
  } else {
    g_arcball = false;
  }*/
}

static void cb_mousemotion(GLFWwindow* win, double xpos, double ypos)
{
  g_mouse->cursorCallback(win, xpos, ypos);
  /*if(g_arcball) {
    g_mouse->curr_x = (int)xpos;
    g_mouse->curr_y = (int)ypos;
  }*/
}

static void cb_drawcube(size_t num_points)
{
  glDrawArrays(GL_TRIANGLES, 0, num_points);
}

static void cb_drawpoint(size_t num_points)
{
  glDrawArrays(GL_POINTS, 0, num_points);
}