#include "viswindow.h"
#include "config.h"
#include "viswidget.h"
#include "tinygl.h"
#include "alphamanager.h"
#include "topanalyzer.h"

#include <GL/glew.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMenuBar>
#include <QTimer>
#include <QMainWindow>
#include <QApplication>
#include <QToolBar>
#include <QStatusBar>

VisWindow::VisWindow(QWidget* parent, int w, int h) :
  QMainWindow(parent)
{
  QGLFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(3, 3);
  format.setSamples(4);
  format.setProfile(QGLFormat::CoreProfile);

  resize(w, h + 40);
  create();
  createInterface();

  m_renderWidget = new VisWidget(format, this);
  setCentralWidget(m_renderWidget);
}

void VisWindow::keyPressEvent(QKeyEvent* e)
{
  switch(e->key()) {
  case Qt::Key_Escape:
    QApplication::instance()->quit();
    break;
  case Qt::Key_Space:
    TopAnalyzer::instance()->AnalyzeCurrDataset(m_renderWidget->getFlowRate(), DatasetManager::instance()->getCurrentKey());
    AlphaManager::instance()->SetActive(DatasetManager::instance()->getCurrentKey(), GL_TEXTURE2);
    break;
  case Qt::Key_1:
    TFManager::instance()->setActive("tff1", GL_TEXTURE3);
    break;
  case Qt::Key_2:
    TFManager::instance()->setActive("tff2", GL_TEXTURE3);
    break;
  case Qt::Key_F1:
    DatasetManager::instance()->setActive("neghip", GL_TEXTURE1);
    break;
  case Qt::Key_F2:
    DatasetManager::instance()->setActive("bonsai", GL_TEXTURE1);
    break;
  case Qt::Key_F3:
    DatasetManager::instance()->setActive("nucleon", GL_TEXTURE1);
    break;
  case Qt::Key_F4:
    DatasetManager::instance()->setActive("silicium", GL_TEXTURE1);
    break;
  case Qt::Key_F5:
    DatasetManager::instance()->setActive("fuel", GL_TEXTURE1);
    break;
  case Qt::Key_F6:
    DatasetManager::instance()->setActive("BostonTeapot", GL_TEXTURE1);
    break;
  case Qt::Key_F7:
    DatasetManager::instance()->setActive("lobster", GL_TEXTURE1);
    break;
  case Qt::Key_F8:
    DatasetManager::instance()->setActive("foot", GL_TEXTURE1);
    break;
  case Qt::Key_F9:
    DatasetManager::instance()->setActive("CT-Knee", GL_TEXTURE1);
    break;
  }
}

void VisWindow::keyReleaseEvent(QKeyEvent*)
{}

void VisWindow::moveEvent(QMoveEvent*)
{}

void VisWindow::resizeEvent(QResizeEvent*)
{}

void VisWindow::closeEvent(QCloseEvent* )
{}

void VisWindow::quit()
{

}

void VisWindow::loadDataset()
{}

void VisWindow::loadAlphaDataset()
{}

void VisWindow::loadColorTFunction()
{}

void VisWindow::saveAlphaDataset()
{}

void VisWindow::analyzeDataset()
{}

void VisWindow::setNumSamples()
{}

void VisWindow::setRootDataDir()
{}

void VisWindow::createInterface()
{
  m_file = new QMenu("File", this);

  m_quit = new QAction("Exit", this);
  connect(m_quit, SIGNAL(triggered()), this, SLOT(quit()));

  m_file->addAction(m_quit);

  menuBar()->addMenu(m_file);

  //TOOLBAR
  QToolBar* toolbar = new QToolBar;
  toolbar->setMovable(false);

  m_analyze = new QAction("Analyze dataset", this);
  connect(m_analyze, SIGNAL(triggered()), this, SLOT(analyzeDataset()));

  m_setNumSamples = new QAction("Change number of samples", this);
  connect(m_setNumSamples, SIGNAL(triggered()), this, SLOT(setNumSamples()));

  m_setRootDir = new QAction("Set root directory", this);
  connect(m_setRootDir, SIGNAL(triggered()), this, SLOT(setRootDataDir()));

  toolbar->addAction(m_analyze);
  toolbar->addAction(m_setNumSamples);
  toolbar->addAction(m_setRootDir);

  addToolBar(toolbar);

  //STATUSBAR
  QStatusBar* statusbar = new QStatusBar;

  setStatusBar(statusbar);
}

