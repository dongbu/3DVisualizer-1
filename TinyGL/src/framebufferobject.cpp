#include "framebufferobject.h"
#include "logger.h"

FramebufferObject::FramebufferObject()
{
  glGenFramebuffers(1, &m_id);
}

FramebufferObject::~FramebufferObject()
{
  unbind();
  glDeleteFramebuffers(1, &m_id);
  m_attMap.clear();
}

void FramebufferObject::attachTexBuffer(GLenum target, GLenum attach, GLuint texid, GLint level)
{
  glFramebufferTexture(target, attach, texid, level);
  m_attMap[attach] = texid;
}

void FramebufferObject::attachRenderBuffer(GLenum target, GLenum attach, GLenum rbtarg, GLuint rbid)
{
  glFramebufferRenderbuffer(target, attach, rbtarg, rbid);
  m_attMap[attach] = rbid;
}

void FramebufferObject::checkStatus()
{
  GLint curr_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curr_fbo);

  bind(GL_FRAMEBUFFER);
  GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (fboStatus) {
  case GL_FRAMEBUFFER_UNDEFINED:
    Logger::GetInstance()->error("FBO undefined");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    Logger::GetInstance()->error("FBO incomplete attachment");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    Logger::GetInstance()->error("FBO missing attachment");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    Logger::GetInstance()->error("FBO incomplete draw buffer");
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED:
    Logger::GetInstance()->error("FBO unsupported");
    break;
  case GL_FRAMEBUFFER_COMPLETE:
    Logger::GetInstance()->log("FBO created successfully");
    break;
  default:
    Logger::GetInstance()->error("FBO undefined problem");
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, curr_fbo);
}
