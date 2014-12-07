#ifndef TopData_H
#define TopData_H

#include <vector>
#include <iostream>
#include <cmath>

#include "knlconfig.h"

/**
 * @brief The top namespace encompases some of the classes and structures of the
 * TopAnalyzer module.
 */
namespace top
{

  /**
   * @brief The Dataset struct
   * This structure represents a dataset to be fed to the topological analyzer
   * for processing.
   *
   * The structure holds the dataset's minimum and maximum values as well as the
   * saddle points found by the analyzer. It also provides access to the
   * knl::Dataset structure underneath.
   */
  struct Dataset 
  {
    knl::Dataset* data;
    size_t size;
    size_t maxValue, minValue; //max and min values occuring in the data
    std::vector<size_t> saddles; //we will fill this up with all of the saddles that exist in the data

    Dataset();
    Dataset(knl::Dataset& rhs);
    ~Dataset();

    size_t Get(int idx)
    {
      return data->get(idx);
    }

    size_t convertIndex( size_t i, size_t j, size_t k )
    {
      return (k * data->height + j) * data->width + i;
    }

    void convertIndex( size_t id, size_t & x, size_t & y, size_t & z )
    {
      if (id >= size) {
        std::cerr << "Error: trying to convert the index of a saddle into xyz" << std::endl;
        x = y = z = 0xffffffff;
        return;
      }

      int size01 = data->width * data->height;

      z = id / size01;
      y = (id - z * size01) / data->width;
      x = id - z * size01 - y * data->width;
    }
    
    size_t operator()(int idx)
    {
      return Get(idx);
    }

    size_t operator()(int i, int j, int k)
    {
      size_t idx = convertIndex(i, j, k);
      return Get(idx);
    }

    //bool greater(size_t a, size_t b);
    //bool less(size_t a, size_t b);
  };

}

#endif // TopData_H
