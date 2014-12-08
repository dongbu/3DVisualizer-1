#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

/**
 * @brief The Light class
 *
 * The Light class abstracts the creation of a light source to be used for
 * rendering. The light source only has a color and a position in space.
 */
class Light
{
public:
  Light();
  Light(glm::vec3 pos, glm::vec3 color = glm::vec3(1, 1, 1));

  ~Light();

  void setPosition(glm::vec3 rhs)
  {
    m_position = rhs;
  }

  void setColor(glm::vec3 rhs)
  {
    m_color = rhs;
  }

  glm::vec3 getPosition()
  {
    return m_position;
  }

  glm::vec3 getColor()
  {
    return m_color;
  }

private:
  glm::vec3 m_position;
  glm::vec3 m_color;
};

#endif // LIGHT_H
