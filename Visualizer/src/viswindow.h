#ifndef VISWINDOW_H
#define VISWINDOW_H

#include <QWindow>
#include <QTime>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class QOpenGLContext;

class VisWindow : public QWindow
{
  Q_OBJECT

public:
  VisWindow(QScreen* screen = 0, int w = 640, int h = 480);

protected slots:
  void draw();

protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void keyPressEvent(QKeyEvent *);
  virtual void keyReleaseEvent(QKeyEvent *);
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void wheelEvent(QWheelEvent *);

private:
  QOpenGLContext* m_context;
  QTime m_timer;

  size_t m_numSamples;
  float m_flowRate;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  void Init();
  void InitGLEW();
  void InitRenderer();
  void InitShaders();
  void InitFBO();
  void InitMesh();
  void InitResources();
};

#endif // VISWINDOW_H
