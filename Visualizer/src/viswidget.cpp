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
  m_numSamples(512)
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

bool VisWidget::createPyroVol(std::string key)
{
  if(!isMetafileLoaded()) {
    return false;
  }

  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(false);
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingColorMap(false);

  knl::Dataset* pyro = knl::Dataset::createPyroclasticVolume(128);

  if(DatasetManager::getInstance()->add(key, pyro))
    dataLoaded(DatasetManager::getInstance()->setActive(key));

  return isDatasetLoaded();
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
  knl::Dataset* alpha = ainstance->get(key);
  if(alpha == NULL)
    return false;

  std::string path = ainstance->getPath() + key + ".raw";
  alpha->save(path);
  ainstance->saveCurrent();

  return false;
}

bool VisWidget::buildContourTree()
{
  return TopAnalyzer::getInstance()->buildContourTree();
}

bool VisWidget::simplifyContourTree(double threshold)
{
  return TopAnalyzer::getInstance()->simplifyContourTree(threshold);
}

bool VisWidget::flowOpacity(double flow_rate)
{
  return TopAnalyzer::getInstance()->flowOpacity(flow_rate);
}

bool VisWidget::buildAlphaMap(std::string key)
{
  if(key.empty())
    return false;

  bool ret = TopAnalyzer::getInstance()->createAlphaMap(key);
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(ret);
  alphaMapLoaded(ret);
  return ret;
}

bool VisWidget::loadDataset(std::string key)
{
  if(!isMetafileLoaded()) {
    return false;
  }

  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingAlphaMap(false);
  ((GLSLRenderer*) RendererManager::getInstance()->getCurrent())->setUsingColorMap(false);
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

//  GLint max_size;
//  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_size);
//  printf("%d\n", max_size);
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

void VisWidget::mousePressEvent(QMouseEvent* e)
{
  if(e->button() == Qt::LeftButton) {
    setNumSamples(128);
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
  setNumSamples(512);
}

void VisWidget::mouseMoveEvent(QMouseEvent* e)
{
  RendererManager::getInstance()->getCurrent()->mousemove(e->x(), e->y());
}
