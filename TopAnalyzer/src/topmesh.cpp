#include "topmesh.h"
#include "logger.h"

#include <tbb/tbb.h>
#include <tbb/parallel_sort.h>
#include <iostream>
#include <algorithm>

namespace top
{
  //functor for sorting
  class AscendingOrder
  {
    top::Dataset* data;
  public:
    AscendingOrder(top::Dataset* d) : data(d) {}

    bool operator()(const size_t & a, const size_t & b) const {
      if(data->data->get(a) == data->data->get(b))
        return a < b;
      return data->data->get(a) < data->data->get(b);
    }
  };

  void Mesh::createGraph(std::vector<size_t>& order) 
  {
    order.resize(data.size);
    for(unsigned int i = 0; i < data.size; i++) order[i] = i;
    //bolt::cl::sort(order.begin(), order.end(), AscendingOrder(&data));
    tbb::parallel_sort(order.begin(), order.end(), AscendingOrder(&data));
    //std::sort(order.begin(), order.end(), AscendingOrder(&data));
  }

  void Mesh::getNeighbors(size_t i, std::vector<size_t>& n) 
  {
    size_t x, y, z;
    data.convertIndex(i, x, y, z);

    if((x + y + z) % 2 == 1)
      find6Neighbors(x, y, z, n);
    else
      find18Neighbors(x, y, z, n);
  }

  size_t Mesh::getNeighbors(size_t idx, size_t* nbrs, size_t nbrs_size)
  {
    size_t x, y, z;
    data.convertIndex(idx, x, y, z);
    std::memset(nbrs, 0, nbrs_size * sizeof(size_t));

    if((x + y + z) % 2 == 1)
      find6Neighbors(x, y, z, nbrs);
    else
      find18Neighbors(x, y, z, nbrs);

    int sz = 0;
    for(size_t i = 0; i < nbrs_size; i++) if(nbrs[i] != 0) sz++;
    return sz;
  }

  void Mesh::find6Neighbors(size_t x, size_t y, size_t z, size_t* nbrs)
  {
    size_t nx[6],ny[6],nz[6];

    for(size_t i = 0; i < 6; i++) {
      nx[i] = x;
      ny[i] = y;
      nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    for(size_t i = 0, j = 0; i < 6; i++) {
      if (nx[i] >= data.data->width) continue;
      if (ny[i] >= data.data->height) continue;
      if (nz[i] >= data.data->slices) continue;

      nbrs[j++] = data.convertIndex(nx[i], ny[i], nz[i]);
    }
  }

  void Mesh::find6Neighbors(size_t x, size_t y, size_t z, std::vector< size_t > & neighbors) 
  {
    size_t nx[6],ny[6],nz[6];

    for(size_t i = 0; i < 6; i++) {
      nx[i] = x;
      ny[i] = y;
      nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    for(size_t i = 0; i < 6; i++) {
      if (nx[i] >= data.data->width) continue;
      if (ny[i] >= data.data->height) continue;
      if (nz[i] >= data.data->slices) continue;

      neighbors.push_back(data.convertIndex(nx[i], ny[i], nz[i]));
    }
  }

  void Mesh::find18Neighbors(size_t x, size_t y, size_t z, std::vector< size_t > & neighbors) 
  {
    size_t nx[18],ny[18],nz[18];

    for(size_t i = 0; i < 18; i++) {
      nx[i] = x;
      ny[i] = y;
      nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    //the rest of the 18
    nx[6] -= 1; ny[6]  -= 1;
    nx[7] += 1; ny[7]  -= 1;
    ny[8] -= 1; nz[8]  -= 1;
    ny[9] += 1; nz[9]  -= 1;
    nz[10]-= 1; nx[10] -= 1;
    nz[11]+= 1; nx[11] -= 1;

    nx[12] -= 1; ny[12] += 1;
    nx[13] += 1; ny[13] += 1;
    ny[14] -= 1; nz[14] += 1;
    ny[15] += 1; nz[15] += 1;
    nz[16] -= 1; nx[16] += 1;
    nz[17] += 1; nx[17] += 1;

    for(size_t i = 0; i < 18; i++) {
      if(nx[i] >= data.data->width) continue;
      if(ny[i] >= data.data->height) continue;
      if(nz[i] >= data.data->slices) continue;

      neighbors.push_back(data.convertIndex(nx[i], ny[i], nz[i]));
    }
  }

  void Mesh::find18Neighbors(size_t x, size_t y, size_t z, size_t* nbrs)
  {
    size_t nx[18],ny[18],nz[18];

    for(size_t i = 0; i < 18; i++) {
      nx[i] = x;
      ny[i] = y;
      nz[i] = z;
    }

    //first 6 neighbors
    nx[0] -= 1;
    ny[1] -= 1;
    nz[2] -= 1;
    nx[3] += 1;
    ny[4] += 1;
    nz[5] += 1;

    //the rest of the 18
    nx[6] -= 1; ny[6]  -= 1;
    nx[7] += 1; ny[7]  -= 1;
    ny[8] -= 1; nz[8]  -= 1;
    ny[9] += 1; nz[9]  -= 1;
    nz[10]-= 1; nx[10] -= 1;
    nz[11]+= 1; nx[11] -= 1;

    nx[12] -= 1; ny[12] += 1;
    nx[13] += 1; ny[13] += 1;
    ny[14] -= 1; nz[14] += 1;
    ny[15] += 1; nz[15] += 1;
    nz[16] -= 1; nx[16] += 1;
    nz[17] += 1; nx[17] += 1;

    for(size_t i = 0, j = 0; i < 18; i++) {
      if(nx[i] >= data.data->width) continue;
      if(ny[i] >= data.data->height) continue;
      if(nz[i] >= data.data->slices) continue;

      nbrs[j++] = data.convertIndex(nx[i], ny[i], nz[i]);
    }
  }

}
