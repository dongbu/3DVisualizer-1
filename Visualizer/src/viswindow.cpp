#include "viswindow.h"
#include "config.h"
#include "viswidget.h"

#include <GL/glew.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QTimer>
#include <QMainWindow>

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

void VisWindow::draw()
{
//  m_context->makeCurrent(this);
//  RendererManager::GetInstance()->GetCurrent()->draw();
  //  m_context->swapBuffers(this);
}

void VisWindow::mousePressEvent(QMouseEvent*)
{

}

void VisWindow::mouseReleaseEvent(QMouseEvent*)
{

}

void VisWindow::mouseMoveEvent(QMouseEvent*)
{

}

void VisWindow::keyPressEvent(QKeyEvent*)
{

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

