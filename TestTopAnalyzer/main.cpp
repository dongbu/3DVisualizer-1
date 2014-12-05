#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "logger.h"
#include "topanalyzer.h"
#include "knlconfig.h"

using namespace std;
static float s_flow_rate = 300;
static float s_avg_mult = 1.f;

void initGL();

int main(int argc, char** argv)
{
  Logger::getInstance()->setLogStream(&std::cout);

  if(argc == 1) {
    Logger::getInstance()->error("Dataset not chosen.");
    printf("Program usage: ./TestTopAnalyzer bonsai [flow_rate] [simp_avg_multiplier]\n");
    exit(1);
  }

  initGL();

  DatasetManager* instance = DatasetManager::getInstance();

  instance->init("/home/guilherme/Pictures/datasets/");
  instance->setActive(std::string(argv[1]));

  if(argc > 2) {
    s_flow_rate = atof(argv[2]);
    s_avg_mult = atof(argv[3]);
  }

  TopAnalyzer::getInstance()->testRun(static_cast<double>(s_flow_rate), s_avg_mult);

  instance->freeResources();
  DatasetManager::destroy();

  glfwTerminate();

  return 0;
}

void initGL()
{
  GLFWwindow* win;

  if(!glfwInit()) {
    cerr << "Error while initializing GLFW.\n";
    exit(1);
  }

  win = glfwCreateWindow(640, 480, "Dummy window", NULL, NULL);
  if(!win) {
    cerr << "Error while creating the window.\n";
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(win);

  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    cerr << "Error while initializing GLEW.\n";
    glfwTerminate();
    exit(1);
  }

}
