#ifndef RENDERER_H
#define RENDERER_H

#include <iup/iup.h>

enum RendererType
{
  GLSL,
  OPENCL
};

class Renderer
{
public:
  Renderer(enum RendererType t = RendererType::GLSL);
  ~Renderer();

  static int Init(int, char**);

  int cb_idle();
  int cb_keypress(Ihandle*, int, int);
  int cb_mousebutton(Ihandle*, int, int, int, int, char*);
  int cb_mousemotion(Ihandle*, int, int, char*);
  int cb_mousewheel(Ihandle*, float, int, int, char*);
  int cb_draw(Ihandle*, float, float);
};

#endif // RENDERER_H
