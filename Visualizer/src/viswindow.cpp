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
#include <QMessageBox>

static void show_error_msg(const char* error_msg)
{
  QMessageBox error_box;
  error_box.setText(error_msg);
  error_box.exec();
}

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
    m_renderWidget->loadMetafile(path);
  }
}

void VisWindow::contextMenuEvent(QContextMenuEvent* e)
{
  QMenu tmp(this);
  tmp.addAction(m_buildCTAction);
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
    TopAnalyzer::getInstance()->analyzeCurrDataset(m_renderWidget->getFlowRate(), DatasetManager::getInstance()->getCurrentKey());
    AlphaManager::getInstance()->setActive(DatasetManager::getInstance()->getCurrentKey(), GL_TEXTURE2);
    m_renderWidget->analyze();
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
{
  delete m_renderWidget;

  DatasetManager::getInstance()->freeResources();
  TFManager::getInstance()->freeResources();
  AlphaManager::getInstance()->freeResources();
  TinyGL::getInstance()->freeResources();

  DatasetManager::destroy();
  TFManager::destroy();
  AlphaManager::destroy();
  TinyGL::destroy();

  QApplication::instance()->quit();
}

void VisWindow::buildContourTree()
{
  std::string key = DatasetManager::getInstance()->getCurrentKey();
  if(key.empty()) {
    show_error_msg("Error: No valid dataset loaded. Unable to build the contour tree.");
    return;
  }

  m_renderWidget->buildContourTree();
}

void VisWindow::buildAlphaMap()
{
  bool ok;
  QString qkey = QInputDialog::getText(this, tr("Build Alpha Map"), tr("Map name:"), QLineEdit::Normal, QString(), &ok);

  if(ok && !qkey.isEmpty())
    m_renderWidget->buildAlphaMap(qkey.toStdString());
}

void VisWindow::simplifyTree()
{
  std::string key = DatasetManager::getInstance()->getCurrentKey();
  if(key.empty()) {
    show_error_msg("Error: No valid dataset loaded. Unable to simplify the contour tree.");
    return;
  }

  m_renderWidget->simplifyContourTree();
}

void VisWindow::flowOpacity()
{
  bool ok;
  double flow_rate = QInputDialog::getDouble(this, "Flow rate", "Rate:", 300.0, 0.0, 2000.0, 1, &ok);

  if(ok && flow_rate > 0.0)
    m_renderWidget->flowOpacity(flow_rate);
}

void VisWindow::loadDataset()
{
  using std::vector;
  using std::string;
  using std::cout;
  using std::endl;

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
{
  using std::vector;
  using std::string;

  std::string data_key = DatasetManager::getInstance()->getCurrentKey();

  if(data_key.empty()) {
    show_error_msg("Error: No dataset loaded. Cannot load a alpha map.");
    return;
  }

  vector<string> vkeys = AlphaManager::getInstance()->getKeys();
  QStringList keys;
  for(auto it : vkeys) {

    //if(data_key == )
    keys << QString(it.c_str());
  }

  bool ok;
  QString key = QInputDialog::getItem(this, tr("Load Alpha Map"), tr("Map:"), keys, 0, false, &ok);
  if(ok && !key.isEmpty()) {
    m_renderWidget->stopTimer();
    m_renderWidget->loadAlphaTF(key.toStdString());
    m_renderWidget->startTimer(16);
  }
}

void VisWindow::loadColorTF()
{
  using std::vector;
  using std::string;

  knl::Dataset* data = DatasetManager::getInstance()->getCurrent();

  if(!data) {
    show_error_msg("Error: No dataset loaded. Cannot load a color map.");
    return;
  }

  TFManager* tfinstance = TFManager::getInstance();

  vector<string> vkeys = tfinstance->getKeys();
  QStringList keys;

  for(auto it : vkeys) {
    TFunction* tf = tfinstance->get(it.c_str());
    if(tf->bytes_elem == data->bytes_elem &&
       tf->num_channels > 1) {
      keys << QString(it.c_str());
    }
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
  std::string data_key = DatasetManager::getInstance()->getCurrentKey();
  std::string alpha_key = AlphaManager::getInstance()->getCurrentKey();

  if(data_key.empty() || alpha_key.empty()) {
    show_error_msg("Error: No dataset/alpha map loaded. Cannot save the alpha map.");
    return;
  }

  m_renderWidget->saveAlphaTF(alpha_key, data_key);
}

void VisWindow::createPyroclasticVol()
{
  bool ok;
  QString qkey = QInputDialog::getText(this, tr("Create Pyroclastic Volume"), tr("Volume name:"), QLineEdit::Normal, QString(), &ok);

  if(ok && !qkey.isEmpty()) {
    m_renderWidget->stopTimer();
    m_renderWidget->createPyroVol(qkey.toStdString());
    m_renderWidget->startTimer(16);
  }
}

void VisWindow::createInterface()
{
  createMenus();
  createStatusBar();
}

void VisWindow::createMenus()
{
  m_fileMenu = new QMenu("File", this);
  m_analyzeMenu = new QMenu("Analyze", this);
  m_helpMenu = new QMenu("Help", this);

  m_loadDatasetAction = new QAction("Load dataset", this);
  connect(m_loadDatasetAction, SIGNAL(triggered()), this, SLOT(loadDataset()));

  m_loadColorAction = new QAction("Load color map", this);
  connect(m_loadColorAction, SIGNAL(triggered()), this, SLOT(loadColorTF()));

  m_loadAlphaAction = new QAction("Load alpha map", this);
  connect(m_loadAlphaAction, SIGNAL(triggered()), this, SLOT(loadAlphaTF()));

  m_saveAlphaAction = new QAction("Save alpha map", this);
  connect(m_saveAlphaAction, SIGNAL(triggered()), this, SLOT(saveAlphaTF()));

  m_createPyroVolAction = new QAction("Create synthetic volume", this);
  connect(m_createPyroVolAction, SIGNAL(triggered()), this, SLOT(createPyroclasticVol()));

  m_quitAction = new QAction("Exit", this);
  connect(m_quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  m_buildCTAction = new QAction("Build Contour Tree", this);
  connect(m_buildCTAction, SIGNAL(triggered()), this, SLOT(buildContourTree()));

  m_simpCTAction = new QAction("Simplify Contour Tree", this);
  connect(m_simpCTAction, SIGNAL(triggered()), this, SLOT(simplifyTree()));

  m_flowOpAction = new QAction("Flow Opacity", this);
  connect(m_flowOpAction, SIGNAL(triggered()), this, SLOT(flowOpacity()));

  m_buildAlphaAction = new QAction("Build Alpha Map", this);
  connect(m_buildAlphaAction, SIGNAL(triggered()), this, SLOT(buildAlphaMap()));

  m_fileMenu->addAction(m_createPyroVolAction);
  m_fileMenu->addAction(m_loadDatasetAction);
  m_fileMenu->addAction(m_loadColorAction);
  m_fileMenu->addAction(m_loadAlphaAction);
  m_fileMenu->addAction(m_saveAlphaAction);
  m_fileMenu->addAction(m_quitAction);

  m_analyzeMenu->addAction(m_buildCTAction);
  m_analyzeMenu->addAction(m_simpCTAction);
  m_analyzeMenu->addAction(m_flowOpAction);
  m_analyzeMenu->addAction(m_buildAlphaAction);

  menuBar()->addMenu(m_fileMenu);
  menuBar()->addMenu(m_analyzeMenu);
  menuBar()->addMenu(m_helpMenu);
}

void VisWindow::createStatusBar()
{
  QStatusBar* statusbar = new QStatusBar;

  statusbar->showMessage("This is a message on the status bar.");

  setStatusBar(statusbar);
}

