#ifndef QUAD_H
#define QUAD_H

#include "mesh.h"

/**
 * @brief The Quad class
 *
 * This class creates a 2D quadrilateral Mesh. The main intention of this class
 * is to render textures on the screen.
 *
 */
class Quad : public Mesh
{
public:
  Quad();
  ~Quad();
};

#endif // QUAD_H
