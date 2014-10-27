#ifndef VISUALIZER_H
#define VISUALIZER_H

static int WINDOW_W = 700;
static int WINDOW_H = 700;

#define M_PI 3.14159265358979323846

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "datasetmanager.h"
#include "tfmanager.h"

static const std::string FPASS_KEY = "first_pass";
static const std::string SPASS_KEY = "second_pass";
static const std::string FBO_KEY = "back_faces";

#ifdef WIN32
  static const std::string RESOURCE_PATH = "../Resources";
#else
  static const char* RESOURCE_PATH = ".";
#endif // WIN32

#endif // VISUALIZER_H
