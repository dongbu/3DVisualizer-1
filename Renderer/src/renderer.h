#ifndef RENDERER_H
#define RENDERER_H

/**
 * @brief The RendererType enum
 * This enumeration lists the possible renderer types. For now, only the GLSL
 * renderer is implemented. This renderer serves as a proof of concept.
 */
enum RendererType
{
  GLSL,
  OPENCL,
  CUDA
};

/**
 * @brief The Renderer class defines the basic interface that all renderer
 * subclasses must follow. Each subclass is responsible for initializing the
 * resources needed for their objectives, this includes any framebuffers,
 * shaders, meshes, OpenCL/CUDA kernels (future), etc. Most of these resources
 * must be added to some manager (TinyGL instance) and may be retrived by the
 * renderer subclass when needed.
 */
class Renderer
{
public:
  Renderer(int w = 640, int h = 480)
  {
    setWidth(w);
    setHeight(h);
  }

  virtual ~Renderer()
  {
    initialized(false);
    updating(false);
  }

  /**
   * Pure virtual methods. Theses methods must be implemented to handle actions
   * that are local to each renderer.
   */
  virtual int init() = 0;
  virtual void update() = 0;
  virtual void resize(int w, int h) = 0;
  virtual void keypress(int key, int press) = 0;
  virtual void mousebutton(int button, int press, int x, int y) = 0;
  virtual void mousemove(int x, int y) = 0;
  virtual void mousewheel(float dist, int x, int y) = 0;
  virtual void draw() = 0;

  bool isInitialized()
  {
    return m_initialized;
  }

  bool isUpdating()
  {
    return m_updating;
  }

  int width()
  {
    return m_width;
  }

  int height()
  {
    return m_height;
  }

  void setWidth(int w)
  {
    m_width = w;
  }

  void setHeight(int h)
  {
    m_height = h;
  }

  void updating(bool u)
  {
    m_updating = u;
  }

protected:
  int m_width;
  int m_height;
  RendererType m_type;

  void initialized(bool i)
  {
    m_initialized = i;
  }

private:
  bool m_initialized;
  bool m_updating;
};

#endif // RENDERER_H
