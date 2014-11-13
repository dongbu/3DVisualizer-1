#ifndef GLSLRENDERER_H
#define GLSLRENDERER_H

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

#include "renderer.h"
#include "arcball.h"

class GLSLRenderer : public Renderer
{
public:
  GLSLRenderer();
  ~GLSLRenderer();

  virtual int init();
  virtual int update();
  virtual int keypress(int, int);
  virtual int mousebutton(int, int, int, int, char*);
  virtual int mousemotion(int, int, char*);
  virtual int mousewheel(float, int, int, char*);
  virtual int draw(float, float);

  Arcball* m_mouse;
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;
  glm::vec3 m_eye;
};

#endif // GLSLRENDERER_H
