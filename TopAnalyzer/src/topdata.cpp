#include "topdata.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

namespace top
{

  Dataset::Dataset()
  {
    data = NULL;
    size = maxValue = 0;
    minValue = 0xffffffff;
  }

  Dataset::Dataset(knl::Dataset& rhs)
  {
    size = rhs.width * rhs.height * rhs.slices;
    minValue = maxValue = rhs.get(0);
    data = &rhs;

    for(size_t i = 0; i < size; i++) {
      size_t val = rhs.get(i);
      if(val > maxValue)
        maxValue = val;
      if(val < minValue)
        minValue = val;
    }
  }

  Dataset::~Dataset()
  {
    size = 0;
    saddles.clear();
    data = NULL;
  }

}
