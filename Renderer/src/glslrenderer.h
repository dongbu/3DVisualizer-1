#ifndef GLSLRENDERER_H
#define GLSLRENDERER_H

#include <GL/glew.h>

#include "renderer.h"
#include "arcball.h"

/**
 * @brief The GLSLRenderer class
 * This class inherits from the Renderer class. This implements a renderer that
 * uses GLSL shaders to apply the Direct Volume Rendering using Volume
 * Raycasting techinique.
 *
 * This class' instance is responsible for initializing the Framebuffer objects,
 * shaders and meshes needed to render on the screen. All of these objects are
 * stored in the TinyGL instance and retrieved when needed, on the draw method.
 */
class GLSLRenderer : public Renderer
{
public:
  GLSLRenderer(size_t w = 640, size_t h = 480, size_t numSamples = 256);
  ~GLSLRenderer();

  virtual int init();
  virtual void update();
  virtual void resize(int w, int h);
  virtual void keypress(int key, int press);
  virtual void mousebutton(int button, int press, int x, int y);
  virtual void mousemove(int x, int y);
  virtual void mousewheel(float dist, int x, int y);
  virtual void draw();

  size_t getNumSamples()
  {
    return m_numSamples;
  }

  void setNumSamples(size_t n)
  {
    m_numSamples = n;
  }

  bool isUsingAlphaMap()
  {
    return m_usingAlphaMap;
  }

  void setUsingAlphaMap(bool b)
  {
    m_usingAlphaMap = b;
  }

  bool isUsingColorMap()
  {
    return m_usingColorMap;
  }

  void setUsingColorMap(bool b)
  {
    m_usingColorMap = b;
  }

  Arcball* m_arcball;
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;
  glm::vec3 m_eye;

private:

  size_t m_numSamples;
  bool m_usingAlphaMap;
  bool m_usingColorMap;

  void initFBO();
  void initMesh();
  void initShaders();
  void initGLEW();
};

#endif // GLSLRENDERER_H
