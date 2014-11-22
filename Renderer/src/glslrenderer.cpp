#include "glslrenderer.h"
#include "singleton.h"
#include "tinygl.h"
#include "topanalyzer.h"
#include "alphamanager.h"
#include "datasetmanager.h"
#include "tfmanager.h"
#include "cube.h"
#include "shader.h"
#include "mesh.h"
#include "config.h"

static void cb_drawcube_idx(size_t)
{
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}

GLSLRenderer::GLSLRenderer(size_t w, size_t h, size_t numSamples) :
  Renderer(w, h),
  m_numSamples(numSamples)
{
  m_type = RendererType::GLSL;
  initialized(false);
  updating(false);

#ifdef WIN32
  m_arcball = new Arcball(width(), height(), 0.85);
#else
  m_arcball = new Arcball(width(), height(), 0.05);
#endif
}

GLSLRenderer::~GLSLRenderer()
{
  updating(false);
  initialized(false);

  delete m_arcball;
  m_arcball = nullptr;
}

int GLSLRenderer::init()
{
  m_eye = glm::vec3(0, 0, 3.5);
  m_viewMatrix = glm::lookAt(m_eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  m_projMatrix = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 1.f, 5.f);

  initGLEW();
  initMesh();
  initShaders();
  initFBO();

  initialized(true);
  return 0;
}

void GLSLRenderer::update()
{
  if(!isUpdating() || !isInitialized())
    return;
}

void GLSLRenderer::resize(int w, int h)
{}

void GLSLRenderer::keypress(int key, int press)
{}

void GLSLRenderer::mousebutton(int button, int pressed, int, int)
{
  m_arcball->mouseButtonCallback(button, pressed);
}

void GLSLRenderer::mousemove(int x, int y)
{
  m_arcball->cursorCallback(static_cast<double>(x), static_cast<double>(y));
}

void GLSLRenderer::mousewheel(float, int, int)
{}

void GLSLRenderer::draw()
{
  if(!isUpdating() || !isInitialized())
    return;

  TinyGL* gl_ptr = TinyGL::instance();
  Mesh* m = gl_ptr->getMesh("proxy_cube");
  Shader* fpass = gl_ptr->getShader(FPASS_KEY);
  Shader* spass = gl_ptr->getShader(SPASS_KEY);
  FramebufferObject* fbo = gl_ptr->getFBO(FBO_KEY);

  if(!fpass || !spass || !fbo || !m) return;

  fpass->bind();
  glm::mat4 rot = m_viewMatrix * m_arcball->createViewRotationMatrix();
  fpass->setUniformMatrix("u_mView", rot);

  //First pass
  fbo->bind(GL_FRAMEBUFFER);
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_FRONT);
  m->draw();

  FramebufferObject::unbind();
  glViewport(0, 0, width(), height());

  //Second pass
  spass->bind();
  spass->setUniformMatrix("u_mView", rot);

  DatasetManager::instance()->getCurrent()->bind(GL_TEXTURE1);
  AlphaManager::instance()->GetCurrent()->bind(GL_TEXTURE2);
  TFManager::instance()->getCurrent()->bind(GL_TEXTURE3);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo->getAttachmentId(GL_COLOR_ATTACHMENT0));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_BACK);
  m->draw();
  Shader::Unbind();
}

void GLSLRenderer::initFBO()
{
  FramebufferObject* fbo = new FramebufferObject;
  fbo->bind(GL_FRAMEBUFFER);

  GLuint ids[2];

  glGenTextures(1, &ids[0]);

  glBindTexture(GL_TEXTURE_2D, ids[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  fbo->attachTexBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ids[0], 0);

  glGenRenderbuffers(1, &ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, ids[1]);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width(), height());
  fbo->attachRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ids[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  fbo->checkStatus();

  GLenum draw_buff[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buff);

  FramebufferObject::unbind();
  TinyGL::instance()->addResource(FRAMEBUFFER, FBO_KEY, fbo);
}

void GLSLRenderer::initMesh()
{
  Cube* cube = new Cube();
  cube->m_modelMatrix = glm::mat4(1);
  cube->setDrawCb(cb_drawcube_idx);

  TinyGL::instance()->addResource(MESH, "proxy_cube", cube);
}

void GLSLRenderer::initShaders()
{
  float screen_sz[2] = {static_cast<float>(width()), static_cast<float>(height())};
  glm::mat4 modelMatrix = TinyGL::instance()->getMesh("proxy_cube")->m_modelMatrix;

  Shader* fpass = new Shader(std::string("shaders/FPass.vs"),
                             std::string("shaders/FPass.fs"));
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

  Shader* spass = new Shader(std::string("shaders/SPass.vs"),
                             std::string("shaders/SPassMultiOp.fs"));

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

  TinyGL::instance()->addResource(SHADER, FPASS_KEY, fpass);
  TinyGL::instance()->addResource(SHADER, SPASS_KEY, spass);
}

void GLSLRenderer::initGLEW()
{
  glewExperimental = true;
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
    return;
  }
}
