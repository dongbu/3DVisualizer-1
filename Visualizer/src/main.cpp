#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <QGuiApplication>

//#ifdef WIN32
//  #include <iup.h>
//  #include <iupgl.h>
//  #include <iupkey.h>
//#else
//  #include <iup/iup.h>
//  #include <iup/iupgl.h>
//  #include <iup/iupkey.h>
//#endif

#include "config.h"
#include "logger.h"
#include "viswindow.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//DVRWindow* g_window;
//Ihandle* canvas;

//static void testSimplification();
//static GLuint initGLEW();
//static void initIUP();

//static int cb_idle();
//static int cb_keypress(Ihandle*, int, int);
//static int cb_mousebutton(Ihandle*, int, int, int, int, char*);
//static int cb_mousemove(Ihandle*, int, int, char*);
//static int cb_draw(Ihandle*, float, float);

int main(int argc, char** argv)
{
//  IupOpen(&argc, &argv);
//  IupGLCanvasOpen();

  Logger::GetInstance()->setLogStream(&std::cout);

  QGuiApplication app(argc, argv);
  VisWindow win(0, WINDOW_W, WINDOW_H);
  win.show();

  return app.exec();

//  initIUP();
//  initGLEW();

//  g_window = new DVRWindow(WINDOW_W, WINDOW_H);
//  g_window->init();
  
//  IupMainLoop();

//  IupClose();

//  delete g_window;
//  g_window = nullptr;

  return 0;
}

//void initIUP()
//{
//  Ihandle* dlg;
//  canvas = IupGLCanvas(NULL);

//  IupSetFunction("IDLE_ACTION", (Icallback) cb_idle);
//  IupSetCallback(canvas, "ACTION", (Icallback) cb_draw);
//  IupSetCallback(canvas, "KEYPRESS_CB", (Icallback) cb_keypress);

//  IupSetCallback(canvas, "BUTTON_CB", (Icallback) cb_mousebutton);
//  IupSetCallback(canvas, "MOTION_CB", (Icallback) cb_mousemove);
//  //  IupSetCallback(canvas, "WHEEL_CB", (Icallback) cb_mousewheel);

//  IupSetAttribute(canvas, IUP_BUFFER, IUP_DOUBLE);

//  char* rastersize = (char*) calloc(30, sizeof(char));
//  sprintf(rastersize, "%dx%d", WINDOW_W, WINDOW_H);

//  IupSetAttribute(canvas, "RASTERSIZE", rastersize);
//  free(rastersize);

//  IupGLMakeCurrent(canvas);

//  dlg = IupDialog(IupHbox(IupFill(), canvas, NULL));
//  IupSetAttribute(dlg, "TITLE", "My first IUP window");
//  IupMap(dlg);

//  IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
//}

//GLuint initGLEW()
//{
//  glewExperimental = true;
//  GLenum err = glewInit();
//  if(err != GLEW_OK) {
//    std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
//    return 1;
//  }
//  return 0;
//}

//void testSimplification()
//{
//  DatasetManager::GetInstance()->Init("/home/guilherme/Pictures/datasets/");
//
//  std::cout << "\n\n\n--------------Neghip--------------\n";
//  DatasetManager::GetInstance()->SetActive("neghip", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Hydrogen Atom--------------\n";
//  DatasetManager::GetInstance()->SetActive("hydrogenAtom", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Shockwave--------------\n";
//  DatasetManager::GetInstance()->SetActive("shockwave", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Bonsai--------------\n";
//  DatasetManager::GetInstance()->SetActive("bonsai", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Foot--------------\n";
//  DatasetManager::GetInstance()->SetActive("foot", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Boston Teapot--------------\n";
//  DatasetManager::GetInstance()->SetActive("BostonTeapot", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------CT-Knee--------------\n";
//  DatasetManager::GetInstance()->SetActive("CT-Knee", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//
//  std::cout << "\n\n\n--------------Stent--------------\n";
//  DatasetManager::GetInstance()->SetActive("stent8", GL_TEXTURE1);
//  TopAnalyzer::GetInstance()->AnalyzeCurrDataset(g_flowRate, DatasetManager::GetInstance()->GetCurrentKey());
//}

//static int cb_keypress(Ihandle*, int c, int press)
//{
//  if(g_window) g_window->keypress(c, press);
//  return IUP_DEFAULT;
//}

//static int cb_idle()
//{
//  if(g_window) g_window->update();
//  return cb_draw(canvas, 0.f, 0.f);
//}

//static int cb_mousebutton(Ihandle* win, int button, int pressed, int x, int y, char* c)
//{
//  if(g_window) g_window->mousebutton(button, pressed, x, y);
//  return IUP_DEFAULT;
//}

//static int cb_mousemove(Ihandle* win, int x, int y, char* a)
//{
//  if(g_window) g_window->mousemove(x, y);
//  return IUP_DEFAULT;
//}

//static int cb_draw(Ihandle* ih, float a, float b)
//{
//  IupGLMakeCurrent(ih);
//  if(g_window) g_window->draw();
//  IupGLSwapBuffers(ih);
//  return IUP_DEFAULT;
//}
