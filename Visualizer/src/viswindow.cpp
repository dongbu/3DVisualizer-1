#include "viswindow.h"
#include "config.h"
#include "viswidget.h"
#include "tinygl.h"
#include "alphamanager.h"
#include "topanalyzer.h"

#include <GL/glew.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QTimer>
#include <QMainWindow>
#include <QApplication>

VisWindow::VisWindow(QWidget* parent, int w, int h) :
  QMainWindow(parent)
{
  QGLFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(3, 3);
  format.setSamples(4);
  format.setProfile(QGLFormat::CoreProfile);

  resize(w, h);
  create();

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
    TopAnalyzer::GetInstance()->AnalyzeCurrDataset(m_renderWidget->GetFlowRate(), DatasetManager::GetInstance()->GetCurrentKey());
    AlphaManager::GetInstance()->SetActive(DatasetManager::GetInstance()->GetCurrentKey(), GL_TEXTURE2);
    break;
  case Qt::Key_1:
    TFManager::GetInstance()->SetActive("tff1", GL_TEXTURE3);
    break;
  case Qt::Key_2:
    TFManager::GetInstance()->SetActive("tff2", GL_TEXTURE3);
    break;
  case Qt::Key_F1:
    DatasetManager::GetInstance()->SetActive("neghip", GL_TEXTURE1);
    break;
  case Qt::Key_F2:
    DatasetManager::GetInstance()->SetActive("bonsai", GL_TEXTURE1);
    break;
  case Qt::Key_F3:
    DatasetManager::GetInstance()->SetActive("nucleon", GL_TEXTURE1);
    break;
  case Qt::Key_F4:
    DatasetManager::GetInstance()->SetActive("silicium", GL_TEXTURE1);
    break;
  case Qt::Key_F5:
    DatasetManager::GetInstance()->SetActive("fuel", GL_TEXTURE1);
    break;
  case Qt::Key_F6:
    DatasetManager::GetInstance()->SetActive("BostonTeapot", GL_TEXTURE1);
    break;
  case Qt::Key_F7:
    DatasetManager::GetInstance()->SetActive("lobster", GL_TEXTURE1);
    break;
  case Qt::Key_F8:
    DatasetManager::GetInstance()->SetActive("foot", GL_TEXTURE1);
    break;
  case Qt::Key_F9:
    DatasetManager::GetInstance()->SetActive("CT-Knee", GL_TEXTURE1);
    break;
  }
}

void VisWindow::keyReleaseEvent(QKeyEvent*)
{

}

void VisWindow::moveEvent(QMoveEvent*)
{

}

void VisWindow::resizeEvent(QResizeEvent*)
{

}

void VisWindow::closeEvent(QCloseEvent*)
{

}

