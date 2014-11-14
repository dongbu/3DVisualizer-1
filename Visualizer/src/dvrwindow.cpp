#include "dvrwindow.h"
#include "datasetmanager.h"
#include "tfmanager.h"
#include "topanalyzer.h"
#include "alphamanager.h"
#include "renderermanager.h"
#include "tinygl.h"
#include "cube.h"
#include "arcball.h"
#include "config.h"
#include "glslrenderer.h"

static void cb_drawcube_idx(size_t)
{
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}

DVRWindow::DVRWindow(int w, int h) :
  m_frame(0),
  m_numSamples(256),
  m_flowRate(300.f)
{
  SetWidth(w);
  SetHeight(h);
}

DVRWindow::~DVRWindow()
{
  DatasetManager::GetInstance()->FreeResources();
  TFManager::GetInstance()->FreeResources();
  AlphaManager::GetInstance()->FreeResources();
  TinyGL::GetInstance()->freeResources();
}

int DVRWindow::init()
{
  InitRenderer();
  InitResources();
  InitMesh();
  InitFBO();
  InitShaders();

  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1.0f);

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return 0;
}

void DVRWindow::draw()
{
  RendererManager::GetInstance()->GetCurrent()->draw();
}

void DVRWindow::keypress(int key, int press)
{
  RendererManager::GetInstance()->GetCurrent()->keypress(key, press);
  if(press == 0) {
    switch(key) {
    case K_ESC:
      break;
    case K_SP:
      TopAnalyzer::GetInstance()->AnalyzeCurrDataset(m_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
      AlphaManager::GetInstance()->SetActive(DatasetManager::GetInstance()->GetCurrentKey(), GL_TEXTURE2);
      break;
    case K_1:
      TFManager::GetInstance()->SetActive("tff1", GL_TEXTURE3);
      break;
    case K_2:
      TFManager::GetInstance()->SetActive("tff2", GL_TEXTURE3);
      break;
    case K_F1:
      DatasetManager::GetInstance()->SetActive("neghip", GL_TEXTURE1);
      break;
    case K_F2:
      DatasetManager::GetInstance()->SetActive("bonsai", GL_TEXTURE1);
      break;
    case K_F3:
      DatasetManager::GetInstance()->SetActive("nucleon", GL_TEXTURE1);
      break;
    case K_F4:
      DatasetManager::GetInstance()->SetActive("silicium", GL_TEXTURE1);
      break;
    case K_F5:
      DatasetManager::GetInstance()->SetActive("fuel", GL_TEXTURE1);
      break;
    case K_F6:
      DatasetManager::GetInstance()->SetActive("BostonTeapot", GL_TEXTURE1);
      break;
    case K_F7:
      DatasetManager::GetInstance()->SetActive("lobster", GL_TEXTURE1);
      break;
    case K_F8:
      DatasetManager::GetInstance()->SetActive("foot", GL_TEXTURE1);
      break;
    case K_F9:
      DatasetManager::GetInstance()->SetActive("CT-Knee", GL_TEXTURE1);
      break;
    }
  }
}

void DVRWindow::mousebutton(int button, int press, int x, int y)
{
  RendererManager::GetInstance()->GetCurrent()->mousebutton(button, press, x, y);
  std::cout << button << " " << press << " " << x << " " << y << std::endl;
}

void DVRWindow::mousemove(int x, int y)
{
  RendererManager::GetInstance()->GetCurrent()->mousemove(x, y);
}

void DVRWindow::mousewheel(float dist, int x, int y)
{
  RendererManager::GetInstance()->GetCurrent()->mousewheel(dist, x, y);
}

void DVRWindow::InitRenderer()
{
  GLSLRenderer* rend = new GLSLRenderer(GetWidth(), GetHeight());
  rend->init();
  rend->Updating(true);

  RendererManager::GetInstance()->Add("glslrenderer", rend);
}

void DVRWindow::InitShaders()
{
  float screen_sz[2] = {static_cast<float>(GetWidth()), static_cast<float>(GetHeight())};
  glm::mat4 modelMatrix = TinyGL::GetInstance()->getMesh("proxy_cube")->m_modelMatrix;

  Shader* fpass = new Shader(std::string(RESOURCE_PATH) + std::string("/shaders/FPass.vs"),
                             std::string(RESOURCE_PATH) + std::string("/shaders/FPass.fs"));
  fpass->bind();
  fpass->bindFragDataLoc("fColor", 0);
  fpass->setUniformMatrix("u_mView", m_viewMatrix);
  fpass->setUniformMatrix("u_mProj", m_projMatrix);
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
  spass->setUniformMatrix("u_mView", m_viewMatrix);
  spass->setUniformMatrix("u_mProj", m_projMatrix);
  spass->setUniformMatrix("u_mModel", modelMatrix);
  spass->setUniformfv("u_vScreenSize", screen_sz, 2);
  spass->setUniform1f("u_fNumSamples", static_cast<float>(m_numSamples));
  spass->setUniform1i("u_sBackFaces", 0);
  spass->setUniform1i("u_sDensityMap", 1);
  spass->setUniform1i("u_sAlphaMap", 2);
  spass->setUniform1i("u_sColorTFunction", 3);

  Shader::Unbind();

  TinyGL::GetInstance()->addResource(SHADER, FPASS_KEY, fpass);
  TinyGL::GetInstance()->addResource(SHADER, SPASS_KEY, spass);
}

void DVRWindow::InitFBO()
{
  FramebufferObject* fbo = new FramebufferObject;
  fbo->bind(GL_FRAMEBUFFER);

  GLuint ids[2];

  glGenTextures(1, &ids[0]);

  glBindTexture(GL_TEXTURE_2D, ids[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  fbo->attachTexBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ids[0], 0);

  glGenRenderbuffers(1, &ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, ids[1]);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, GetWidth(), GetHeight());
  fbo->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  fbo->checkStatus();

  GLenum draw_buff[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buff);

  FramebufferObject::unbind();
  TinyGL::GetInstance()->addResource(FRAMEBUFFER, FBO_KEY, fbo);
}

void DVRWindow::InitMesh()
{
  Cube* cube = new Cube();
  cube->m_modelMatrix = glm::mat4(1);
  cube->setDrawCb(cb_drawcube_idx);

  TinyGL::GetInstance()->addResource(MESH, "proxy_cube", cube);
  delete cube;
}

void DVRWindow::InitResources()
{
  using namespace knl;
  m_viewMatrix = glm::lookAt(glm::vec3(0, 0, 3.5),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0));

  m_projMatrix = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 1.f, 5.f);

#ifdef WIN32
  DatasetManager::GetInstance()->Init("C:/Users/schardong/Pictures/datasets/");
#else
  DatasetManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/");
#endif
  DatasetManager::GetInstance()->SetActive("nucleon", GL_TEXTURE1);

#ifdef WIN32
  TFManager::GetInstance()->Init("C:/Users/schardong/Pictures/datasets/transfer-functions/");
#else
  TFManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/transfer-functions/");
#endif
  TFManager::GetInstance()->SetActive("tff1", GL_TEXTURE3);

  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(m_flowRate,
    DatasetManager::GetInstance()->GetCurrentKey());

  AlphaManager::GetInstance()->SetActive(DatasetManager::GetInstance()->GetCurrentKey(),
    GL_TEXTURE2);
}
