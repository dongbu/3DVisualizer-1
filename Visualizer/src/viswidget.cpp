#include "viswidget.h"
#include "singleton.h"
#include "tinygl.h"
#include "datasetmanager.h"
#include "tfmanager.h"
#include "topanalyzer.h"
#include "alphamanager.h"
#include "renderermanager.h"
#include "glslrenderer.h"
#include "visconfig.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>


VisWidget::VisWidget(const QGLFormat& format, QWidget *parent) :
  QGLWidget(format, parent),
  m_numSamples(512),
  m_flowRate(450.f)
{
  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  initialized(false);
  dataLoaded(false);
  metafileLoaded(false);
  colorMapLoaded(false);
  alphaMapLoaded(false);
}

VisWidget::~VisWidget()
{
  m_timer->stop();
  delete m_timer;
  initialized(false);
  metafileLoaded(false);
  dataLoaded(false);
  colorMapLoaded(false);
  alphaMapLoaded(false);

  DatasetManager::getInstance()->freeResources();
  TFManager::getInstance()->freeResources();
  AlphaManager::getInstance()->freeResources();
  RendererManager::getInstance()->freeResources();
}

void VisWidget::startTimer(int msec)
{
  m_timer->start(msec);
}

void VisWidget::stopTimer()
{
  m_timer->stop();
}

bool VisWidget::loadMetafile(std::string path)
{
  std::string new_path = path + std::string("/");

  bool ret = DatasetManager::getInstance()->init(new_path);
  ret &= TFManager::getInstance()->init(new_path + "transfer-functions/");
  ret &= AlphaManager::getInstance()->init(new_path + "alpha-maps/");

  metafileLoaded(ret);

  return isMetafileLoaded();
}

bool VisWidget::loadColorTF(std::string key)
{
  if(!isMetafileLoaded()) {
    return false;
  }

  colorMapLoaded(TFManager::getInstance()->setActive(key, GL_TEXTURE3));
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingColorMap(isColorLoaded());
  return isColorLoaded();
}

bool VisWidget::loadAlphaTF(std::string key)
{
  if(!isMetafileLoaded()) {
    return false;
  }

  alphaMapLoaded(AlphaManager::getInstance()->setActive(key, GL_TEXTURE2));
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(isAlphaLoaded());
  return isAlphaLoaded();
}

bool VisWidget::saveAlphaTF(std::string key, std::string data_key)
{
  if(!isMetafileLoaded() || key.empty() || data_key.empty()) {
    return false;
  }

  AlphaManager* ainstance = AlphaManager::getInstance();
  knl::Dataset* alpha = AlphaManager::getInstance()->get(key);
  if(alpha == NULL) return false;

  std::string path = ainstance->getPath() + key + ".raw";
  alpha->save(path);
  ainstance->saveCurrent();

  return false;
}

bool VisWidget::analyze()
{
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(true);
  return true;
}

bool VisWidget::buildContourTree(std::string key)
{
  return false;
}

bool VisWidget::simplifyContourTree(std::string key)
{
  return false;
}

bool VisWidget::flowOpacity(std::string key)
{
  return false;
}

bool VisWidget::buildAlphaMap(std::string key)
{
  return false;
}

bool VisWidget::loadDataset(std::string key)
{
  if(!isMetafileLoaded()) {
    return false;
  }

  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(false);
  dataLoaded(DatasetManager::getInstance()->setActive(key));
  return isDatasetLoaded();
}

void VisWidget::update()
{
  if(isInitialized() && isDatasetLoaded()) {
    paintGL();
    updateGL();
  }
}

void VisWidget::initializeGL()
{
  makeCurrent();

  glClearColor(0.3f, 0.3f, 0.3f, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1.0f);

  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLSLRenderer* rend = new GLSLRenderer(width(), height());
  rend->init();
  rend->updating(true);

  RendererManager::getInstance()->add("glslrenderer", rend);
  RendererManager::getInstance()->setActive("glslrenderer");
  initialized(true);
}

void VisWidget::paintGL()
{
  if(isInitialized() && isDatasetLoaded()) {
    RendererManager::getInstance()->getCurrent()->update();
    RendererManager::getInstance()->getCurrent()->draw();
  }
}

void VisWidget::resizeEvent(QResizeEvent*)
{
  if(isInitialized())
    RendererManager::getInstance()->getCurrent()->resize(width(), height());
}

void VisWidget::closeEvent(QCloseEvent*)
{}

void VisWidget::moveEvent(QMoveEvent*)
{}

void VisWidget::keyPressEvent(QKeyEvent*)
{}

void VisWidget::keyReleaseEvent(QKeyEvent*)
{}

void VisWidget::mousePressEvent(QMouseEvent* e)
{
  if(e->button() == Qt::LeftButton) {
    GLSLRenderer* rend = (GLSLRenderer*) RendererManager::getInstance()->getCurrent();
    rend->setNumSamples(128);
    rend->mousebutton(e->button(), 1, e->x(), e->y());
  }
}

void VisWidget::mouseReleaseEvent(QMouseEvent* e)
{
  GLSLRenderer* rend = (GLSLRenderer*) RendererManager::getInstance()->getCurrent();
  rend->setNumSamples(256);
  rend->mousebutton(e->button(), 0, e->x(), e->y());
}

void VisWidget::mouseMoveEvent(QMouseEvent* e)
{
  RendererManager::getInstance()->getCurrent()->mousemove(e->x(), e->y());
}

void VisWidget::wheelEvent(QWheelEvent*)
{}
