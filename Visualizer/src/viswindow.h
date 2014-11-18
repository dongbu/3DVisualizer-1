#ifndef VISWINDOW_H
#define VISWINDOW_H

#include <GL/glew.h>
#include <QMainWindow>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>

class VisWidget;

class VisWindow : public QMainWindow
{
  Q_OBJECT

public:
  VisWindow(QWidget *parent = 0, int w = 640, int h = 480);

protected:
//  virtual void mousePressEvent(QMouseEvent *);
//  virtual void mouseReleaseEvent(QMouseEvent *);
//  virtual void mouseMoveEvent(QMouseEvent *);
//  virtual void wheelEvent(QWheelEvent *);

  virtual void keyPressEvent(QKeyEvent *);
  virtual void keyReleaseEvent(QKeyEvent *);

  virtual void moveEvent(QMoveEvent *); // reduce sampling.
  virtual void resizeEvent(QResizeEvent *); //stop animation.
  virtual void closeEvent(QCloseEvent *); //free everything.

protected slots:
  void loadDataset();
  void loadAlphaDataset();
  void loadColorTFunction();

  void saveAlphaDataset();

  void analyzeDataset();
  void setNumSamples();
  void setRootDataDir();

private:
  VisWidget* m_renderWidget;

  QMenu* m_file;
  QAction* m_loadDataAction;
  QAction* m_saveAlphaAction;

  void createInterface();
};

#endif // VISWINDOW_H
