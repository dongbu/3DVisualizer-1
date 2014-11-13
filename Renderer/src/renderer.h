#ifndef RENDERER_H
#define RENDERER_H

enum RendererType
{
  GLSL,
  OPENCL,
  CUDA
};

class Renderer
{
public:
  Renderer(int w = 640, int h = 480)
  {
    SetWidth(w);
    SetHeight(h);
  }

  virtual ~Renderer()
  {
    Initialized(false);
    Updating(false);
  }

  /**
   * Pure virtual methods. Theses methods may be implemented to handle actions
   * that are local to each renderer.
   */
  virtual int init() = 0;
  virtual void update() = 0;
  virtual void keypress(int key, int press) = 0;
  virtual void mousebutton(int button, int press, int x, int y) = 0;
  virtual void mousemove(int x, int y) = 0;
  virtual void mousewheel(float dist, int x, int y) = 0;
  virtual void draw() = 0;

  bool IsInitialized()
  {
    return m_initialized;
  }

  bool IsUpdating()
  {
    return m_updating;
  }

  int GetWidth()
  {
    return m_width;
  }

  int GetHeight()
  {
    return m_height;
  }

  void SetWidth(int w)
  {
    m_width = w;
  }

  void SetHeight(int h)
  {
    m_height = h;
  }

  void Updating(bool u)
  {
    m_updating = u;
  }

protected:
  int m_width;
  int m_height;
  RendererType m_type;

  void Initialized(bool i)
  {
    m_initialized = i;
  }

private:
  bool m_initialized;
  bool m_updating;
};

#endif // RENDERER_H
