#ifndef TFUNC_H
#define TFUNC_H

#include "datahandler.h"
#include "datatransfer.h"

class TFunction
{
public:
  void* data;
  size_t bytes_elem;

  TFunction();
  TFunction(TFunction& rhs);
  ~TFunction();

  bool UploadToGPU();

  static TFunction* Load(std::string path);
  static bool Save(std::string path, TFunction* tf);
};

#endif // TFUNC_H
