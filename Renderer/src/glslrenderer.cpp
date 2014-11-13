#include "glslrenderer.h"
#include "singleton.h"
#include "tinygl.h"
#include "topanalyzer.h"
#include "alphamanager.h"
#include "datasetmanager.h"
#include "tfmanager.h"
#include "config.h"

GLSLRenderer::GLSLRenderer()
{
  Initialized(false);
  Updating(false);
  m_mouse = new Arcball(700, 700, 0.05f, true, true);
}

GLSLRenderer::~GLSLRenderer()
{
  Updating(false);
  Initialized(false);

  //TODO: Delete resources here
  delete m_mouse;
}

int GLSLRenderer::init()
{
  m_eye = glm::vec3(0, 0, 3.5);
  m_viewMatrix = glm::lookAt(m_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  m_projMatrix = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 1.f, 5.f);
  Initialized(true);
}

int GLSLRenderer::update()
{
  if(!IsUpdating() or !IsInitialized())
    return IUP_DEFAULT;

  draw(0.f, 0.f);
  return IUP_DEFAULT;
}

int GLSLRenderer::keypress(int c, int press)
{
  if(press == 0) {
    switch(c) {
    case K_ESC:
      return IUP_CLOSE;
      break;
    case K_SP:
      TopAnalyzer::GetInstance()->AnalyzeCurrDataset(300.f, DatasetManager::GetInstance()->GetCurrentKey());
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
  return IUP_DEFAULT;
}

int GLSLRenderer::mousebutton(int button, int pressed, int, int, char*)
{
  m_mouse->mouseButtonCallback(button, pressed);
  return IUP_DEFAULT;
}

int GLSLRenderer::mousemotion(int xpos, int ypos, char*)
{
  m_mouse->cursorCallback(static_cast<double>(xpos), static_cast<double>(ypos));
  return IUP_DEFAULT;
}

int GLSLRenderer::mousewheel(float, int, int, char*)
{
  return IUP_DEFAULT;
}

int GLSLRenderer::draw(float, float)
{
  TinyGL* gl_ptr = TinyGL::GetInstance();
  Mesh* m = gl_ptr->getMesh("proxy_cube");
  Shader* fpass = gl_ptr->getShader(FPASS_KEY);
  Shader* spass = gl_ptr->getShader(SPASS_KEY);
  FramebufferObject* fbo = gl_ptr->getFBO(FBO_KEY);

  if(!fpass || !spass || !fbo || !m) return IUP_DEFAULT;

  fpass->bind();
  glm::mat4 rot = m_viewMatrix * m_mouse->createViewRotationMatrix();
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

  DatasetManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE1);
  AlphaManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE2);
  TFManager::GetInstance()->GetCurrent()->bind(GL_TEXTURE3);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo->getAttachmentId(GL_COLOR_ATTACHMENT0));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_BACK);
  m->draw();

  Shader::Unbind();

  return IUP_DEFAULT;
}
