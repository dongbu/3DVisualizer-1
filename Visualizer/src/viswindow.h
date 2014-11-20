#ifndef VISWINDOW_H
#define VISWINDOW_H

#include <GL/glew.h>
#include <QMainWindow>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QLabel>

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

  void contextMenuEvent(QContextMenuEvent *);

  virtual void keyPressEvent(QKeyEvent *);
  virtual void keyReleaseEvent(QKeyEvent *);

  virtual void moveEvent(QMoveEvent *); // reduce sampling.
  virtual void resizeEvent(QResizeEvent *); //stop animation.
  virtual void closeEvent(QCloseEvent *); //free everything.

protected slots:
  void loadDataset();
  void loadAlphaDataset();
  void saveAlphaDataset();
  void loadColorTFunction();
  void quit();

  void analyzeDataset();
  void setNumSamples();
  void setRootDataDir();
  void setFlowRate();

private:
  VisWidget* m_renderWidget;

  QMenu* m_fileMenu;
  QMenu* m_editMenu;
  QMenu* m_helpMenu;

  QAction* m_loadMetafileAction;
  QAction* m_loadDatasetAction;
  QAction* m_saveAlphaAction;
  QAction* m_loadAlphaAction;
  QAction* m_quitAction;

  QAction* m_analyzeAction;
  QAction* m_setNumSamplesAction;
  QAction* m_setFlowRateAction;

  void createInterface();
  void createMenus();
  void createStatusBar();
};

#endif // VISWINDOW_H
