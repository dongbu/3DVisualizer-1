#ifndef AXIS_H
#define AXIS_H

#include <glm/glm.hpp>
#include "mesh.h"

/**
 * @brief The Axis class.
 *
 * This class creates a set of x, y, z axes. The limits of each axis can be
 * configured at build time.
 */
class Axis : public Mesh
{
public:
  Axis(glm::vec2 xlimits = glm::vec2(0, 1),
       glm::vec2 ylimits = glm::vec2(0, 1),
       glm::vec2 zlimits = glm::vec2(0, 1));

  virtual ~Axis();
};

#endif // AXIS_H
