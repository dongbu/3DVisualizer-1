#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <QGuiApplication>

#include "config.h"
#include "logger.h"
#include "viswindow.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

int main(int argc, char** argv)
{
  Logger::GetInstance()->setLogStream(&std::cout);

  QGuiApplication app(argc, argv);
  VisWindow win(0, WINDOW_W, WINDOW_H);
  win.show();

  return app.exec();
}
