#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include <map>
#include <GL/glew.h>

class FramebufferObject
{
public:
  FramebufferObject();
  ~FramebufferObject();

  void attachTexBuffer(GLenum target, GLenum attach, GLuint texid, GLint level);
  void attachRenderBuffer(GLenum target, GLenum attach, GLenum rbtarg, GLuint rbid); 

  void bind(GLenum target)
  {
    glBindFramebuffer(target, m_id);
  }

  static void unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  GLuint getAttachmentId(GLenum attachment_point)
  {
    if(m_attMap.find(attachment_point) != m_attMap.end())
      return m_attMap[attachment_point];
    return 0;
  }

  void checkStatus();

private:
  GLuint m_id;

  std::map<GLenum, GLuint> m_attMap;
};

#endif // FRAMEBUFFEROBJECT_H
