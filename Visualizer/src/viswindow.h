#ifndef VISWINDOW_H
#define VISWINDOW_H

#include <GL/glew.h>
#include <QMainWindow>
#include <QTime>
#include <QMenuBar>
#include <QMenu>

class VisWidget;

class VisWindow : public QMainWindow
{
  Q_OBJECT

public:
  VisWindow(QWidget *parent = 0, int w = 640, int h = 480);

protected slots:
  void draw();

private:
  VisWidget* m_renderWidget;
};

#endif // VISWINDOW_H
