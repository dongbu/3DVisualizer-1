#include "viswindow.h"
#include "visconfig.h"
#include "viswidget.h"
#include "tinygl.h"
#include "alphamanager.h"
#include "topanalyzer.h"
#include "datasetmanager.h"

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
#include <QInputDialog>

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
    TopAnalyzer::getInstance()->AnalyzeCurrDataset(m_renderWidget->getFlowRate(), DatasetManager::getInstance()->getCurrentKey());
    AlphaManager::getInstance()->setActive(DatasetManager::getInstance()->getCurrentKey(), GL_TEXTURE2);
    break;
  case Qt::Key_1:
    m_renderWidget->stopTimer();
    m_renderWidget->loadColorTF("tff1");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_2:
    m_renderWidget->stopTimer();
    m_renderWidget->loadColorTF("tff2");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F1:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("neghip");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F2:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("bonsai");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F3:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("nucleon");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F4:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("silicium");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F5:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("fuel");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F6:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("BostonTeapot");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F7:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("lobster");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F8:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("foot");
    m_renderWidget->startTimer(16);
    break;
  case Qt::Key_F9:
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset("CT-Knee");
    m_renderWidget->startTimer(16);
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

  vector<string> vkeys = DatasetManager::getInstance()->getKeys();
  QStringList keys;
  for(auto it : vkeys) {
    keys << QString(it.c_str());
  }

  bool ok;
  QString key = QInputDialog::getItem(this, tr("Load Dataset"), tr("Dataset:"), keys, 0, false, &ok);
  if(ok && !key.isEmpty()) {
    m_renderWidget->stopTimer();
    m_renderWidget->loadDataset(key.toStdString());
    m_renderWidget->startTimer(16);
  }
}

void VisWindow::loadAlphaTF()
{}

void VisWindow::loadColorTF()
{
  using std::vector;
  using std::string;

  vector<string> vkeys = TFManager::getInstance()->getKeys();
  QStringList keys;

  for(auto it : vkeys) {
    keys << QString(it.c_str());
  }

  bool ok;
  QString key = QInputDialog::getItem(this, tr("Load Color Map"), tr("Color Map:"), keys, 0, false, &ok);
  if(ok && !key.isEmpty()) {
    m_renderWidget->stopTimer();
    m_renderWidget->loadColorTF(key.toStdString());
    m_renderWidget->startTimer(16);
  }
}

void VisWindow::saveAlphaTF()
{

}

void VisWindow::analyzeDataset()
{
  TopAnalyzer::getInstance()->AnalyzeCurrDataset(m_renderWidget->getFlowRate(), DatasetManager::getInstance()->getCurrentKey());
  AlphaManager::getInstance()->setActive(DatasetManager::getInstance()->getCurrentKey(), GL_TEXTURE2);
}

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

  m_loadColorAction = new QAction("Load color map", this);
  connect(m_loadColorAction, SIGNAL(triggered()), this, SLOT(loadColorTF()));

  m_loadAlphaAction = new QAction("Load alpha map", this);
  connect(m_loadAlphaAction, SIGNAL(triggered()), this, SLOT(loadAlphaTF()));

  m_saveAlphaAction = new QAction("Save alpha map", this);
  connect(m_saveAlphaAction, SIGNAL(triggered()), this, SLOT(saveAlphaTF()));

  m_quitAction = new QAction("Exit", this);
  connect(m_quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  m_analyzeAction = new QAction("Analyze dataset", this);
  connect(m_analyzeAction, SIGNAL(triggered()), this, SLOT(analyzeDataset()));

  m_setNumSamplesAction = new QAction("Edit sampling rate", this);
  connect(m_setNumSamplesAction, SIGNAL(triggered()), this, SLOT(setNumSamples()));

  m_setFlowRateAction = new QAction("Edit flow rate", this);
  connect(m_setFlowRateAction, SIGNAL(triggered()), this, SLOT(setFlowRate()));

  m_fileMenu->addAction(m_loadDatasetAction);
  m_fileMenu->addAction(m_loadColorAction);
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

