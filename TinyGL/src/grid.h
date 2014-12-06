#ifndef GRID_H
#define GRID_H

#include "mesh.h"

/**
 * @brief The Grid class.
 *
 * This class builds a grid, given the number of horizontal and vertical
 * subdivisions. The grid always begins at (0,0,0) and ends at (1,1,1) no matter
 * how many subdivisions are specified. The draw function is given as a
 * callback to the Mesh.
 */
class Grid : public Mesh
{
public:
  Grid(int nx, int ny);
  virtual ~Grid();
};

#endif
