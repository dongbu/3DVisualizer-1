#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <QApplication>

#include "visconfig.h"
#include "logger.h"
#include "viswindow.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// TODO Create a DataManager class that will replace the Dataset/TFunction/Alpha Manager classes.
//      Will need to implement substructs to hold the maps + active keys.
// TODO Create a thread to execute the topological analysis. Use an std::thread.
// TODO (related to the above todo) Modularize the Topological analysis process.
// TODO (later) Create a factory method for my geometry and replace the cube/axis/sphere/etc classes.
// TODO Create a base Manager class. Search for template inheritance, it may solve earlier issues.
// FIXME Make the rendering run in a separate thread.
// FIXME Give the option to use the alpha channel of the color transfer function.

int main(int argc, char** argv)
{
  Logger::getInstance()->setLogStream(&std::cout);

  QApplication app(argc, argv);
  VisWindow win(0, WINDOW_W, WINDOW_H);
  win.setWindowTitle("3D Visualizer");
  win.show();

  return app.exec();
}
