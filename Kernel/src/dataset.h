#ifndef DATASET_H
#define DATASET_H

#include "datahandler.h"
#include "datatransfer.h"


class Dataset
{
public:

  void* data;
  size_t width;
  size_t height;
  size_t slices;
  size_t bytes_elem;

  Dataset();
  Dataset(Dataset& rhs);
  ~Dataset();

  bool UploadToGPU();

  static Dataset* Load(std::string path);
  static bool Save(std::string path, Dataset* data);
};

#endif // DATASET_H
