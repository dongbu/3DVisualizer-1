#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include <map>
#include <GL/glew.h>

/**
 * @brief The FramebufferObject class  Abstracts the creation and management of
 * framebuffer objects.
 *
 * This class is intended to facilitate the creating and management of
 * OpenGL framebuffer objects. The user may attach a series of renderbuffers or
 * textures to act as the color/depth/stencil buffers. All of the attached
 * buffers must be created and bound before the call to attach*Buffer.
 */
class FramebufferObject
{
public:
  FramebufferObject();
  ~FramebufferObject();

  /**
   * @brief attachTexBuffer Calls the glFramebufferTexture function to attach
   * a texture to the currently bound framebuffer.
   * @param target The framebuffer target: GL_DRAW_FRAMEBUFFER,
   * GL_READ_FRAMEBUFFER, or GL_FRAMEBUFFER
   * @param attach The framebuffer attachment point: GL_COLOR_ATTACHMENTi,
   * GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT or GL_DEPTH_STENCIL_ATTACHMENT
   * @param texid The texture name given by the OpenGL context
   * @param level The texture mipmap level
   */
  void attachTexBuffer(GLenum target, GLenum attach, GLuint texid, GLint level);

  /**
   * @brief attachRenderBuffer calls the glFramebufferRenderbuffer to attach
   * a renderbuffer to the currently bound framebuffer.
   * @param target The framebuffer target: GL_DRAW_FRAMEBUFFER,
   * GL_READ_FRAMEBUFFER, or GL_FRAMEBUFFER
   * @param attach Specifies the framebuffer attachment point
   * @param rbtarg The renderbuffer target. Always GL_RENDERBUFFER
   * @param rbid The renderbuffer name given by the OpenGL context
   */
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

  /**
   * @brief checkStatus Checks if there were any errors with the framebuffer
   * creation and prints them on the log.
   */
  void checkStatus();

private:
  GLuint m_id;

  std::map<GLenum, GLuint> m_attMap;
};

#endif // FRAMEBUFFEROBJECT_H
