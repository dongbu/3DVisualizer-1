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

