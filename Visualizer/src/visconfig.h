#ifndef VISUALIZER_CONFIG_H
#define VISUALIZER_CONFIG_H

static int WINDOW_W = 700;
static int WINDOW_H = 700;

#define M_PI 3.14159265358979323846

#include <GL/glew.h>
#include <string>

static const std::string FPASS_KEY = "first_pass";
static const std::string SPASS_KEY = "second_pass";
static const std::string FBO_KEY = "back_faces";

#endif // VISUALIZER_CONFIG_H
