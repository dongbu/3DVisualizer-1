#ifndef RENDERER_H
#define RENDERER_H

enum RendererType
{
  GLSL
};

class Renderer
{
public:
  virtual int init() = 0;
  virtual int update() = 0;
  virtual int keypress(int, int) = 0;
  virtual int mousebutton(int, int, int, int, char*) = 0;
  virtual int mousemotion(int, int, char*) = 0;
  virtual int mousewheel(float, int, int, char*) = 0;
  virtual int draw(float, float) = 0;

  bool IsInitialized()
  {
    return m_initialized;
  }

  bool IsUpdating()
  {
    return m_updating;
  }

  void Updating(bool u)
  {
    m_updating = u;
  }

protected:
  void Initialized(bool i)
  {
    m_initialized = i;
  }

private:
  bool m_initialized;
  bool m_updating;
};

#endif // RENDERER_H
