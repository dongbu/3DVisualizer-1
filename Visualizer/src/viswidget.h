#ifndef VISWIDGET_H
#define VISWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

enum DatasetType
{
  DATASET,
  COLORTF,
  ALPHATF,
};

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

  bool isMetafileLoaded()
  {
    return m_metafileLoaded;
  }

  bool isDatasetLoaded()
  {
    return m_datasetLoaded;
  }

  bool isAlphaLoaded()
  {
    return m_alphaMapLoaded;
  }

  bool isColorLoaded()
  {
    return m_colorMapLoaded;
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
  void startTimer(int msec);
  void stopTimer();

  bool loadMetafile(std::string path, DatasetType tp);
  bool loadDataset(std::string key);
  bool loadColorTF(std::string key);
  bool loadAlphaTF(std::string key);

  bool saveAlphaTF(std::string key, std::string data_key);

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

  bool m_initCalled;
  bool m_metafileLoaded;
  bool m_datasetLoaded;
  bool m_alphaMapLoaded;
  bool m_colorMapLoaded;

  void initialized(bool i)
  {
    m_initCalled = i;
  }

  void metafileLoaded(bool l)
  {
    m_metafileLoaded = l;
  }

  void dataLoaded(bool l)
  {
    m_datasetLoaded = l;
  }

  void alphaMapLoaded(bool l)
  {
    m_alphaMapLoaded = l;
  }

  void colorMapLoaded(bool l)
  {
    m_colorMapLoaded = l;
  }
};

#endif // VISWIDGET_H
