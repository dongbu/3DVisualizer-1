#include "viswindow.h"
#include "config.h"
#include "viswidget.h"
#include "tinygl.h"
#include "alphamanager.h"
#include "topanalyzer.h"

#include <iostream>
#include <GL/glew.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMenuBar>
#include <QTimer>
#include <QMainWindow>
#include <QApplication>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>

VisWindow::VisWindow(QWidget* parent, int w, int h, std::string path) :
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

  while(path.empty()) {
    QString qpath = QFileDialog::getExistingDirectory(this,
                                                     "Load Metafile",
                                                     QDir::home().absolutePath());

    path = qpath.toStdString();
    m_renderWidget->loadMetafile(path, DATASET);
  }
}

void VisWindow::contextMenuEvent(QContextMenuEvent* e)
{
  QMenu tmp(this);
  tmp.addAction(m_analyzeAction);
  tmp.addAction(m_setNumSamplesAction);
  tmp.addAction(m_setFlowRateAction);
  //FIXME: Test if a valid alpha dataset was generated. If true, then add
  // the options below.
  tmp.addAction(m_saveAlphaAction);
  tmp.addAction(m_loadAlphaAction);
  tmp.exec(e->globalPos());
}

void VisWindow::keyPressEvent(QKeyEvent* e)
{
  switch(e->key()) {
  case Qt::Key_Escape:
    QApplication::instance()->quit();
    break;
  case Qt::Key_Space:
    TopAnalyzer::instance()->AnalyzeCurrDataset(m_renderWidget->getFlowRate(), DatasetManager::instance()->getCurrentKey());
    AlphaManager::instance()->setActive(DatasetManager::instance()->getCurrentKey(), GL_TEXTURE2);
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
{}

void VisWindow::loadDataset()
{
  using std::vector;
  using std::string;
  using std::cout;
  using std::endl;

  vector<string> vec = DatasetManager::instance()->getKeys();
  m_renderWidget->loadDataset(vec[0]);
  m_renderWidget->startTimer(16);
}

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

void VisWindow::setFlowRate()
{}

void VisWindow::createInterface()
{
  createMenus();
  createStatusBar();
}

void VisWindow::createMenus()
{
  m_fileMenu = new QMenu("File", this);
  m_editMenu = new QMenu("Edit", this);
  m_helpMenu = new QMenu("Help", this);

  m_loadDatasetAction = new QAction("Load dataset", this);
  connect(m_loadDatasetAction, SIGNAL(triggered()), this, SLOT(loadDataset()));

  m_loadAlphaAction = new QAction("Load alpha map", this);
  connect(m_loadAlphaAction, SIGNAL(triggered()), this, SLOT(loadAlphaDataset()));

  m_saveAlphaAction = new QAction("Save alpha map", this);
  connect(m_saveAlphaAction, SIGNAL(triggered()), this, SLOT(saveAlphaDataset()));

  m_quitAction = new QAction("Exit", this);
  connect(m_quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  m_analyzeAction = new QAction("Analyze dataset", this);
  connect(m_analyzeAction, SIGNAL(triggered()), this, SLOT(analyzeDataset()));

  m_setNumSamplesAction = new QAction("Edit sampling rate", this);
  connect(m_setNumSamplesAction, SIGNAL(triggered()), this, SLOT(setNumSamples()));

  m_setFlowRateAction = new QAction("Edit flow rate", this);
  connect(m_setFlowRateAction, SIGNAL(triggered()), this, SLOT(setFlowRate()));

  m_fileMenu->addAction(m_loadDatasetAction);
  m_fileMenu->addAction(m_loadAlphaAction);
  m_fileMenu->addAction(m_saveAlphaAction);
  m_fileMenu->addAction(m_quitAction);

  m_editMenu->addAction(m_analyzeAction);
  m_editMenu->addAction(m_setNumSamplesAction);
  m_editMenu->addAction(m_setFlowRateAction);

  menuBar()->addMenu(m_fileMenu);
  menuBar()->addMenu(m_editMenu);
  menuBar()->addMenu(m_helpMenu);
}

void VisWindow::createStatusBar()
{
  QStatusBar* statusbar = new QStatusBar;

  statusbar->showMessage("This is a message on the status bar.");

  setStatusBar(statusbar);
}

