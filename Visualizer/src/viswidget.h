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

  bool isInitialized()
  {
    return m_initCalled;
  }

  size_t getNumSamples()
  {
    return m_numSamples;
  }

  void setNumSamples(size_t num)
  {
    m_numSamples = num;
  }

  float getFlowRate()
  {
    return m_flowRate;
  }

  void setFlowRate(float flow)
  {
    m_flowRate = flow;
  }

public slots:

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

protected slots:
  virtual void update();

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

  void initialized(bool i)
  {
    m_initCalled = i;
  }
};

#endif // VISWIDGET_H
