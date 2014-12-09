#ifndef VISWIDGET_H
#define VISWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 * @brief The VisWidget class
 * This class implements the central widget used for rendering with OpenGL.
 *
 * This widget has a series of slots and signals to receive commands from the
 * parent window and pass them to the corresponding classes
 */
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

public slots:
  void startTimer(int msec);
  void stopTimer();

  bool createPyroVol(std::string key);
  bool loadMetafile(std::string path);
  bool loadDataset(std::string key);
  bool loadColorTF(std::string key);
  bool loadAlphaTF(std::string key);

  bool saveAlphaTF(std::string key, std::string data_key);

  bool buildContourTree();
  bool simplifyContourTree(double threshold);
  bool flowOpacity(double flow_rate);
  bool buildAlphaMap(std::string key);

protected:
  virtual void initializeGL();
  virtual void paintGL();

  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void resizeEvent(QResizeEvent *); //stop animation.

protected slots:
  virtual void update();

private:
  QTimer* m_timer;
  size_t m_numSamples;

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
