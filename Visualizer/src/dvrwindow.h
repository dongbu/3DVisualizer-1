#ifndef DVRWINDOW_H
#define DVRWINDOW_H

#include <cstdlib>

class Renderer;

class DVRWindow
{
public:

  DVRWindow();
  ~DVRWindow();

  void Init();
  void Render();

private:
  size_t m_frame;
  Renderer* m_renderer;

  void InitShaders();
  void InitFBO();
  void InitMesh();
  void InitResources();

  void cb_drawcube_idx(size_t);
};

#endif // DVRWINDOW_H
