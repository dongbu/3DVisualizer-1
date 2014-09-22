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
    minValue = maxValue = rhs.Get(0);
    data = new knl::Dataset(rhs);

    for(size_t i = 0; i < size; i++) {
      size_t val = rhs.Get(i);
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
  }
  //bool Data::less( uint a, uint b ) 
  //{
  //  //use overloaded [] operator to access saddles
  //  if (compareEqual( (*this)[a],(*this)[b])) return a < b;
  //  else return compareLess((*this)[a],(*this)[b]);
  //}
  //
  //bool Data::greater( uint a, uint b ) 
  //{
  //  if (compareEqual((*this)[a],(*this)[b])) return a > b;
  //  else return compareLess((*this)[b],(*this)[a]);
  //}

  //Data::Data(void* arr, uint* dims)
  //{
  //  if(!arr || !dims) {
  //    std::cerr << "ERROR: Invalid data or dimension pointer" << std::endl;
  //    return;
  //  }
  //
  //  memcpy(size, dims, 3 * sizeof(uint));
  //  totalSize = dims[0] * dims[1] * dims[2];
  //  data = new DataType[totalSize];
  //  memcpy(data, arr, totalSize * sizeof(DataType));
  //
  //  maxValue = minValue = data[0];
  //  for (uint i = 0; i < totalSize; i++) {
  //    if (data[i] > maxValue) maxValue = data[i];
  //    if (data[i] < minValue) minValue = data[i];
  //  }
  //}

}