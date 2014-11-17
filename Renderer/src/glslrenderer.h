#ifndef GLSLRENDERER_H
#define GLSLRENDERER_H

#include <GL/glew.h>

#include "renderer.h"
#include "arcball.h"

class GLSLRenderer : public Renderer
{
public:
  GLSLRenderer(size_t w = 640, size_t h = 480);
  ~GLSLRenderer();

  virtual int init();
  virtual void update();
  virtual void resize(int w, int h);
  virtual void keypress(int key, int press);
  virtual void mousebutton(int button, int press, int x, int y);
  virtual void mousemove(int x, int y);
  virtual void mousewheel(float dist, int x, int y);
  virtual void draw();

  Arcball* m_arcball;
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;
  glm::vec3 m_eye;
};

#endif // GLSLRENDERER_H
