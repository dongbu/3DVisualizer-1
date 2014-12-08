#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"

/**
 * @brief The Cube class.
 *
 * This class creates a standard unit cube with center on the origin.
 */
class Cube : public Mesh
{
public:
  Cube();
  ~Cube();
};

#endif // CUBE_H
