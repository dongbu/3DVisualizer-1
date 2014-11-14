#ifndef DVRWINDOW_H
#define DVRWINDOW_H

#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class DVRWindow
{
public:
  DVRWindow(int w = 640, int h = 480);
  virtual ~DVRWindow();

//  virtual int init();
//  virtual void update(){}
//  virtual void resize(int w, int h){}
//  virtual void draw();
//  virtual void keypress(int key, int press);
//  virtual void mousebutton(int button, int press, int x, int y);
//  virtual void mousemove(int x, int y);
//  virtual void mousewheel(float dist, int x, int y);

private:
  size_t m_frame;
  size_t m_numSamples;
  float m_flowRate;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  void InitRenderer();
  void InitShaders();
  void InitFBO();
  void InitMesh();
  void InitResources();
};

#endif // DVRWINDOW_H
