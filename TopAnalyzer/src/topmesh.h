#ifndef MESH_H
#define MESH_H

#include "topdata.h"

namespace top
{

  /**
   * @brief The Mesh class
   * This class creates the connectivity information between the voxels. This is
   * needed by the libtourtre's functions. The only information stored is a
   * reference to the dataset being analyzed.
   */
  class Mesh 
  {
  public:
    top::Dataset& data;
    Mesh(top::Dataset& d) : data(d) {}

    void getNeighbors(size_t i, std::vector<size_t>& n);
    size_t getNeighbors(size_t idx, size_t* nbrs, size_t nbrs_size);
    void createGraph(std::vector<size_t>& order);
    size_t numVerts();

    void find6Neighbors(size_t x, size_t y, size_t z, std::vector<size_t>& neighbors);
    void find6Neighbors(size_t x, size_t y, size_t z, size_t* nbrs);
    void find18Neighbors(size_t x, size_t y, size_t z, std::vector<size_t>& neighbors);
    void find18Neighbors(size_t x, size_t y, size_t z, size_t* nbrs);
  };

}

#endif
