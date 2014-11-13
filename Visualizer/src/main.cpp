#include <iostream>
#include <cstdio>
#include <GL/glew.h>

#ifdef WIN32
  #include <iup.h>
  #include <iupgl.h>
  #include <iupkey.h>
#else
  #include <iup/iup.h>
  #include <iup/iupgl.h>
  #include <iup/iupkey.h>
#endif

#include "config.h"
#include "glslrenderer.h"
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

static Arcball* g_mouse;
Ihandle* canvas;

static GLSLRenderer* rend = NULL;

glm::vec3 g_eye;
glm::mat4 g_viewMatrix;
glm::mat4 g_projMatrix;
static GLuint g_numSamples = 356;
static double g_flowRate = 350.0;

static void testSimplification();
static void initResources();
static GLuint initGLEW();
static void initMesh();
static void initFBO();
static void initShaders();
static void cb_drawcube_idx(size_t);

static int cb_idle();
static int cb_keypress(Ihandle*, int, int);
static int cb_mousebutton(Ihandle*, int, int, int, int, char*);
static int cb_mousemotion(Ihandle*, int, int, char*);
static int cb_draw(Ihandle*, float, float);

int main(int argc, char** argv)
{
  Logger::GetInstance()->setLogStream(&std::cout);

  Ihandle* dlg;

  IupOpen(&argc, &argv);
  IupGLCanvasOpen();

  canvas = IupGLCanvas(NULL);

  IupSetFunction("IDLE_ACTION", (Icallback) cb_idle);
  IupSetCallback(canvas, "ACTION", (Icallback) cb_draw);
  IupSetCallback(canvas, "KEYPRESS_CB", (Icallback) cb_keypress);

  IupSetCallback(canvas, "BUTTON_CB", (Icallback) cb_mousebutton);
  IupSetCallback(canvas, "MOTION_CB", (Icallback) cb_mousemotion);

  //  IupSetCallback(canvas, "WHEEL_CB", (Icallback) cb_mousewheel);

  IupSetAttribute(canvas, IUP_BUFFER, IUP_DOUBLE);

  char* rastersize = (char*) calloc(30, sizeof(char));
  sprintf(rastersize, "%dx%d", WINDOW_W, WINDOW_H);

  IupSetAttribute(canvas, "RASTERSIZE", rastersize);
  free(rastersize);

  IupGLMakeCurrent(canvas);

  dlg = IupDialog(IupHbox(IupFill(), canvas, IupFill()));
  IupSetAttribute(dlg, "TITLE", "My first IUP window");
  IupMap(dlg);

  IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

  initGLEW();
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

  rend = new GLSLRenderer;
  rend->init();
  rend->Updating(true);

  //g_mouse = new Arcball(WINDOW_W, WINDOW_H, 0.05f, true, true);

  IupMainLoop();

  DatasetManager::GetInstance()->FreeResources();
  TFManager::GetInstance()->FreeResources();
  AlphaManager::GetInstance()->FreeResources();
  TinyGL::GetInstance()->freeResources();
  delete g_mouse;
  delete rend;

  IupClose();
  return 0;
}

GLuint initGLEW()
{
  glewExperimental = true;
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
    return 1;
  }

  return 0;
}

void testSimplification()
{
  DatasetManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/");

  std::cout << "\n\n\n--------------Neghip--------------\n";
  DatasetManager::GetInstance()->SetActive("neghip", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Hydrogen Atom--------------\n";
  DatasetManager::GetInstance()->SetActive("hydrogenAtom", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Shockwave--------------\n";
  DatasetManager::GetInstance()->SetActive("shockwave", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Bonsai--------------\n";
  DatasetManager::GetInstance()->SetActive("bonsai", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Foot--------------\n";
  DatasetManager::GetInstance()->SetActive("foot", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Boston Teapot--------------\n";
  DatasetManager::GetInstance()->SetActive("BostonTeapot", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------CT-Knee--------------\n";
  DatasetManager::GetInstance()->SetActive("CT-Knee", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());

  std::cout << "\n\n\n--------------Stent--------------\n";
  DatasetManager::GetInstance()->SetActive("stent8", GL_TEXTURE1);
  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
}

void initResources()
{
  using namespace knl;
  g_eye = glm::vec3(0, 0, 3.5);
  g_viewMatrix = glm::lookAt(g_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  g_projMatrix = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 1.f, 5.f);

  //  DatasetManager::GetInstance()->Init("C:/Users/schardong/Pictures/datasets/");
  DatasetManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/");
  DatasetManager::GetInstance()->SetActive("nucleon", GL_TEXTURE1);

  //  TFManager::GetInstance()->Init("C:/Users/schardong/Pictures/datasets/transfer-functions/");
  TFManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/transfer-functions/");
  TFManager::GetInstance()->SetActive("tff1", GL_TEXTURE3);

  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
  AlphaManager::GetInstance()->SetActive(DatasetManager::GetInstance()->GetCurrentKey(), GL_TEXTURE2);
}

static void initMesh()
{
  Cube* cube = new Cube();
  cube->m_modelMatrix = glm::mat4(1);
  cube->setDrawCb(cb_drawcube_idx);

  TinyGL::GetInstance()->addResource(MESH, "proxy_cube", cube);
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
  TinyGL::GetInstance()->addResource(FRAMEBUFFER, FBO_KEY, fbo);
}

static void initShaders()
{
  float screen_sz[2] = {(float)WINDOW_W, (float)WINDOW_H};
  glm::mat4 modelMatrix = TinyGL::GetInstance()->getMesh("proxy_cube")->m_modelMatrix;

  Shader* fpass = new Shader(std::string(RESOURCE_PATH) + std::string("/shaders/FPass.vs"),
                             std::string(RESOURCE_PATH) + std::string("/shaders/FPass.fs"));
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

  Shader* spass = new Shader(std::string(RESOURCE_PATH) + std::string("/shaders/SPass.vs"),
                             std::string(RESOURCE_PATH) + std::string("/shaders/SPassMultiOp.fs"));
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

  Shader::Unbind();

  TinyGL::GetInstance()->addResource(SHADER, FPASS_KEY, fpass);
  TinyGL::GetInstance()->addResource(SHADER, SPASS_KEY, spass);
}

static int cb_keypress(Ihandle*, int c, int press)
{
  if(rend) return rend->keypress(c, press);
//  if(press == 0) {
//    switch(c) {
//    case K_ESC:
//      return IUP_CLOSE;
//      break;
//    case K_SP:
//      TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//      AlphaManager::GetInstance()->SetActive(DatasetManager::GetInstance()->GetCurrentKey(), GL_TEXTURE2);
//      break;
//    case K_1:
//      TFManager::GetInstance()->SetActive("tff1", GL_TEXTURE3);
//      break;
//    case K_2:
//      TFManager::GetInstance()->SetActive("tff2", GL_TEXTURE3);
//      break;
//    case K_F1:
//      DatasetManager::GetInstance()->SetActive("neghip", GL_TEXTURE1);
//      break;
//    case K_F2:
//      DatasetManager::GetInstance()->SetActive("bonsai", GL_TEXTURE1);
//      break;
//    case K_F3:
//      DatasetManager::GetInstance()->SetActive("nucleon", GL_TEXTURE1);
//      break;
//    case K_F4:
//      DatasetManager::GetInstance()->SetActive("silicium", GL_TEXTURE1);
//      break;
//    case K_F5:
//      DatasetManager::GetInstance()->SetActive("fuel", GL_TEXTURE1);
//      break;
//    case K_F6:
//      DatasetManager::GetInstance()->SetActive("BostonTeapot", GL_TEXTURE1);
//      break;
//    case K_F7:
//      DatasetManager::GetInstance()->SetActive("lobster", GL_TEXTURE1);
//      break;
//    case K_F8:
//      DatasetManager::GetInstance()->SetActive("foot", GL_TEXTURE1);
//      break;
//    case K_F9:
//      DatasetManager::GetInstance()->SetActive("CT-Knee", GL_TEXTURE1);
//      break;
//    }
//  }
//  return IUP_DEFAULT;
}

static int cb_idle()
{
  return cb_draw(canvas, 0.f, 0.f);
}

static int cb_mousebutton(Ihandle* win, int button, int pressed, int a, int b, char* c)
{
  if(rend) return rend->mousebutton(button, pressed, a, b, c);
//  g_mouse->mouseButtonCallback(button, pressed);
  return IUP_DEFAULT;
}

static int cb_mousemotion(Ihandle* win, int xpos, int ypos, char* a)
{
  if(rend) return rend->mousemotion(xpos, ypos, a);
//  g_mouse->cursorCallback(static_cast<double>(xpos), static_cast<double>(ypos));
  return IUP_DEFAULT;
}

static int cb_draw(Ihandle* ih, float a, float b)
{
  int e = IUP_DEFAULT;
  IupGLMakeCurrent(ih);
  if(rend) e = rend->update();

//  IupGLMakeCurrent(ih);

//  TinyGL* gl_ptr = TinyGL::GetInstance();
//  Mesh* m = gl_ptr->getMesh("proxy_cube");
//  Shader* fpass = gl_ptr->getShader(FPASS_KEY);
//  Shader* spass = gl_ptr->getShader(SPASS_KEY);
//  FramebufferObject* fbo = gl_ptr->getFBO(FBO_KEY);

//  if(!fpass || !spass || !fbo || !m) return IUP_DEFAULT;

//  fpass->bind();
//  glm::mat4 rot = g_viewMatrix * g_mouse->createViewRotationMatrix();
//  fpass->setUniformMatrix("u_mView", rot);

  //First pass
//  fbo->bind(GL_FRAMEBUFFER);
//  glViewport(0, 0, WINDOW_W, WINDOW_H);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glCullFace(GL_FRONT);
//  m->draw();

//  FramebufferObject::unbind();
//  glViewport(0, 0, WINDOW_W, WINDOW_H);

  //Second pass
//  spass->bind();
//  spass->setUniformMatrix("u_mView", rot);

//  DatasetManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE1);
//  AlphaManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE2);
//  TFManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE3);

//  glActiveTexture(GL_TEXTURE0);
//  glBindTexture(GL_TEXTURE_2D, fbo->getAttachmentId(GL_COLOR_ATTACHMENT0));

//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glCullFace(GL_BACK);
//  m->draw();

//  Shader::Unbind();

  IupGLSwapBuffers(ih);
  return e;
}

static void cb_drawcube_idx(size_t)
{
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}
