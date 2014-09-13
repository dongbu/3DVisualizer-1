#ifndef VISUALIZER_H
#define VISUALIZER_H

static int WINDOW_W = 700;
static int WINDOW_H = 700;

#define M_PI       3.14159265358979323846

#include <string>

#ifdef WIN32
  static const std::string RESOURCE_PATH = "../Resources";
#else
  static const char* RESOURCE_PATH = "../../Resources";
#endif // WIN32

#endif // VISUALIZER_H