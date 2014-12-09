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

/**
 * @brief The VisWindow class
 * This class creates the user interface and implements the slots to dispatch
 * any commands to the OpenGL widget.
 */
class VisWindow : public QMainWindow
{
  Q_OBJECT

public:
  VisWindow(QWidget *parent = 0,
            int w = 640,
            int h = 480,
            std::string path = "");

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
  void loadColorTF();
  void loadAlphaTF();
  void saveAlphaTF();
  void createPyroclasticVol();

  void quit();

  void buildContourTree();
  void simplifyTree();
  void flowOpacity();
  void buildAlphaMap();

private:
  VisWidget* m_renderWidget;

  QMenu* m_fileMenu;
  QMenu* m_analyzeMenu;
  QMenu* m_helpMenu;

  QAction* m_loadMetafileAction;
  QAction* m_loadDatasetAction;
  QAction* m_saveAlphaAction;
  QAction* m_loadAlphaAction;
  QAction* m_loadColorAction;
  QAction* m_createPyroVolAction;
  QAction* m_quitAction;

  QAction* m_buildCTAction;
  QAction* m_simpCTAction;
  QAction* m_flowOpAction;
  QAction* m_buildAlphaAction;

  QAction* m_setNumSamplesAction;
  QAction* m_setFlowRateAction;

  void createInterface();
  void createMenus();
  void createStatusBar();
};

#endif // VISWINDOW_H
