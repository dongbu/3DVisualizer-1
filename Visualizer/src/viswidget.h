#ifndef VISWIDGET_H
#define VISWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class VisWidget : public QGLWidget
{
  Q_OBJECT
public:
  explicit VisWidget(const QGLFormat& format, QWidget *parent);
  virtual ~VisWidget();

  bool IsInitialized()
  {
    return m_initCalled;
  }

  size_t GetNumSamples()
  {
    return m_numSamples;
  }

  void SetNumSamples(size_t num)
  {
    m_numSamples = num;
  }

  float GetFlowRate()
  {
    return m_flowRate;
  }

protected slots:
  virtual void update();

protected:
  virtual void initializeGL();
  virtual void paintGL();

  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void wheelEvent(QWheelEvent *);

  virtual void keyPressEvent(QKeyEvent *);
  virtual void keyReleaseEvent(QKeyEvent *);

  virtual void moveEvent(QMoveEvent *); // reduce sampling.
  virtual void resizeEvent(QResizeEvent *); //stop animation.
  virtual void closeEvent(QCloseEvent *); //free everything.

private:
  QTimer* m_timer;
  size_t m_numSamples;
  float m_flowRate;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_projMatrix;

  bool m_initCalled;

  void Init();
  void InitGLEW();
  void InitRenderer();
  void InitShaders();
  void InitFBO();
  void InitMesh();
  void InitResources();

  void Initialized(bool i)
  {
    m_initCalled = i;
  }
};

#endif // VISWIDGET_H
