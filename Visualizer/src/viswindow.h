#ifndef VISWINDOW_H
#define VISWINDOW_H

class VisWindow
{
public:
  VisWindow(int w = 640, int h = 480);
  virtual ~VisWindow();

  /**
   * Pure virtual methods. Callbacks to be implemented by the derived classes.
   */
  virtual int init() = 0;
  virtual void update() = 0;
  virtual void resize(int w, int h) = 0;
  virtual void draw() = 0;
  virtual void keypress(int key, int press) = 0;
  virtual void mousebutton(int button, int press, int x, int y) = 0;
  virtual void mousemove(int x, int y) = 0;
  virtual void mousewheel(float dist, int x, int y) = 0;

  int GetWidth()
  {
    return m_width;
  }

  int GetHeight()
  {
    return m_height;
  }

  void SetWidth(int w)
  {
    m_width = w;
  }

  void SetHeight(int h)
  {
    m_height = h;
  }

protected:
  int m_width;
  int m_height;
};

#endif // VISWINDOW_H